#ifndef FILEBUFFER_H
#define FILEBUFFER_H

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>
#include <iostream>

class FileBuffer : public std::enable_shared_from_this<FileBuffer>
{
public:
    typedef std::shared_ptr<FileBuffer> Ptr;

    template<class T>
    class DataPtr
    {
    public:
        DataPtr(FileBuffer* pBuffer, size_t pOffset)
            : buffer(pBuffer)
            , offset(pOffset)
        {}

        T* operator->() const
        {
            return (T*)(buffer->ptr(offset));
        }

        DataPtr<T>& operator++()
        {
            offset += sizeof(T);
            return *this;
        }

        DataPtr<T> operator++(int)
        {
            DataPtr<T> tmp(*this); //Kopier-Konstruktor
            ++(*this); //Inkrement
            return tmp; //alten Wert zurueckgeben
        }

    private:
        FileBuffer* buffer;
        size_t offset;
    };

    FileBuffer();
    FileBuffer(std::vector<uint8_t> pData);

    void reserve(size_t length);

    void seek(size_t offset);
    void skip(size_t offset);

    size_t tell();
    size_t size();

    void write(std::vector<uint8_t> *buffer);
    void write(FileBuffer *buffer);
    void write(void *buffer, size_t length);

    uint8_t *ptr(size_t offset = 0);

    template<class T>
    DataPtr<T> ptr(int offset = 0)
    {
        DataPtr<T> dataptr(this, pos + offset);
        return dataptr;
    }

private:
    size_t pos;
    std::vector<uint8_t> data;
};

#endif // FILEBUFFER_H
