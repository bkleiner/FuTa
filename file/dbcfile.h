#ifndef DBCFILE_H
#define DBCFILE_H

#include <QFile>

#include "wowfile.h"

#include <iostream>


class DBCFile : public WoWFile
{
public:
    DBCFile(QString fileName);
    template<typename T> T get (int record, int field);
    std::string getString(int record, int field);
    std::string getLoc(int record, int firstField);

    template<typename T> void set (int record, int field, T value);
    template<typename T> void set (int record, int field, T value, int size);

    void save();

private:
    QFile *dbcfile;
    char *data;
    char *stringData;
    std::string letters;
    int records;
    int fields;
    int fieldSize;
    int recordSize;
    int stringSize;


};

#endif // DBCFILE_H
