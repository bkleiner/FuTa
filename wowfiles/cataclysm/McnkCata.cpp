#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <wowfiles/Chunk.h>
#include <wowfiles/Mcnk.h>
#include <wowfiles/cataclysm/McnkCata.h>
#include <wowfiles/cataclysm/McnrCata.h>
#include <wowfiles/lichking/McnkLk.h>
#include <utilities/Utilities.h>

McnkCata::McnkCata(const std::vector<char> & adtFile, int offsetInFile, const int & headerSize) : Mcnk(adtFile, offsetInFile, 128)
{
  const int headerStartOffset (offsetInFile + chunkLettersAndSize);
  const int absoluteMcnkEnd = offsetInFile + chunkLettersAndSize + givenSize;

  offsetInFile = chunkLettersAndSize + offsetInFile;

  mcnkHeader = *reinterpret_cast<McnkHeader*>(&data[0]);

  offsetInFile = headerStartOffset + mcnkTerrainHeaderSize;
  
  int chunkName;

  while (offsetInFile < absoluteMcnkEnd)
  {
    chunkName = Utilities::get<int>(adtFile, offsetInFile);

    switch (chunkName)
    {
      case 'MCVT' :
        mcvt = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcvt.getGivenSize();
        break;  

      case 'MCCV' :
        mccv = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mccv.getGivenSize();
        break;  		

      case 'MCLV' :
        mclv = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mclv.getGivenSize();
        break;  

      case 'MCNR' :
        mcnr = McnrCata(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcnr.getGivenSize();
        break;  		

      case 'MCLQ' :
        mclq = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mclq.getGivenSize();
        break;  
		
      case 'MCSE' :
        mcse = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcse.getGivenSize();
        break; 

      case 'MCBB' :
        mcbb = Chunk(adtFile, offsetInFile);
        offsetInFile = offsetInFile + chunkLettersAndSize + mcbb.getGivenSize();
        break;  	
		
      default :
        terrainMcnkUnknown.push_back(Chunk(adtFile, offsetInFile));
        offsetInFile = offsetInFile + chunkLettersAndSize + terrainMcnkUnknown.back().getGivenSize();
    }
  }		
}

std::vector<char> McnkCata::getWholeChunk() const
{
  std::vector<char> wholeChunk (0);

  std::vector<char> tempData (letters.begin(), letters.end());
  wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());

  tempData = Utilities::getCharVectorFromInt(givenSize);
  wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());

  wholeChunk.insert(wholeChunk.end(), data.begin(), data.end());
  
  tempData = mcvt.getWholeChunk();
  wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());

  if (!mccv.isEmpty())
  {
    tempData = mccv.getWholeChunk();
    wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());
  }

  if (!mclv.isEmpty())
  {
    tempData = mccv.getWholeChunk();
    wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());
  }

  tempData = mcnr.getWholeChunk();
  wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());

  if (!mclq.isEmpty())
  {
    tempData = mclq.getWholeChunk();
    wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());
  }

  if (!mcse.isEmpty())
  {
    tempData = mcse.getWholeChunk();
    wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());
  }  

  if (!mcbb.isEmpty())
  {
    tempData = mcbb.getWholeChunk();
    wholeChunk.insert(wholeChunk.end(), tempData.begin(), tempData.end());
  }  

  return wholeChunk;
}

void McnkCata::toFile()
{
  // TODO
}

McnkLk McnkCata::toMcnkLk() // TODO : probably shouldn't be here.
{
  std::vector<char> emptyData (0); 
  Chunk (emptyChunk); 

  McnrLk cMcnr(mcnr.toMcnrLk());

  Chunk (cMccv);
  if (!mccv.isEmpty())
    cMccv = mccv;

  Chunk cMcly ("YLCM", 0, emptyData);
  Chunk cMcrf ("FRCM", 0, emptyData);
  Chunk cMcsh ("HSCM", 0, emptyData);
  Mcal (cMcal);

  McnkLk mcnkLk = McnkLk(mcnkHeader, mcvt, cMccv, cMcnr, cMcly, cMcrf, cMcsh, cMcal, emptyChunk, emptyChunk);
  return mcnkLk;
}

std::ostream & operator<<(std::ostream & os, const McnkCata & mcnkCata)
{
  os << "Chunk letters : " << mcnkCata.letters << std::endl;
  os << "Chunk givenSize : " << mcnkCata.givenSize << std::endl;

  os << "------------------------------" << std::endl;

  os << mcnkCata.mcvt;
  os << mcnkCata.mccv;
  os << mcnkCata.mclv;
  os << mcnkCata.mcnr;
  os << mcnkCata.mclq;
  os << mcnkCata.mcse;
  os << mcnkCata.mcbb;

  os << "Hi, in here we're unknown !" << std::endl;
  os << std::endl;

  std::vector<Chunk>::const_iterator unknownIter;

  for (unknownIter = mcnkCata.terrainMcnkUnknown.begin() ; unknownIter != mcnkCata.terrainMcnkUnknown.end() ; ++unknownIter)
  {
    os << *unknownIter;
  }
  
  os << "------------------------------" << std::endl;
  return os;
}
