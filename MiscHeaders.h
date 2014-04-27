#ifndef MISCHEADERS_H
#define MISCHEADERS_H

#include <stdint.h>



struct BLSHeader {
    /*0x00*/	char magix[4];		// in reverse character order: "SVXG" in case of a vertex shader, "SPXG" in case of a fragment shader
    /*0x04*/	uint32_t version;		// Always 0x10003 - version 1.3 of format
    /*0x08*/	uint32_t permutationCount;
    /*0x0C*/
};

struct BLSEntryHeader {
    /*0x00*/	uint32_t flags0;		// seen: 0x3FE80 in pixel shaders; 0x1A0F in vertex shaders. there may be more ..
    /*0x04*/	uint32_t flags4;		// seen: 0x200 in pixel shaders; 0x3FEC1 in vertex shaders (there may be more ..)
    /*0x08*/	uint32_t unk8;		// Never seen anything in here.
    /*0x0C*/	uint32_t size;		// Tells you how large the block actually is.
};

#endif // MISCHEADERS_H
