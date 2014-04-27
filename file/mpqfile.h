#ifndef MPQFILE_H
#define MPQFILE_H

#include "wowfile.h"
#include "StormLib.h"
#include <QStringList>

class MPQFile : public WoWFile
{
public:
    MPQFile(QString fileName);

    void reloadListfile();
    bool openFile(QString &fileName, int* size, char** buffer);
    QStringList getListfile() { return mpqlistfile; }
    bool hasFile(QString path);

private:
    HANDLE mpqHandle;
    QStringList mpqlistfile;
};

#endif // MPQFILE_H
