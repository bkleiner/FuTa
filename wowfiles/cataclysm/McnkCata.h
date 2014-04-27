#ifndef _WOWFILES_CATACLYSM_MCNKCATA_H_
#define _WOWFILES_CATACLYSM_MCNKCATA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/lichking/McnkLk.h>
#include <wowfiles/cataclysm/McnrCata.h>

class McnkCata : public Mcnk
{
  public:

    McnkCata(const std::vector<char> & adtFile, int offsetInFile, const int & headerSize);
    McnkCata(std::string letters, int givenSize, const std::vector<char> &data);

    std::vector<char> getWholeChunk() const;
	  void toFile();
    McnkLk toMcnkLk();

    friend std::ostream & operator<<(std::ostream & os, const McnkCata & mcnkCata);

  private:

    McnkHeader mcnkHeader;
    Chunk mcvt;
    Chunk mccv;
    Chunk mclv;
    McnrCata mcnr;
    Chunk mclq;
    Chunk mcse;
    Chunk mcbb;
	  std::vector<Chunk> terrainMcnkUnknown;
};

#endif