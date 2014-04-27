#ifndef WOWFILE_H
#define WOWFILE_H

#include <QString>
#include <QPair>
#include "widgets/wowfilewidget.h"

class WoWFile
{
public:

    enum Version
    {
        ALPHA = 0,
        VANNILLA = 1,
        WOTLK = 3,
        CATA = 4
    };

    typedef QPair<QString, WoWFileWidget *> wowpair;
    typedef float NoLoDArray[9][9];
    typedef float LoDArray[8][8];

    typedef int16_t LowNoLoDArray[17][17];
    typedef int16_t LowLoDArray[16][16];

    WoWFile(QString fileName, char *dataIn = 0, int sizeIn = 0);


    virtual QList< wowpair > getAvailableWidgets() { return functions; }
    virtual void save() {}

    int getAvailableWidgetsCount() { return functions.count(); }
    QString getFilename() { return filename; }

    char* data;

protected:
    virtual void init() {}

    QList< wowpair > functions;
    QString filename;
    int size;


};

#endif // WOWFILE_H
