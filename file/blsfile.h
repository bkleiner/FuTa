#ifndef BLSFILE_H
#define BLSFILE_H

#include "wowfile.h"
#include "MiscHeaders.h"

#include <QFile>
#include <QTextStream>


class BLSFile : public WoWFile
{
public:
    typedef QPair<BLSEntryHeader,QString> BLSEntry;

    BLSFile(QString & fileName, char *dataIn, int & sizeIn);

    void toPlainFile(QString path);

private:
    void readEntry(size_t offset);

    BLSHeader header;
    QList< BLSEntry > entrys;
};

#endif // BLSFILE_H
