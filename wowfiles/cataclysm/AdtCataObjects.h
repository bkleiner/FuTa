#ifndef _WOWFILES_CATACLYSM_ADTCATAOBJECTS_H_
#define _WOWFILES_CATACLYSM_ADTCATAOBJECTS_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/WowChunkedFormat.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCataObjects.h>

class AdtCataObjects : public WowChunkedFormat
{
  public:

    AdtCataObjects(const std::string & adtName, const std::vector<char> & adtFile);

    void toFile();
    void toFile(const std::string & fileName);

    friend std::ostream & operator<<(std::ostream & os, const AdtCataObjects & adtCataObjects);

  private:

    std::string adtName;
    Chunk objectsMver;
    Chunk mmdx;
    Chunk mmid;
    Chunk mwmo;
    Chunk mwid;
    Chunk mddf;
    Chunk modf;
    std::vector<McnkCataObjects> objectsMcnks;
	  std::vector<Chunk> objectsUnknown;
};

#endif