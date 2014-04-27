#ifndef ADTFILE_H
#define ADTFILE_H

#include "wowfile.h"
#include "wowfiles/lichking/AdtLk.h"
#include "wowfiles/cataclysm/AdtCata.h"
#include "wowfiles/ChunkHeaders.h"
#include "MapHeaders.h"

class AdtFile : public WoWFile
{
public:
    AdtFile(QString fileName);

    struct ADTSpecs
    {
        bool mBigAlpha;
        int mapX;
        int mapY;
    };

    ADTSpecs specs;

    virtual void getAlphamap(unsigned char *amap,int x, int y, int layer) { std::cout << "not declarted in template" << std::endl; }
    virtual void setAlphamap(unsigned char *data, int x, int y, int layer) { std::cout << "not declarted in template" << std::endl; }

    virtual McnkHeader *getMcnkHeader(int at) { std::cout << "not declarted in template" << std::endl; }
    virtual std::vector<ENTRY_MCLY> *getMclyEntrys(int at) { std::cout << "not declarted in template" << std::endl; }

    virtual void setTextures(std::string textures) { std::cout << "not declarted in template" << std::endl; }
    virtual std::string getTextures() { std::cout << "not declarted in template" << std::endl; }

    virtual void getValNoLOD(NoLoDArray *NoLoD, LoDArray *LoD, int at) { std::cout << "not declarted in template" << std::endl; }

    virtual MCNR getMcnrStruct(int at) { std::cout << "not declarted in template" << std::endl; }

    virtual void importWater(std::string fileName) { std::cout << "not declarted in template" << std::endl; }
    virtual void exportWater(std::string fileName) { std::cout << "not declarted in template" << std::endl; }

    virtual void importHeight(std::string fileName) { std::cout << "not declarted in template" << std::endl; }
    virtual void exportHeight(std::string fileName) { std::cout << "not declarted in template" << std::endl; }

    virtual QList<WoWFile::wowpair> getAvailableWidgets() { std::cout << "not declarted in template" << std::endl; }

    ADTSpecs *getSpecs() { return &specs; }

private:


};

class AdtFileLk : public AdtFile
{

public:
    void save();
    void exportHeight(std::string fileName);
    void importHeight(std::string fileName);

    QList<WoWFile::wowpair> getAvailableWidgets();


    void getValNoLOD(NoLoDArray *NoLoD, LoDArray *LoD, int at);

    MCNR getMcnrStruct(int at);
    McnkHeader *getMcnkHeader(int at);
    std::vector<ENTRY_MCLY> *getMclyEntrys(int at);
    std::string getTextures();

    void setTextures(std::string textures);
    void getAlphamap(unsigned char *amap, int x, int y, int layer);
    void setAlphamap(unsigned char *data, int x, int y, int layer);

private:
    AdtFileLk(QString fileName);
    AdtFileLk(QString fileName, QString textureString, int mapx, int mapy, int areaID);

    void init();

    AdtLk *theAdt;
    MHDR mhdrHeader;
    MCNR mcnrStruct;
    std::vector< std::vector< ENTRY_MCLY > > mclyEntrys;
    size_t nTextures;

    friend class WoWFileManager;
};

#endif // ADTFILE_H
