#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <wowfiles/cataclysm/AdtCataObjects.h>
#include <wowfiles/Chunk.h>
#include <wowfiles/cataclysm/McnkCataObjects.h>
#include <utilities/Utilities.h>

AdtCataObjects::AdtCataObjects(const std::string & adtName, const std::vector<char> & adtFile) : adtName(adtName)
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
        objectsMver = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + objectsMver.getGivenSize();
        break;

      case 'MMDX' :
        mmdx = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mmdx.getGivenSize();
        break;
		
      case 'MMID' :
        mmid = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mmid.getGivenSize();
        break;

      case 'MWMO' :
        mwmo = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mwmo.getGivenSize();
        break;

      case 'MWID' :
        mwid = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mwid.getGivenSize();
        break;

      case 'MDDF' :
        mddf = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mddf.getGivenSize();
        break;	 		
		
      case 'MODF' :
        modf = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + modf.getGivenSize();
        break;	  	

      case 'MCNK' :
        objectsMcnks.push_back(McnkCataObjects(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + objectsMcnks.back().getGivenSize();
        break;

      default :
        objectsUnknown.push_back(Chunk(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + objectsUnknown.back().getGivenSize();
    }
  }
}

void AdtCataObjects::toFile()
{
  std::string fileName (adtName);
  fileName.append("_new");

  toFile(fileName);
}

void AdtCataObjects::toFile(const std::string & fileName)
{
  std::vector<char> wholeAdt(0);
  
  std::vector<char> tempData(objectsMver.getWholeChunk());
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  
  tempData = mmdx.getWholeChunk();
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());

  tempData = mmid.getWholeChunk();
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());

  tempData = mwmo.getWholeChunk();
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());

  tempData = mwid.getWholeChunk();
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());

  tempData = mddf.getWholeChunk();
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());

  tempData = modf.getWholeChunk();
  wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());

  int currentMcnk;

  for (currentMcnk = 0 ; currentMcnk < 256 ; ++currentMcnk)
  {
    tempData =  objectsMcnks[currentMcnk].getWholeChunk();
    wholeAdt.insert(wholeAdt.end(), tempData.begin(), tempData.end());
  }

  std::ofstream outputFile (fileName.c_str(), std::ios::out|std::ios::binary);

  if (outputFile.is_open())
    outputFile.write((char *)&wholeAdt[0], sizeof(char) * wholeAdt.size());

  outputFile.close();
}

std::ostream & operator<<(std::ostream & os, const AdtCataObjects & adtCataObjects)
{
  os << "------------------------------" << std::endl;
  os << adtCataObjects.adtName << std::endl;
  os << "------------------------------" << std::endl;
  os << adtCataObjects.objectsMver;
  os << adtCataObjects.mmdx;
  os << adtCataObjects.mmid;
  os << adtCataObjects.mwmo;
  os << adtCataObjects.mwid;
  os << adtCataObjects.mddf;
  os << adtCataObjects.modf;

  std::vector<McnkCataObjects>::const_iterator mcnksObjectsIter;
  int i (0);
  
  for (mcnksObjectsIter = adtCataObjects.objectsMcnks.begin() ; mcnksObjectsIter != adtCataObjects.objectsMcnks.end() ; ++mcnksObjectsIter)
  {
    os << "MCNK (objects) #" << i << " : " << std::endl;
    os << *mcnksObjectsIter;
    ++i;
  }

  std::vector<Chunk>::const_iterator objectsUnknownIter;

  for (objectsUnknownIter = adtCataObjects.objectsUnknown.begin() ; objectsUnknownIter != adtCataObjects.objectsUnknown.end() ; ++objectsUnknownIter)
  {
    os << *objectsUnknownIter;
  }    
  
  return os;
}