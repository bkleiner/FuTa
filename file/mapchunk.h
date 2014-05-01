#ifndef MAPCHUNK_H
#define MAPCHUNK_H

#include <memory>

#include "MapHeaders.h"
#include "file.h"
#include "filebuffer.h"
#include "misc.h"

class Alphamap
{
public:
    typedef std::shared_ptr<Alphamap> Ptr;

    Alphamap();

    void read(ENTRY_MCLY mcly, File &file, size_t size);
    void write(FileBuffer &buffer);

    uint8_t value(size_t offset) { return data[offset]; }
    void setValue(size_t offset, uint8_t value) { data[offset] = value; }

private:
    std::vector<uint8_t> data;
};


class MapChunk
{
public:
    typedef std::shared_ptr<MapChunk> Ptr;

    MapChunk();

    void read(ENTRY_MCIN mcin, File &file);
    FileBuffer write();
    Alphamap::Ptr getAlphamap(size_t layer);

private:
    MCNK mcnk;
    std::vector<uint8_t> mcvt;
    std::vector<uint8_t> mclv;
    std::vector<uint8_t> mccv;
    std::vector<uint8_t> mcnr;
    std::vector<ENTRY_MCLY> mcly;
    std::vector<uint8_t> mcrf;
    std::vector<uint8_t> mcsh;
    std::vector<Alphamap::Ptr> mcal;
    std::vector<uint8_t> mclq;
    std::vector<uint8_t> mcse;
};

#endif // MAPCHUNK_H
