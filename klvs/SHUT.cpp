#include "SHUT.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::SHUT::SHUT(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'f' )
        error_("SHUT klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    //inTime = _byteswap_ulong(*((uint32_t *) dataString.c_str()));
    size_t stringOffset = 0;
    uint32_t shutterSpeedInt;
    for ( int index = 0 ; index < dataRepeat; index++ ) {
        shutterSpeedInt = _byteswap_ulong(*((uint32_t *) dataString.substr(stringOffset).c_str() ));
        samples.push_back( *( (float *) &shutterSpeedInt ) );
        stringOffset += 4;
    }
}

void GPMF::SHUT::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : shutter speed" << std::endl;
    size_t index = 1;
    std::cout << dataIndent << "[#] (time)\n";
    if ( fullLists || (!fullLists && samples.size() <= 6) ) {
        for ( auto sample : samples ) {
            std::cout << dataIndent << "[" << index << "] ( "
                << sample << " )" << std::endl;
            index++;
        }
    } else {
        for ( index = 0 ; index < 3; index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << samples[index] << " )" << std::endl;
        }
        std::cout << dataIndent << "     ...\n";
        for ( index = samples.size()-3 ; index < samples.size(); index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << samples[index] << " )" << std::endl;
        }
    }
}

void GPMF::SHUT::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::SHUT::key = "SHUT";

