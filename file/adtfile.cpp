#include "adtfile.h"
#include "utilities/Utilities.h"
#include "wowfiles/lichking/AdtLk.h"
#include "wowfiles/cataclysm/AdtCata.h"
#include <QString>
#include <QLabel>
#include "widgets/alphamapwidget.h"
#include "widgets/layerwidget.h"
#include "widgets/heightwidget.h"
#include "widgets/waterwidget.h"

AdtFile::AdtFile(QString fileName) : WoWFile(fileName)
{

}

AdtFileLk::AdtFileLk(QString fileName) : AdtFile(fileName)
{
    filename = fileName;
    QRegExp rx("_(\\d+)_(\\d+).adt");
    rx.indexIn(filename);
    specs.mapX = rx.cap(1).toInt();
    specs.mapY = rx.cap(2).toInt();
    specs.mBigAlpha = false;

    std::vector<char> adtFile(0);
    Utilities::getWholeFile(filename.toStdString(), adtFile);
    theAdt = new AdtLk(adtFile,filename.toStdString());
    this->init();
}

AdtFileLk::AdtFileLk(QString fileName, QString textureString, int mapx, int mapy, int areaID) : AdtFile(fileName)
{
    specs.mapX = mapx;
    specs.mapY = mapy;
    specs.mBigAlpha = false;

#ifdef Q_OS_WIN
    filename.replace("/","\\");
#else
    filename.replace("\\","/");
#endif

    McnkHeader headerStruct;
    std::vector<McnkLk> McnkVec;

    memset(&headerStruct,0,sizeof(headerStruct));

    //    ENTRY_MCLY mclyStruct;
    //    mclyData.effectID = 0;
    //    mclyData.flags = 0;
    //    mclyData.ofsAlpha = 0;
    //    mclyData.textureID = 0;
    //    std::vector<char> mclyData((char*)&mclyData, (char*)&mclyData + sizeof(mclyData));

    headerStruct.flags = 5;
    headerStruct.areaId = areaID;
    headerStruct.nLayers = 1;
    headerStruct.posZ = 1.0f;
    headerStruct.mcalOffset = 1732;
    headerStruct.mcvtOffset= 136;
    headerStruct.mclyOffset = 1180;
    headerStruct.mcnrOffset = 724;
    headerStruct.mcrfOffset = 1204;
    headerStruct.mcshOffset = 1212;
    headerStruct.mcseOffset = 1740;
    headerStruct.mclqSize = 8;
    headerStruct.mcshSize = 512;
    headerStruct.mcalSize = 8;

    std::vector<char> mcnrData(448);

    for(int i = 0; i < 145; ++i)
        mcnrData.at(3*i+2) = 127;


    Chunk mcvt = Chunk("TVCM",580,std::vector<char>(580));
    McnrLk mcnr = McnrLk("RNCM",448,mcnrData);
    Chunk mcly = Chunk("YLCM",16,std::vector<char>(16));
    Chunk mcsh = Chunk("HSCM",512,std::vector<char>(512));

    float startPosX = 17066.66656f - mapx * 533.33333f;
    float startPosY = 17066.66656f - mapy * 533.33333f;

    for(int y = 0; y < 16; ++y)
    {
        for(int x = 0; x < 16; ++x)
        {
            headerStruct.indexX = x;
            headerStruct.indexY = y;
            headerStruct.posY = startPosY - x*33.33333f;
            headerStruct.posX = startPosX - y*33.33333f;;
            McnkVec.push_back(McnkLk(headerStruct,mcvt,Chunk(),mcnr,mcly,Chunk("FRCM",0,std::vector<char>(0)),mcsh,Mcal("LACM",0,std::vector<char>(0)),Chunk(),Chunk("ESCM",0,std::vector<char>(0))));
        }
    }
    std::string textureName = textureString.toStdString();
    std::vector<char> xtemData(textureName.begin(),textureName.end());
    xtemData.insert(xtemData.end(),0);


    theAdt = new  AdtLk(filename.toStdString()
                        ,Chunk("REVM",4,Utilities::getCharVectorFromInt(18))
                        ,64
                        ,Mh2o()
                        ,Chunk("XETM",35,xtemData)
                        ,Chunk("XDMM",0,std::vector<char>(0))
                        ,Chunk("DIMM",0,std::vector<char>(0))
                        ,Chunk("OMWM",0,std::vector<char>(0))
                        ,Chunk("DIWM",0,std::vector<char>(0))
                        ,Chunk("FDDM",0,std::vector<char>(0))
                        ,Chunk("FDOM",0,std::vector<char>(0))
                        ,McnkVec
                        ,Chunk()
                        ,Chunk()
                        );
    this->init();
}

void AdtFileLk::save()
{
    for(int i = 0; i < 256; ++i)
    {
        nTextures = theAdt->mcnks.at(i).mcly.getGivenSize() / 16U;
        for (size_t t=0; t<nTextures; ++t) {
            memcpy(&theAdt->mcnks.at(i).mcly.data[t*sizeof(getMclyEntrys(i)->at(t))], &getMclyEntrys(i)->at(t), sizeof(getMclyEntrys(i)->at(t)));
        }
    }

    theAdt->updateOrCreateMhdrAndMcin();
    theAdt->toFile();
}

void AdtFileLk::exportHeight(std::string fileName)
{
    std::ofstream outputFile(fileName.c_str(), std::ios::out|std::ios::binary);
    if (outputFile.is_open())
        for(int i = 0; i < 256; ++i)
        {
            outputFile.write((char *)&getMcnkHeader(i)->posZ, sizeof(float));
            theAdt->mcnks.at(i).mcvt.toFile(outputFile);
        }

    outputFile.close();
}

void AdtFileLk::importHeight(std::string fileName)
{
    std::ifstream inputFile(fileName.c_str(), std::ios::binary);

    int offset = 0;
    static const int chunkLettersAndSize = 8;

    if(inputFile.is_open())
        for(int i = 0; i < 256; ++i)
        {
            this->getMcnkHeader(i)->posZ = Utilities::get<float>(Utilities::getCharVectorFromFile(inputFile,offset,sizeof(float)),0);
            offset = offset + sizeof(float);
            theAdt->mcnks.at(i).mcvt = Chunk(inputFile,offset);
            offset = offset + sizeof(float)*145 + chunkLettersAndSize;
        }

    inputFile.close();
}

QList<WoWFile::wowpair> AdtFileLk::getAvailableWidgets()
{
    functions << QPair< QString,WoWFileWidget* >("Alphamap", new alphamapWidget(this));
    functions << QPair< QString,WoWFileWidget* >("Layer Settings", new LayerWidget(this));
    functions << QPair< QString,WoWFileWidget* >("Heightmap", new HeightWidget(this));
    functions << QPair< QString,WoWFileWidget* >("Water", new WaterWidget(this));
    return functions;
}

void AdtFileLk::init()
{
    for(int i = 0; i < 256; ++i)
    {
        nTextures = theAdt->mcnks.at(i).mcly.getGivenSize() / 16U;
        mclyEntrys.push_back(std::vector< ENTRY_MCLY >());
        for (size_t t=0; t<nTextures; ++t)
        {
            getMclyEntrys(i)->push_back(Utilities::get<ENTRY_MCLY>(theAdt->mcnks.at(i).mcly.data, t*16));
        }
    }
}

void AdtFileLk::getValNoLOD(NoLoDArray *NoLoD, LoDArray *LoD, int at)
{
    McnkLk *mcnk = &theAdt->mcnks.at(at);
    int offset = 0;

    int indexLoD = 0;
    int indexNoLoD = 0;

    for (int j=0; j < 17; ++j)
    {
        for (int i=0; i < ((j % 2) ? 8 : 9); ++i)
        {
            if((j % 2))
                (*LoD)[indexLoD][i] = Utilities::get<float>(mcnk->mcvt.data,offset);
            else
                (*NoLoD)[indexNoLoD][i] = Utilities::get<float>(mcnk->mcvt.data,offset);

            offset += sizeof(float);
        }
        if((j % 2))
            indexLoD++;
        else
            indexNoLoD++;
    }
}

MCNR AdtFileLk::getMcnrStruct(int at)
{
    return Utilities::get<MCNR>(theAdt->mcnks.at(at).mcnr.data,0);
}

McnkHeader *AdtFileLk::getMcnkHeader(int at)
{
    return &theAdt->mcnks.at(at).mcnkHeader;
}

std::vector<ENTRY_MCLY> *AdtFileLk::getMclyEntrys(int at)
{
    return &mclyEntrys.at(at);
}

std::string AdtFileLk::getTextures()
{
    return Utilities::getStringFromCharVector(theAdt->mtex.data,0,theAdt->mtex.data.size());
}

void AdtFileLk::setTextures(std::string textures)
{
    theAdt->mtex.data.clear();
    theAdt->mtex.data.insert(theAdt->mtex.data.end(),textures.begin(),textures.end());
    theAdt->mtex.givenSize = theAdt->mtex.data.size();
}

void AdtFileLk::getAlphamap(unsigned char *amap, int x, int y, int layer)
{
    McnkLk *mcnk = &theAdt->mcnks.at(y*16+x);
    McnkHeader *mcnkHeader = getMcnkHeader(y*16+x);
    std::vector<ENTRY_MCLY> *mclyEntry = getMclyEntrys(y*16+x);


    memset(amap,0,64*64);

    if(layer < mcnkHeader->nLayers)
    {
        if(!specs.mBigAlpha)
            specs.mBigAlpha = (mclyEntry->size() > layer+1 ? (mclyEntry->at(layer+1).ofsAlpha - mclyEntry->at(layer).ofsAlpha) : (mcnk->mcal.data.size() - mclyEntry->at(layer).ofsAlpha)) == 64*64;

        if( mclyEntry->at(layer).flags & 0x100 )
        {
            if( mclyEntry->at(layer).flags & 0x200 )
            {  // compressed

                // 21-10-2008 by Flow
                unsigned offI = 0; //offset IN buffer
                unsigned offO = 0; //offset OUT buffer
                char* buffIn = &mcnk->mcal.data[mclyEntry->at(layer).ofsAlpha]; // pointer to data in adt file

                while( offO < 4096 )
                {
                    // fill or copy mode
                    bool fill = buffIn[offI] & 0x80;
                    unsigned n = buffIn[offI] & 0x7F;
                    offI++;
                    for( unsigned k = 0; k < n; ++k )
                    {
                        if (offO == 4096) break;
                        amap[offO] = buffIn[offI];
                        offO++;
                        if( !fill )
                            offI++;
                    }
                    if( fill ) offI++;
                }
            }
            else if(specs.mBigAlpha){
                // not compressed
                unsigned char *p = amap;
                char *abuf = &mcnk->mcal.data[mclyEntry->at(layer).ofsAlpha];
                for (int j=0; j<64; ++j) {
                    for (int i=0; i<64; ++i) {
                        *p++ = *abuf++;
                    }

                }

                memcpy(amap+63*64,amap+62*64,64);
            }
            else
            {

                // not compressed
                char *abuf = &mcnk->mcal.data[mclyEntry->at(layer).ofsAlpha];
                unsigned char *p = amap;
                for (int j=0; j<64; ++j) {
                    for (int i=0; i<32; ++i) {
                        unsigned char c = *abuf++;
                        *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0x0f)))/0x0f);
                        if(i != 31)
                            *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0xf0)))/0xf0);

                        else
                            *p++ = static_cast<unsigned char>((255*(static_cast<int>(c & 0x0f)))/0x0f);
                    }

                }
                memcpy(amap+63*64,amap+62*64,64);
            }

        }else
            std::cout << "layer with no tex flag" << std::endl;
    }

}

void AdtFileLk::setAlphamap(unsigned char *data, int x, int y, int layer)
{
    McnkLk *mcnk = &theAdt->mcnks.at(y*16+x);
    int lDimensions = 64 * ( specs.mBigAlpha ? 64 : 32 );
    McnkHeader *mcnkHeader = getMcnkHeader(y*16+x);
    std::vector<ENTRY_MCLY> *mclyEntry = getMclyEntrys(y*16+x);
    int num = 1;

    if(layer >= mcnkHeader->nLayers)
    {
        if(strlen((const char*)data) == NULL)
            return;

        num = layer+1 - mcnkHeader->nLayers;

        ENTRY_MCLY newMclyEntry;
        newMclyEntry.flags = 0;
        newMclyEntry.textureID = 0;
        newMclyEntry.effectID = 0;
        newMclyEntry.ofsAlpha = 0;

        for(int i = 0; i < num; ++i)
            mclyEntry->push_back(newMclyEntry);
        std::cout << "adding "<<num<< " layers at " << x <<"x"<<y<<" mcnk num: "<<y*16+x<<std::endl;

        mcnk->mcly.data.insert(mcnk->mcly.data.end(),16U*num,0);
        mcnk->mcly.givenSize = mcnk->mcly.givenSize + 16U*num;

        mcnkHeader->nLayers = layer+1;
        mcnkHeader->mcalOffset = mcnkHeader->mcalOffset+16U*num;
        mcnkHeader->mclqOffset = mcnkHeader->mclqOffset ? mcnkHeader->mclqOffset+16U*num : 0;
        mcnkHeader->mcshOffset = mcnkHeader->mcshOffset  ? mcnkHeader->mcshOffset != mcnkHeader->mcalOffset ? mcnkHeader->mcshOffset+16U*num : 0 : 0;
        mcnkHeader->mcseOffset = mcnkHeader->mcseOffset ? mcnkHeader->mcseOffset+16U*num : 0;
        mcnkHeader->mcrfOffset = mcnkHeader->mcrfOffset+16U*num;
        mcnk->givenSize = mcnk->givenSize + 16U*num;

        nTextures = mcnk->mcly.getGivenSize() / 16U;

    }
    if( !(mclyEntry->at(layer).flags & 0x100) )
    {
        mclyEntry->at(layer).flags = 0x100;
        mclyEntry->at(layer).ofsAlpha = mcnk->mcal.getRealSize();

        mcnkHeader->mcalSize = mcnkHeader->mcalSize + lDimensions;
        mcnkHeader->mclqOffset = mcnkHeader->mclqOffset ? mcnkHeader->mclqOffset+lDimensions : 0;
        mcnkHeader->mcseOffset = mcnkHeader->mcseOffset ? mcnkHeader->mcseOffset+lDimensions : 0;

        mcnk->mcal.givenSize = mcnkHeader->mcalSize - 8;
        mcnk->givenSize = mcnk->givenSize + lDimensions;

        mcnk->mcal.data.insert(mcnk->mcal.data.begin()+mclyEntry->at(layer).ofsAlpha,lDimensions,0);
    }
    if(mclyEntry->at(layer).flags & 0x200)
    {
        std::cout << "changin layer flags to 0x100 at " << x <<"x"<<y<<" mcnk num: "<<y*16+x<<std::endl;

        mclyEntry->at(layer).flags = 0x100;
        int oldSize = mcnk->mcal.data.size()-mclyEntry->at(layer).ofsAlpha;

        if(mclyEntry->size() > layer+1)
        {
            oldSize = mclyEntry->at(layer+1).ofsAlpha-mclyEntry->at(layer).ofsAlpha;
            for(int i = 1; i < mclyEntry->size()-layer; ++i)
                mclyEntry->at(layer+i).ofsAlpha = (mclyEntry->at(layer+i).ofsAlpha - oldSize) + lDimensions;
        }

        mcnk->mcal.data.erase(mcnk->mcal.data.begin()+mclyEntry->at(layer).ofsAlpha ,mcnk->mcal.data.begin()+mclyEntry->at(layer).ofsAlpha+oldSize); //
        mcnk->mcal.data.reserve(mcnk->mcal.data.size()+lDimensions);
        mcnk->mcal.data.insert(mcnk->mcal.data.begin()+mclyEntry->at(layer).ofsAlpha,lDimensions,0);

        //mclyEntry->at(layer).ofsAlpha = mclyEntry->at(layer-1).ofsAlpha+(layer-1 == 0 ? 0 : lDimensions);
        mcnkHeader->mcalSize = mcnk->mcal.data.size()+8;
        mcnkHeader->mclqOffset = mcnkHeader->mclqOffset ? (mcnkHeader->mclqOffset - oldSize) + lDimensions : 0;
        mcnkHeader->mcseOffset = mcnkHeader->mcseOffset ? (mcnkHeader->mcseOffset - oldSize) + lDimensions : 0;

        mcnk->mcal.givenSize = mcnk->mcal.data.size();
        mcnk->givenSize = (mcnk->givenSize-oldSize) + lDimensions;
    }

    if( specs.mBigAlpha )
    {
        for( int k = 0; k < lDimensions; k++ )
            mcnk->mcal.data.at(mclyEntry->at(layer).ofsAlpha+k) = data[k];
    }
    else
    {
        unsigned char upperNibble, lowerNibble;
        for( int k = 0; k < lDimensions; k++ )
        {
            lowerNibble = static_cast<unsigned char>(std::max(std::min( ( static_cast<float>(data[k * 2 + 0]) ) * 0.05882f + 0.5f , 15.0f),0.0f));
            upperNibble = static_cast<unsigned char>(std::max(std::min( ( static_cast<float>(data[k * 2 + 1]) ) * 0.05882f + 0.5f , 15.0f),0.0f));
            mcnk->mcal.data[mclyEntry->at(layer).ofsAlpha+k] = ( upperNibble << 4 ) + lowerNibble;
        }
    }

}
