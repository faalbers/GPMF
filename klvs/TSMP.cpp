#include "TSMP.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::TSMP::TSMP(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'L' )
        error_("TSMP klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        error_("TSMP klv has more then value: " + std::to_string(dataRepeat));

    totalSampleCount = _byteswap_ulong(*((uint32_t *) dataString.c_str()));
}

void GPMF::TSMP::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : total sample count" << std::endl;
    std::cout << dataIndent << totalSampleCount << std::endl;
}

void GPMF::TSMP::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::TSMP::key = "TSMP";

