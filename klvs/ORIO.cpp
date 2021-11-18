#include "ORIO.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::ORIO::ORIO(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'c' )
        error_("ORIO klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    name = dataString.substr(0, sampleSize * dataRepeat);
}

void GPMF::ORIO::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") :  recommended output orientation" << std::endl;
    std::cout << dataIndent << name << std::endl;
}

void GPMF::ORIO::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::ORIO::key = "ORIO";

