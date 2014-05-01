#ifndef MISC_H
#define MISC_H

#include <cstring>
#include <algorithm>

#include "file.h"
#include "filebuffer.h"
#include "MapHeaders.h"

void readChunkHeader(File &file, ChunkHeader *header, std::string magic);
void writeChunkHeader(FileBuffer &buffer, std::string magic, uint32_t size);

#endif // MISC_H
