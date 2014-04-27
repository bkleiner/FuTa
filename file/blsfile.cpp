#include "blsfile.h"

BLSFile::BLSFile(QString &fileName, char *dataIn, int & sizeIn) : WoWFile(fileName,dataIn,sizeIn)
{
    memcpy(&header,data,sizeof(header));

    size_t offset(0x0C);

    BLSEntryHeader entry;

    for(uint i = 0; i < header.permutationCount; ++i)
    {
        memcpy(&entry,data+offset,0x10);
        offset += 0x10;

        QString text = QString::fromLocal8Bit(data+offset);
        offset += text.size()+1;

        entrys.append(BLSEntry(entry,text));

        if(entrys.at(i).first.flags0 ==  2560)
            offset += 3;
        else if(entrys.at(i).first.flags0 == 7680)
            offset += 2;
//        else
            //offset += 3;
    }
}

void BLSFile::toPlainFile(QString path)
{
    QString savepath = path+filename.mid(filename.lastIndexOf("/")+1);
    for(uint i = 0; i < header.permutationCount; ++i)
    {
        QString filepath(savepath);
        filepath.insert(filepath.lastIndexOf("."),QString("_plain_%1").arg(i));
        QFile file(filepath);

        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
            return;

         QTextStream out(&file);
         out << entrys.at(i).second;

         file.close();
    }
}

void BLSFile::readEntry(size_t offset)
{
    BLSEntryHeader entry;
    memcpy(&entry,data+offset,4*sizeof(uint32_t));

    QString text = QString::fromLocal8Bit(data+offset+0x10);

    entrys.append(BLSEntry(entry,text));
}
