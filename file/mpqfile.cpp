#include "mpqfile.h"
#include <iostream>
#include <StormLib.h>


MPQFile::MPQFile(QString fileName) : WoWFile(fileName)
{
    if(!SFileOpenArchive(fileName.toStdString().c_str(),NULL,NULL,&mpqHandle))
        std::cout << "Error opening MPQ: " << GetLastError() << std::endl;

    this->reloadListfile();
}

bool MPQFile::hasFile(QString path)
{
    return mpqlistfile.contains(path,Qt::CaseInsensitive);
}

void MPQFile::reloadListfile()
{
    HANDLE listfile;
    if(SFileOpenFileEx(mpqHandle,LISTFILE_NAME,0,&listfile))
    {
        const size_t filesize (SFileGetFileSize (listfile));

        char* readbuffer (new char[filesize]);

        SFileReadFile (listfile, readbuffer, filesize);
        SFileCloseFile (listfile);
        mpqlistfile = QString::fromAscii (readbuffer, filesize).toLower().split ("\r\n", QString::SkipEmptyParts);

        delete[] readbuffer;
    }
}

bool MPQFile::openFile(QString &fileName, int *size, char **buffer)
{
    if(!hasFile(fileName))
        return false;

    HANDLE fileHandle;

    if (SFileOpenFileEx (mpqHandle, qPrintable (fileName), NULL, &fileHandle))
    {
      *size = SFileGetFileSize (fileHandle);
      *buffer = new char[*size];

      SFileReadFile (fileHandle, *buffer, *size);
      SFileCloseFile (fileHandle);
      return true;
    }
}
