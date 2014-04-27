#include "wowfileitem.h"
#include <QtGlobal>

WoWFileItem::WoWFileItem(QString name, WoWFileItem *parentItem) :
   parent(parentItem)
{
    data.first = name;
}

WoWFileItem::WoWFileItem(WoWFile::wowpair function, WoWFile *WoWfile, WoWFileItem *parentItem) :
   parent(parentItem)
  ,data(function)
  ,wowfile(WoWfile)
{

}

WoWFileItem::WoWFileItem(WoWFile *WoWfile, WoWFileItem *parentItem) :
    parent(parentItem)
   ,wowfile(WoWfile)
{
    QString filename = wowfile->getFilename();
#ifdef Q_WS_WIN
    data.first = filename.mid(filename.lastIndexOf("\\")+1);
#else
    data.first = filename.mid(filename.lastIndexOf("/")+1);
#endif

    foreach(WoWFile::wowpair function, wowfile->getAvailableWidgets())
    {
        childList.append(new WoWFileItem(function,wowfile,this));
    }
}
