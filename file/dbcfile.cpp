#include "dbcfile.h"

DBCFile::DBCFile(QString fileName) : WoWFile(filename)
    , letters("NONE")
{
    dbcfile = new QFile(fileName);
    if(!dbcfile->open(QIODevice::ReadWrite))
        std::cout << "couldnt open file";

    dbcfile->read(&letters[0],4);
    dbcfile->read((char*)&records,4);
    dbcfile->read((char*)&fields,4);
    dbcfile->read((char*)&recordSize,4);
    dbcfile->read((char*)&stringSize,4);

    fieldSize = recordSize / fields;

    data = new char[recordSize*records];
    stringData = new char[stringSize];

    dbcfile->read(data,recordSize*records);
    dbcfile->read(stringData,stringSize);

    std::cout << get<float>(records-1,0) << std::endl;
    int value = 1;
    set(records-1,0,1.111f);

    std::cout << get<float>(records-1,0) << std::endl;

}

std::string DBCFile::getString(int record, int field)
{
    return (std::string)(stringData+get<int>(record,field));
}

std::string DBCFile::getLoc(int record, int firstField)
{
    for(int i = firstField; i < firstField+9; ++i)
        if(!getString(record,i).empty())
            return getString(record,i);
}

template<typename T> T DBCFile::get (int record, int field)
{
    return (T)*reinterpret_cast<const T*> (data+record*recordSize+fieldSize*field);
}

template<typename T> void DBCFile::set (int record, int field, T value)
{
    set(record,field,value,fieldSize);
}

template<typename T> void DBCFile::set (int record, int field, T value, int size)
{
    char *fp = (char*)&value;
    char *op = data+record*recordSize+fieldSize*field;
    while(op < data+record*recordSize+fieldSize*field+size)
    {
         *op++ = *fp++;
    }
}

void DBCFile::save()
{
    if(dbcfile->reset())
    {
        dbcfile->write(&letters[0],4);
        dbcfile->write((char*)&records,4);
        dbcfile->write((char*)&fields,4);
        dbcfile->write((char*)&recordSize,4);
        dbcfile->write((char*)&stringSize,4);
        dbcfile->write(data,recordSize*records);
        dbcfile->write(stringData,stringSize);
        dbcfile->resize(dbcfile->pos());
    }
    dbcfile->close();
}


