#include "TYPE.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::TYPE::TYPE(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'c' )
        error_("TYPE klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    type = dataString.substr(0, sampleSize * dataRepeat);
}

void GPMF::TYPE::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : stream type" << std::endl;
    std::cout << dataIndent << getZeroTerminatedString(type) << std::endl;
}

void GPMF::TYPE::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::TYPE::key = "TYPE";

