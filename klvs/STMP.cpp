#include "STMP.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::STMP::STMP(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'J' )
        error_("STMP klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        error_("STMP klv has more then value: " + std::to_string(dataRepeat));

    timeStamp = _byteswap_uint64(*((uint64_t *) dataString.c_str()));
}

void GPMF::STMP::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : timestamp in µs" << std::endl;
    std::cout << dataIndent << timeStamp << std::endl;
}

void GPMF::STMP::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::STMP::key = "STMP";

