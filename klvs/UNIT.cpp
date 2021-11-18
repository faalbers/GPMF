#include "UNIT.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::UNIT::UNIT(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'c' )
        error_("UNIT klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    std::string unit;
    size_t stringOffset = 0;
    for ( int index = 0 ; index < dataRepeat; index++ ) {
        std::string unit = dataString.substr(stringOffset, sampleSize);
        stringOffset += (size_t) sampleSize;
        units.push_back(unit);
    }
}

void GPMF::UNIT::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : display units" << std::endl;
    int index = 1;
    std::cout << dataIndent << "[#] (unit)\n";
    for ( auto unit : units ) {
        std::cout << dataIndent << "[" << index << "] ( '" << getZeroTerminatedString(unit) << "' )" << std::endl;
        index++;
    }
}

void GPMF::UNIT::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::UNIT::key = "UNIT";

