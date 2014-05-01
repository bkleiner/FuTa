#include "mapchunk.h"

#include <cstring>



Alphamap::Alphamap()
    : data(64 * 64)
{}

void Alphamap::read(ENTRY_MCLY mcly, File &file, size_t size)
{
    file.skip(mcly.ofsAlpha);

    if(mcly.flags & 0x200)
    {
        // 21-10-2008 by Flow
        unsigned offI = 0; //offset IN buffer
        unsigned offO = 0; //offset OUT buffer
        std::vector<uint8_t> buffIn(size); // pointer to data in adt file
        file.read_into(&buffIn);

        while( offO < 4096 )
        {
            // fill or copy mode
            bool fill = buffIn[offI] & 0x80;
            unsigned n = buffIn[offI] & 0x7F;
            offI++;
            for( unsigned k = 0; k < n; ++k )
            {
                if (offO == 4096) break;
                data[offO] = buffIn[offI];
                offO++;
                if( !fill )
                    offI++;
            }
            if( fill ) offI++;
        }
    }
    else if(size == 4096)
    {
        file.read_into(&data);
    }
    else if(size == 2048)
    {
        std::vector<uint8_t> buffIn(size); // pointer to data in adt file
        file.read_into(&buffIn);

        uint8_t *abuf = buffIn.data();
        uint8_t *p = data.data();

        for (int j=0; j < 64; ++j)
        {
            for (int i=0; i < 32; ++i)
            {
                unsigned char c = *abuf++;

                uint8_t lower = (c & 0x0F);
                uint8_t upper = i == 31 ? ((c & 0xF0) >> 4) : lower;

                *p++ = (uint8_t)((lower / 15.0f) * 255.0f);
                *p++ = (uint8_t)((upper / 15.0f) * 255.0f);
            }
        }
        memcpy(data.data() + 63 * 64, data.data() + 62 * 64, 64);
    }

}

void Alphamap::write(FileBuffer &buffer)
{
    const size_t size = 64 * 32;
    std::vector<uint8_t> buf(size);

    for(size_t k = 0; k < size; k++ )
    {
        const size_t index = k * 2;

        uint8_t val1 = (uint8_t)((data[index] / 255.0f) * 0xF);
        uint8_t val2 = (uint8_t)((data[index + 1] / 255.0f) * 0xF);

        buf[k] = (val2 << 4) | val1;
    }

    buffer.write(&buf);
}


MapChunk::MapChunk()
{}

void MapChunk::read(ENTRY_MCIN mcin, File &file)
{
    ChunkHeader chunkHeader;

    //MCNK
    file.seek(mcin.offset);
    readChunkHeader(file, &chunkHeader, "MCNK");
    file.read_into(&mcnk, sizeof(MCNK));

    //MCVT
    file.seek(mcin.offset + mcnk.mcvt);
    readChunkHeader(file, &chunkHeader, "MCVT");
    file.read_into(&mcvt, chunkHeader.size);

    //MCLV
    if(mcnk.mclv)
    {
        file.seek(mcin.offset + mcnk.mclv);
        readChunkHeader(file, &chunkHeader, "MCLV");
        file.read_into(&mclv, chunkHeader.size);
    }

    //MCCV
    if(mcnk.mccv)
    {
        file.seek(mcin.offset + mcnk.mccv);
        readChunkHeader(file, &chunkHeader, "MCCV");
        file.read_into(&mccv, chunkHeader.size);
    }

    //MCNR
    file.seek(mcin.offset + mcnk.mcnr);
    readChunkHeader(file, &chunkHeader, "MCNR");
    file.read_into(&mcnr, 0x1C0/*chunkHeader.size*/);

    //MCLY
    file.seek(mcin.offset + mcnk.mcly);
    readChunkHeader(file, &chunkHeader, "MCLY");
    mcly.resize(chunkHeader.size / sizeof(ENTRY_MCLY));
    file.read_into(mcly.data(), chunkHeader.size);

    //MCRF
    file.seek(mcin.offset + mcnk.mcrf);
    readChunkHeader(file, &chunkHeader, "MCRF");
    file.read_into(&mcrf, chunkHeader.size);

    //MCSH
    file.seek(mcin.offset + mcnk.mcsh);
    readChunkHeader(file, &chunkHeader, "MCSH");
    file.read_into(&mcsh, chunkHeader.size);

    //MCAL
    file.seek(mcin.offset + mcnk.mcal);
    readChunkHeader(file, &chunkHeader, "MCAL");
    for(size_t i = 1; i < mcly.size(); ++i)
    {
        if(!(mcly[i].flags & 0x100))
            throw std::runtime_error("there was supposed to be a alpha layer");

        const size_t nextLayerSize = (i + 1 < mcly.size() ? mcly[i + 1].ofsAlpha : (mcnk.sizeMcal - sizeof(ChunkHeader)));
        const size_t size = nextLayerSize - mcly[i].ofsAlpha;
        Alphamap::Ptr amap = std::make_shared<Alphamap>();

        file.seek(mcin.offset + mcnk.mcal + sizeof(ChunkHeader));
        amap->read(mcly[i], file, size);

        mcal.push_back(amap);
    }

    //MCLQ
    if(mcnk.mclq)
    {
        file.seek(mcin.offset + mcnk.mclq);
        readChunkHeader(file, &chunkHeader, "MCLQ");
        file.read_into(&mclq, chunkHeader.size);
    }

    //MCSE
    if(mcnk.mcse)
    {
        file.seek(mcin.offset + mcnk.mcse);
        readChunkHeader(file, &chunkHeader, "MCSE");
        file.read_into(&mcse, chunkHeader.size);
    }
}

FileBuffer MapChunk::write()
{
    FileBuffer buffer;

    //MCNK
    FileBuffer::DataPtr<ChunkHeader> mcnkHeader = buffer.ptr<ChunkHeader>();
    writeChunkHeader(buffer, "MCNK", buffer.size());
    FileBuffer::DataPtr<MCNK> mcnkPtr = buffer.ptr<MCNK>();
    buffer.write(&mcnk, sizeof(MCNK));

    //MCVT
    mcnkPtr->mcvt = buffer.tell();
    writeChunkHeader(buffer, "MCVT", mcvt.size());
    buffer.write(&mcvt);

    //MCLV
    if(mclv.size())
    {
        mcnkPtr->mclv = buffer.tell();
        writeChunkHeader(buffer, "MCLV", mclv.size());
        buffer.write(&mclv);
    }
    else
    {
        mcnkPtr->mclv = 0;
    }

    //MCCV
    if(mccv.size())
    {
        mcnkPtr->mccv = buffer.tell();
        writeChunkHeader(buffer, "MCCV", mccv.size());
        buffer.write(&mccv);
    }
    else
    {
        mcnkPtr->mccv = 0;
    }

    //MCNR
    mcnkPtr->mcnr = buffer.tell();
    writeChunkHeader(buffer, "MCNR", mcnr.size());
    buffer.write(&mcnr);

    //MCRF
    mcnkPtr->mcrf = buffer.tell();
    writeChunkHeader(buffer, "MCRF", mcrf.size());
    buffer.write(&mcrf);

    //MCSH
    mcnkPtr->mcsh = buffer.tell();
    writeChunkHeader(buffer, "MCSH", mcsh.size());
    buffer.write(&mcsh);

    //MCLY
    mcnkPtr->mcly = buffer.tell();
    writeChunkHeader(buffer, "MCLY", (mcal.size() + 1) * sizeof(ENTRY_MCLY));
    FileBuffer::DataPtr<ENTRY_MCLY> mclyPtr = buffer.ptr<ENTRY_MCLY>();
    for(size_t i = 0; i < mcal.size() + 1; ++i)
    {
        ENTRY_MCLY entry;

        if(i < mcly.size())
            entry = mcly[i];
        else
        {
            entry.textureID = 0;
            entry.effectID = 0;
            entry.flags = 0;
        }

        if(i > 0) entry.flags |= 0x100;
        entry.flags &= ~0x200;

        buffer.write(&entry, sizeof(ENTRY_MCLY));
    }


    //MCAL
    {
        FileBuffer alphaBuffer;
        size_t offset = 0;
        mclyPtr++;

        for(auto it = mcal.begin(); it != mcal.end(); ++it)
        {
            (*it)->write(alphaBuffer);
            mclyPtr->ofsAlpha = offset;

            offset += 2048;
            mclyPtr++;
        }

        mcnkPtr->mcal = buffer.tell();
        mcnkPtr->sizeMcal = alphaBuffer.size() + sizeof(ChunkHeader);
        mcnkPtr->nLayers = mcal.size() + 1;

        writeChunkHeader(buffer, "MCAL", alphaBuffer.size());
        buffer.write(&alphaBuffer);
    }

    //MCLQ
    if(mclq.size())
    {
        mcnkPtr->mclq = buffer.tell();
        writeChunkHeader(buffer, "MCLQ", mclq.size());
        buffer.write(&mclq);
    }
    else
    {
        mcnkPtr->mclq = 0;
    }

    //MCSE
    if(mcse.size())
    {
        mcnkPtr->mcse = buffer.tell();
        writeChunkHeader(buffer, "MCSE", mcse.size());
        buffer.write(&mcse);
    }
    else
    {
        mcnkPtr->mcse = 0;
    }

    mcnkHeader->size = buffer.size();

    return buffer;
}

Alphamap::Ptr MapChunk::getAlphamap(size_t layer)
{
    if(layer < mcal.size())
        return mcal[layer];

    if(layer > 3)
        throw std::runtime_error("there can never be more than 3 alphamaps");

    Alphamap::Ptr amap = std::make_shared<Alphamap>();
    mcal.push_back(amap);
    return amap;
}
