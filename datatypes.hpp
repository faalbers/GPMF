#ifndef GPMF_DATATYPES_H
#define GPMF_DATATYPES_H

#include <stdint.h>
#include <string>
#include <vector>

// REMEMBER: they are 32 bit aligned !

namespace GPMF
{

// ********* DATA TYPES *********

typedef struct sampleEntryType
{
    float       value;
    std::string unit;
    std::string info;
} sampleEntryType;

typedef struct sampleType
{
    std::vector<sampleEntryType>    entries;
    sampleEntryType                 time;
    sampleEntryType                 duration;
} sampleType;

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