#ifndef ADTFILE_H
#define ADTFILE_H

#include <iostream>
#include <memory>

#include "wowfile.h"
#include "widgets/alphamapwidget.h"
#include "mapchunk.h"

class MapTile : public WoWFile
{
public:
    MapTile(QString fileName);

    void read();
    void save();

    Alphamap::Ptr getAlphamap(unsigned int x, unsigned int y, size_t layer);

    QList<WoWFile::wowpair> getAvailableWidgets()
    {
        functions << QPair< QString,WoWFileWidget* >("Alphamap", new alphamapWidget(this));
        return functions;
    }

private:
    MVER mver;
    MHDR mhdr;
    MCIN mcin;
    std::list<std::string> mtex;
    std::vector<uint8_t> mmdx;
    std::vector<uint8_t> mmid;
    std::vector<uint8_t> mwmo;
    std::vector<uint8_t> mwid;
    std::vector<uint8_t> mddf;
    std::vector<uint8_t> modf;
    std::vector<uint8_t> mh2o;
    std::vector<MapChunk::Ptr> chunks;
    std::vector<uint8_t> mfbo;
    std::vector<uint8_t> mtfx;
};



#endif // ADTFILE_H
