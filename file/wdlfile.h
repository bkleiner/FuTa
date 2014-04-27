#ifndef WDLFILE_H
#define WDLFILE_H

#include "wowfile.h"
#include "wowfiles/Wdl.h"
#include "adtfile.h"

class WdlFile : public WoWFile
{
public:
    WdlFile();
    WdlFile(QString fileName, bool createNew = false);

    QList< wowpair > getAvailableWidgets();
    void setHeight(int offset, AdtFile* adt);
    void clearHeight();
    int getBaseOffset();

    void init();
    void save();

private:
    Wdl *theWdl;
    MAOF maofHeader;
};

#endif // WDLFILE_H
