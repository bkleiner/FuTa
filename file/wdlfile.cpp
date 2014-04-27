#include "wdlfile.h"
#include "widgets/lowresheightwidget.h"

WdlFile::WdlFile(QString fileName, bool createNew) : WoWFile(fileName)
{
    if(!createNew)
    {
        std::vector<char> wdlFile(0);
        Utilities::getWholeFile(filename.toStdString(), wdlFile);
        theWdl = new Wdl(wdlFile,filename.toStdString());
    }
    else
        //theWdl = new Wdl(filename.toStdString(),Chunk("MWMO",0,0),Chunk("mwmid",0,0),Chunk("MODF",0,0),Chunk("MAOF",0,0),std::vector<Chunk>());
        init();
}

void WdlFile::init()
{
    maofHeader = Utilities::get<MAOF>(theWdl->maof.data);
}

QList< WoWFile::wowpair > WdlFile::getAvailableWidgets()
{
    functions << QPair< QString,WoWFileWidget* >("Low Res Height", new lowResHeightWidget(this));
    return functions;
}

int WdlFile::getBaseOffset()
{
    return theWdl->mver.getRealSize() + theWdl->mwmo.getRealSize() + theWdl->mwid.getRealSize() + theWdl->modf.getRealSize() + theWdl->maof.getRealSize()+ 5 * WowChunkedFormat::chunkLettersAndSize;
}

void WdlFile::clearHeight()
{
    memset(&maofHeader,0,sizeof(MAOF));
    theWdl->mareAndMaho.clear();
}

void WdlFile::save()
{
    memcpy(&theWdl->maof.data[0],&maofHeader,sizeof(MAOF));
    theWdl->toFile();
}

void WdlFile::setHeight(int offset, AdtFile *adt)
{
    maofHeader.mareOffsets[adt->getSpecs()->mapY][adt->getSpecs()->mapX] = getBaseOffset() + offset;

    AdtFile::NoLoDArray NoLoD;    //outer
    AdtFile::LoDArray LoD;        //inner

    AdtFile::LowNoLoDArray LowNoLoD;    //outer
    AdtFile::LowLoDArray LowLoD;        //inner



    memset(&LowNoLoD,0,sizeof(LowNoLoD));
    memset(&LowLoD,0,sizeof(LowLoD));

    for(int x=0; x<16; ++x)
        for(int y=0; y<16; ++y)
        {
            adt->getValNoLOD(&NoLoD,&LoD,y*16+x);
            int baseY = adt->getMcnkHeader(y*16+x)->posZ;

            LowNoLoD[y][x] = (LowNoLoD[y][x] + (baseY+(NoLoD[0][0] + NoLoD[0][1] + LoD[0][0] + NoLoD[1][0] + NoLoD[1][1])/5) )/2;
            LowNoLoD[y+1][x] = (LowNoLoD[y+1][x] + (baseY+(NoLoD[7][0] + NoLoD[7][1] + LoD[7][0] + NoLoD[8][0] + NoLoD[8][1])/5) )/2;
            LowNoLoD[y][x+1] = (LowNoLoD[y][x+1] + (baseY+(NoLoD[0][7] + NoLoD[0][8] + LoD[0][7] + NoLoD[1][7] + NoLoD[1][8])/5) )/2;
            LowNoLoD[y+1][x+1] = (LowNoLoD[y+1][x+1] + (baseY+(NoLoD[7][7] + NoLoD[7][8] + LoD[7][7] + NoLoD[8][7] + NoLoD[8][8])/5) )/2;
            LowLoD[y][x] = baseY+(NoLoD[3][3] + NoLoD[3][5] + NoLoD[4][4] + NoLoD[5][3] + NoLoD[5][5])/5;
        }
    std::vector<char> vector(1090);

    memcpy(&vector[0],LowNoLoD,sizeof(LowNoLoD));
    memcpy(&vector[sizeof(LowNoLoD)],LowLoD,sizeof(LowLoD));



    theWdl->mareAndMaho.push_back(Chunk("ERAM",1090,vector));
}

//    int indexLoD = 0;
//    int indexNoLoD = 0;
//    char data[1090];

//    for (int y=0; y < 33; ++y)
//    {
//        for (int x=0; x < ((y % 2) ? 16 : 17); ++x)
//        {
//            if((y % 2))
//            {

//                int sum = 0;
//                int count = 0;

//                for(int i = std::max(indexLoD - 1, 0); i < std::min(indexLoD + 2, 17); ++i)
//                    for(int j = std::max(x-1,0); j < std::min(x + 2, 17); ++j)
//                    {
//                        if((i % 2))
//                            sum += LowNoLoD[i][j];
//                        else
//                            sum += LowLoD[i][j];
//                        count++;
//                    }

//                LowLoD[indexLoD][x] = sum/count;
//            }
//            else
//            {

//                int sum = 0;
//                int count = 0;

//                for(int i = std::max(indexNoLoD-1, 0); i < std::min(indexNoLoD + 2, 17); ++i)
//                    for(int j = std::max(x-1,0); j < std::min(x + 2, 17); ++j)
//                    {
//                        if((i % 2))
//                            sum += LowLoD[i][j];
//                        else
//                            sum += LowNoLoD[i][j];
//                        count++;
//                    }

//                LowNoLoD[indexNoLoD][x] = sum/count;
//            }

//        }
//        if((y % 2))
//        {
//            indexLoD++;
//        }
//        else
//        {
//            indexNoLoD++;
//        }
//    }

