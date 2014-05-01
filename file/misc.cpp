#include "misc.h"

void readChunkHeader(File &file, ChunkHeader *header, std::string magic)
{
    std::string reverse(magic);
    std::reverse(reverse.begin(), reverse.end());

    file.read_into(header, sizeof(ChunkHeader));
    if(strncmp(header->magic, reverse.c_str(), 4))
        throw std::runtime_error("Magic assert failed: " + magic);
}


void writeChunkHeader(FileBuffer &buffer, std::string magic, uint32_t size)
{
    std::reverse(magic.begin(), magic.end());
    buffer.write((char*)magic.data(), magic.size());
    buffer.write(&size, sizeof(uint32_t));
}
