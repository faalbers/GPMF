#ifndef GPMF_DATATYPES_H
#define GPMF_DATATYPES_H

#include <stdint.h>
#include <string>

// REMEMBER: they are 32 bit aligned !

namespace GPMF
{

// ********* DATA TYPES *********

typedef struct gpsType
{
    int32_t latitude;
    int32_t longitude;
    int32_t altitude;
    int32_t speed2D;
    int32_t speed3D;
} gpsType;

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