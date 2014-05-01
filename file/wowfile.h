#ifndef WOWFILE_H
#define WOWFILE_H

#include <QString>
#include <QPair>
#include <stdexcept>

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

    WoWFile(QString fileName);

    virtual void read() { throw std::runtime_error("read() not implemented"); }
    virtual void save() { throw std::runtime_error("save() not implemented"); }

    int getAvailableWidgetsCount() { return functions.count(); }
    virtual QList< wowpair > getAvailableWidgets() { return functions; }

    QString getFilename() { return filename; }

protected:
    QList< wowpair > functions;
    QString filename;
};

#endif // WOWFILE_H
