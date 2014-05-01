// MapHeaders.h is part of Noggit3, licensed via GNU General Publiicense (version 3).
// Bernd Lörwald <bloerwald+noggit@googlemail.com>
// Stephan Biegel <project.modcraft@googlemail.com>
// Tigurius <bstigurius@googlemail.com>

#ifndef MAPHEADERS
#define MAPHEADERS

#include <stdint.h>

enum eMCNKFlags
{
    FLAG_SHADOW = 0x1,
    FLAG_IMPASS  = 0x2,
    FLAG_LQ_RIVER  = 0x4,
    FLAG_LQ_OCEAN  = 0x8,
    FLAG_LQ_MAGMA  = 0x10,
    FLAG_LQ_SLIME  = 0x20,
    FLAG_MCCV = 0x40,
    FLAG_TBC = 0x8000
};

enum eMCLYFlags
{
    FLAG_ANIMATE_45 = 0x1,
    FLAG_ANIMATE_90 = 0x2,
    FLAG_ANIMATE_180 = 0x4,
    FLAG_ANIM_FAST = 0x8,
    FLAG_ANIM_FASTER = 0x10,
    FLAG_ANIM_FASTEST = 0x20,
    FLAG_ANIMATE = 0x40,
    FLAG_GLOW = 0x80,
    FLAG_USE_ALPHA = 0x100, //should be set for every layer except the first
    FLAG_ALPHA_COMPRESSED = 0x200, //we do not compress, so ignore this
    FLAG_REFLECTION = 0x400
};

static const float TILESIZE = 533.33333f;
static const float CHUNKSIZE = ((TILESIZE) / 16.0f);
static const float UNITSIZE = (CHUNKSIZE / 8.0f);
static const float MINICHUNKSIZE = (CHUNKSIZE / 4.0f);
static const float ZEROPOINT = (32.0f * (TILESIZE));

struct ChunkHeader
{
    /*0x0*/ 	char magic[4]; 	//Chunk identifier - in reverse character order
    /*0x4*/ 	uint32_t size; 		//Chunk size
};

struct MVER {
    uint32_t version;
};

struct MHDR
{
    /*000h*/  uint32_t flags;        // &1: MFBO, &2: unknown. in some Northrend ones.
    /*004h*/  uint32_t mcin;  //Positions of MCNK's
    /*008h*/  uint32_t mtex;  //List of all the textures used
    /*00Ch*/  uint32_t mmdx;  //List of all the md2's used
    /*010h*/  uint32_t mmid;  //Offsets into MMDX list for what each ID is
    /*014h*/  uint32_t mwmo;  //list of all the WMO's used
    /*018h*/  uint32_t mwid;  //Offsets into MWMO list for what each ID is
    /*01Ch*/  uint32_t mddf;  //Doodad Information
    /*020h*/  uint32_t modf;  //WMO Positioning Information
    /*024h*/  uint32_t mfbo;  // tbc, wotlk; only when flags&1
    /*028h*/  uint32_t mh2o;  // wotlk
    /*02Ch*/  uint32_t mtfx;  // wotlk
    /*030h*/  uint32_t pad4;
    /*034h*/  uint32_t pad5;
    /*038h*/  uint32_t pad6;
    /*03Ch*/  uint32_t pad7;
    /*040h*/
};

struct ENTRY_MCIN
{
    uint32_t  offset;
    uint32_t  size;
    uint32_t  flags; //runtime only
    uint32_t  asyncID; //runtime only
};

struct MCIN
{
    ENTRY_MCIN mEntries[256];
};

struct ENTRY_MDDF
{
    uint32_t  nameID;
    uint32_t  uniqueID;
    float  pos[3];
    float  rot[3];
    //uint16_t  flags;
    uint16_t  scale;
    uint16_t  flags;
};

struct ENTRY_MODF
{
    uint32_t  nameID;
    uint32_t  uniqueID;
    float  pos[3];
    float  rot[3];
    float  extents[2][3];
    //uint16_t  flags;
    uint16_t  flags;
    uint16_t  doodadSet;
    uint16_t  nameSet;
    uint16_t  unknown;
};

struct MCNK {
    uint32_t flags;
    uint32_t ix;
    uint32_t iy;
    uint32_t nLayers;
    uint32_t nDoodadRefs;
    uint32_t mcvt;
    uint32_t mcnr;
    uint32_t mcly;
    uint32_t mcrf;
    uint32_t mcal;
    uint32_t sizeMcal;
    uint32_t mcsh;
    uint32_t sizeMcsh;
    uint32_t areaid;
    uint32_t nMapObjRefs;
    uint32_t holes;
    uint16_t reallyLow[8];
    uint32_t predTex;
    uint32_t nEffectDoodad;
    uint32_t mcse;
    uint32_t nSndEmitters;
    uint32_t mclq;
    uint32_t sizeMclq;
    float  zpos;
    float  xpos;
    float  ypos;
    uint32_t mccv;
    uint32_t mclv;
    uint32_t unused2;
};

struct ENTRY_MCLY
{
    uint32_t  textureID;
    uint32_t  flags;
    uint32_t  ofsAlpha;
    uint32_t  effectID;
};

#endif
