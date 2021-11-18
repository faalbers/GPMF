#include "GPSF.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::GPSF::GPSF(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'L' )
        error_("GPSF klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        error_("GPSF klv has more then value: " + std::to_string(dataRepeat));

    gpsFix = _byteswap_ulong(*((uint32_t *) dataString.c_str()));
}

void GPMF::GPSF::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : gps fix (0 = no lock, 2 or 3 = 2D or 3D Lock)" << std::endl;
    std::cout << dataIndent << gpsFix << std::endl;
}

void GPMF::GPSF::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::GPSF::key = "GPSF";

