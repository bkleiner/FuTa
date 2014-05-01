#include "maptile.h"

#include "../file/filebuffer.h"

MapTile::MapTile(QString fileName)
    : WoWFile(fileName)
{}


void MapTile::read()
{
    File file(filename.toStdString(), File::read);
    ChunkHeader chunkHeader;

    // MVER
    readChunkHeader(file, &chunkHeader, "MVER");
    file.read_into(&mver, sizeof(MVER));

    // MHDR
    readChunkHeader(file, &chunkHeader, "MHDR");
    file.read_into(&mhdr, sizeof(MHDR));

    // MCIN
    file.seek(0x14 + mhdr.mcin);
    readChunkHeader(file, &chunkHeader, "MCIN");
    file.read_into(&mcin, sizeof(MCIN));

    // MTEX
    file.seek(0x14 + mhdr.mtex);
    readChunkHeader(file, &chunkHeader, "MTEX");
    {
        size_t offset = 0;
        std::vector<uint8_t> data;
        file.read_into(&data, chunkHeader.size);

        while(offset < data.size())
        {
            std::string filename((char *)(data.data() + offset));
            mtex.push_back(filename);

            offset += filename.size() + 1;
        }

        if(mtex.empty())
            mtex.push_back("tileset\\generic\\black.blp");
    }

    //MMDX
    file.seek(0x14 + mhdr.mmdx);
    readChunkHeader(file, &chunkHeader, "MMDX");
    file.read_into(&mmdx, chunkHeader.size);

    //MMID
    file.seek(0x14 + mhdr.mmid);
    readChunkHeader(file, &chunkHeader, "MMID");
    file.read_into(&mmid, chunkHeader.size);

    //MWMO
    file.seek(0x14 + mhdr.mwmo);
    readChunkHeader(file, &chunkHeader, "MWMO");
    file.read_into(&mwmo, chunkHeader.size);

    //MWID
    file.seek(0x14 + mhdr.mwid);
    readChunkHeader(file, &chunkHeader, "MWID");
    file.read_into(&mwid, chunkHeader.size);

    //MDDF
    file.seek(0x14 + mhdr.mddf);
    readChunkHeader(file, &chunkHeader, "MDDF");
    file.read_into(&mddf, chunkHeader.size);

    //MODF
    file.seek(0x14 + mhdr.modf);
    readChunkHeader(file, &chunkHeader, "MODF");
    file.read_into(&modf, chunkHeader.size);

    //MH2O
    if(mhdr.mh2o)
    {
        file.seek(0x14 + mhdr.mh2o);
        readChunkHeader(file, &chunkHeader, "MH2O");
        file.read_into(&mh2o, chunkHeader.size);
    }

    //chunks
    for (size_t i = 0; i < 256; ++i)
    {
        MapChunk::Ptr entry = std::make_shared<MapChunk>();
        entry->read(mcin.mEntries[i], file);

        chunks.push_back(entry);
    }

    //MFBO
    if((mhdr.flags & 1) && mhdr.mfbo)
    {
        file.seek(0x14 + mhdr.mfbo);
        readChunkHeader(file, &chunkHeader, "MFBO");
        file.read_into(&mfbo, chunkHeader.size);
    }

    //MTFX
    if(mhdr.mtfx)
    {
        file.seek(0x14 + mhdr.mtfx);
        readChunkHeader(file, &chunkHeader, "MTFX");
        file.read_into(&mtfx, chunkHeader.size);
    }
}

void MapTile::save()
{
    FileBuffer buffer;

    // MVER
    writeChunkHeader(buffer, "MVER", sizeof(MVER));
    buffer.write(&mver, sizeof(MVER));

    // MHDR
    writeChunkHeader(buffer, "MHDR", sizeof(MHDR));
    FileBuffer::DataPtr<MHDR> mhdrPtr = buffer.ptr<MHDR>();
    buffer.write(&mhdr, sizeof(MHDR));

    // MCIN
    mhdrPtr->mcin = buffer.tell() - 0x14;
    writeChunkHeader(buffer, "MCIN", sizeof(MCIN));
    FileBuffer::DataPtr<ENTRY_MCIN> mcinPtr = buffer.ptr<ENTRY_MCIN>();
    buffer.write(&mhdr, sizeof(MCIN));


    // MTEX
    mhdrPtr->mtex = buffer.tell() - 0x14;
    {
        FileBuffer texBuffer;

        for(auto it = mtex.begin(); it != mtex.end(); ++it)
        {
            texBuffer.write((char*)it->c_str(), it->size() + 1);
        }

        writeChunkHeader(buffer, "MTEX", texBuffer.size());
        buffer.write(&texBuffer);
    }

    //MMDX
    mhdrPtr->mmdx = buffer.tell() - 0x14;
    writeChunkHeader(buffer, "MMDX", mmdx.size());
    buffer.write(&mmdx);

    //MMID
    mhdrPtr->mmid = buffer.tell() - 0x14;
    writeChunkHeader(buffer, "MMID", mmid.size());
    buffer.write(&mmid);

    //MWMO
    mhdrPtr->mwmo = buffer.tell() - 0x14;
    writeChunkHeader(buffer, "MWMO", mwmo.size());
    buffer.write(&mwmo);

    //MWID
    mhdrPtr->mwid = buffer.tell() - 0x14;
    writeChunkHeader(buffer, "MWID", mwid.size());
    buffer.write(&mwid);

    //MDDF
    mhdrPtr->mddf = buffer.tell() - 0x14;
    writeChunkHeader(buffer, "MDDF", mddf.size());
    buffer.write(&mddf);

    //MODF
    mhdrPtr->modf = buffer.tell() - 0x14;
    writeChunkHeader(buffer, "MODF", modf.size());
    buffer.write(&modf);

    //MH2O
    if(mh2o.size())
    {
        mhdrPtr->mh2o = buffer.tell() - 0x14;
        writeChunkHeader(buffer, "MH2O", mh2o.size());
        buffer.write(&mh2o);
    }
    else
    {
        mhdrPtr->mh2o = 0;
    }

    //chunks
    for (auto it = chunks.begin(); it != chunks.end(); ++it)
    {
        FileBuffer chunk = (*it)->write();

        mcinPtr->offset = buffer.tell();
        mcinPtr->size = chunk.size();
        mcinPtr->asyncID = 32;
        mcinPtr->flags = 32;

        buffer.write(&chunk);
        mcinPtr++;
    }

    //MFBO
    if(mfbo.size())
    {
        mhdrPtr->mfbo = buffer.tell() - 0x14;
        mhdrPtr->flags |= 1;
        writeChunkHeader(buffer, "MFBO", mfbo.size());
        buffer.write(&mfbo);
    }
    else
    {
        mhdrPtr->mfbo = 0;
        mhdrPtr->flags &= ~1;
    }

    //MTFX
    if(mtfx.size())
    {
        mhdrPtr->mtfx = buffer.tell() - 0x14;
        writeChunkHeader(buffer, "MTFX", mtfx.size());
        buffer.write(&mtfx);
    }
    else
    {
        mhdrPtr->mtfx = 0;
    }

    File file(filename.toStdString(), File::write);
    file.write_from(&buffer);
}

Alphamap::Ptr MapTile::getAlphamap(unsigned int x, unsigned int y, size_t layer)
{
    return chunks[y * 16 + x]->getAlphamap(layer);
}
