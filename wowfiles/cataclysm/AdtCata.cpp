#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <wowfiles/cataclysm/AdtCata.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mhdr.h>
#include <wowfiles/Mh2o.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCata.h>
#include <wowfiles/cataclysm/McnrCata.h>
#include <utilities/Utilities.h>
#include <wowfiles/lichking/AdtLk.h>

AdtCata::AdtCata(const std::string & adtFileName, const std::vector<char> & adtFile) : adtName(adtFileName)
{
  const int fileSize = adtFile.size();
  int offsetInFile (0);

  int chunkName;

  while (offsetInFile < fileSize)
  {
    chunkName = Utilities::get<int>(adtFile, offsetInFile);

    switch (chunkName)
    {
      case 'MVER' :
        terrainMver = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + terrainMver.getGivenSize();
        break;

      case 'MHDR' :
        mhdr = Mhdr(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mhdr.getGivenSize();
        break;

      case 'MH2O' :
        mh2o = Mh2o(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mh2o.getGivenSize();
        break;

      case 'MCNK' :
        terrainMcnks.push_back(McnkCata(adtFile, offsetInFile, 128));
        offsetInFile = offsetInFile + chunkLettersAndSize + terrainMcnks.back().getGivenSize(); 
        break;

      case 'MBMH' :
        mbmh = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mbmh.getGivenSize();
        break;

      case 'MBMI' :
        mbmi = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mbmi.getGivenSize();
        break;

      case 'MBMV' :
        mbmv = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mbmv.getGivenSize();
        break;

      case 'MFBO' :
        mfbo = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mfbo.getGivenSize();
        break;

      default :
        terrainUnknown.push_back(Chunk(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + terrainUnknown.back().getGivenSize();
    }
  }
}

void AdtCata::toFile()
{
  std::string fileName (adtName);
  fileName.append("_new");

  toFile(fileName);
}

void AdtCata::toFile(const std::string & fileName)
{
  std::vector<char> wholeAdt(0);
  
  std::vector<char> tempData(terrainMver.getWholeChunk());
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  
  tempData = mhdr.getWholeChunk();
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());

  if (!mh2o.isEmpty())
  {
    tempData = mh2o.getWholeChunk();
    wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  }

  int currentMcnk;

  for (currentMcnk = 0 ; currentMcnk < 256 ; ++currentMcnk)
  {
    tempData =  terrainMcnks[currentMcnk].getWholeChunk();
    wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  }

  if (!mbmh.isEmpty())
  {
    tempData = mbmh.getWholeChunk();
    wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  }

  if (!mbmi.isEmpty())
  {
    tempData = mbmi.getWholeChunk();
    wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  }

  if (!mbmv.isEmpty())
  {
    tempData = mbmv.getWholeChunk();
    wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  }

  if (!mfbo.isEmpty())
  {
    tempData = mfbo.getWholeChunk();
    wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  }

  std::ofstream outputFile (fileName.c_str(), std::ios::out|std::ios::binary);

  if (outputFile.is_open())
    outputFile.write((char *)&wholeAdt[0], sizeof(char) * wholeAdt.size());

  outputFile.close();
}

AdtLk AdtCata::toAdtLk() // TODO : conversion is only partial, only terrain (other chunks are empty), and it's weird to have method in Cata terrain.
{
  std::string cName (adtName);

  std::vector<char> mverData;
  mverData.push_back(0x12);
  mverData.push_back(0x0);
  mverData.push_back(0x0);
  mverData.push_back(0x0);
  Chunk cMver ("REVM", 4, mverData);

  Mh2o (cMh2o);
  if (!mh2o.isEmpty())
    cMh2o = mh2o;
  
  std::vector<char> emptyData(0);

  Chunk cMtex ("XETM", 0, emptyData); // TODO : fill emptiness for objects
  Chunk cMmdx ("XDMM", 0, emptyData); 
  Chunk cMmid ("DIMM", 0, emptyData);
  Chunk cMwmo ("OMWM", 0, emptyData);
  Chunk cMwid ("DIWM", 0, emptyData);
  Chunk cMddf ("FDDM", 0, emptyData);
  Chunk cModf ("FDOM", 0, emptyData);
  
  std::vector<McnkLk> cMcnks;
  int currentMcnk;

  for (currentMcnk = 0 ; currentMcnk < 256 ; ++currentMcnk)
  {
    cMcnks.push_back(terrainMcnks[currentMcnk].toMcnkLk());
  }

  Chunk (cMfbo);
  Chunk (cMtxf);

  const int mhdrFlags = 0;

  AdtLk adtLk(cName, cMver, mhdrFlags, cMh2o, cMtex, cMmdx, cMmid, cMwmo, cMwid, cMddf, cModf, cMcnks, cMfbo, cMtxf);
  return adtLk;  
}

std::ostream & operator<<(std::ostream & os, const AdtCata & adtCata)
{
  os << adtCata.adtName << std::endl;
  os << "------------------------------" << std::endl;
  os << adtCata.terrainMver;
  os << adtCata.mhdr;
  os << adtCata.mh2o;

  std::vector<McnkCata>::const_iterator mcnksIter;
  int i (0);

  for (mcnksIter = adtCata.terrainMcnks.begin() ; mcnksIter != adtCata.terrainMcnks.end() ; ++mcnksIter)
  {
    os << "MCNK #" << i << " : " << std::endl;
    os << *mcnksIter;
    ++i;
  }

  os << adtCata.mbmh;
  os << adtCata.mbmi;
  os << adtCata.mbmv;
  os << adtCata.mfbo;

  std::vector<Chunk>::const_iterator terrainUnknownIter;

  for (terrainUnknownIter = adtCata.terrainUnknown.begin() ; terrainUnknownIter != adtCata.terrainUnknown.end() ; ++terrainUnknownIter)
  {
    os << *terrainUnknownIter;
  }  
    
  return os;
}
