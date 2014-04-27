#ifndef WOWFILEITEM_H
#define WOWFILEITEM_H

#include "wowfile.h"
#include <QPair>
#include <QVariant>

class WoWFileItem
{
public:

    WoWFileItem(QString name, WoWFileItem *parentItem = 0);
    WoWFileItem(WoWFile *WoWfile, WoWFileItem *parentItem);
    WoWFileItem(WoWFile::wowpair function, WoWFile *WoWfile, WoWFileItem *parentItem);

    void addChild(WoWFileItem *child) { childList.append(child); }
    WoWFileItem *getParent() { return parent; }
    WoWFileItem *getChild(int num) { return childList.at(num); }
    int getChildCount() { return childList.count(); }
    QVariant getData() { return data.first; }
    WoWFile *getWoWFile() { return wowfile; }
    WoWFileWidget *getWidget() { return data.second; }

    int row()
    {
        if (parent)
            return parent->childList.indexOf(const_cast<WoWFileItem*>(this));

        return 0;
    }

private:
    WoWFile *wowfile;
    WoWFileItem *parent;
    QList< WoWFileItem *> childList;
    QPair < QString,WoWFileWidget* > data;

};

#endif // WOWFILEITEM_H
