#ifndef GPMF_DATATYPES_H
#define GPMF_DATATYPES_H

#include <stdint.h>
#include <string>

// REMEMBER: they are 32 bit aligned !

namespace GPMF
{

// ********* DATA TYPES *********

/*
typedef struct sampleType
{
    uint32_t    ID;
    uint32_t    duration;
    uint32_t    time;
    uint32_t    currentTime;
    uint32_t    dataSize;
} sampleType;
*/

namespace datablock

{
// ********* DATA RETRIEVAL BLOCKS *********


typedef struct klvHeaderBlock
{
    char        key[4];
    uint8_t    	type;
    uint8_t		sampleSize;
    uint16_t	repeat;
} klvHeaderBlock;

}

}

#endif