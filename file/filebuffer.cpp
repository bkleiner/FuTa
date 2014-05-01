#include "filebuffer.h"

FileBuffer::FileBuffer()
    : pos(0)
{}

FileBuffer::FileBuffer(std::vector<uint8_t> pData)
    : data(pData)
    , pos(0)
{}

void FileBuffer::reserve(size_t length)
{
    data.resize(length);
}

void FileBuffer::seek(size_t offset)
{
    pos = offset;
}

void FileBuffer::skip(size_t offset)
{
    pos += offset;
}

size_t FileBuffer::tell()
{
    return pos;
}

size_t FileBuffer::size()
{
    return data.size();
}

uint8_t *FileBuffer::ptr(size_t offset)
{
    return &data[offset];
}

void FileBuffer::write(std::vector<uint8_t> *buffer)
{
    data.insert(data.begin() + pos, buffer->begin(), buffer->end());
    pos += buffer->size();
}

void FileBuffer::write(FileBuffer *buffer)
{
    data.insert(data.begin() + pos, buffer->data.begin(), buffer->data.end());
    pos += buffer->size();
}

void FileBuffer::write(void *buffer, size_t length)
{
    data.insert(data.begin() + pos, (uint8_t*)buffer, (uint8_t*)(buffer) + length);
    pos += length;
}
