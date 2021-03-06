#include "DEVC.hpp"
#include <iostream>

GPMF::DEVC::DEVC(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
}

std::vector<GPMF::STRM *> GPMF::DEVC::getStreams()
{
    return getTypeKlvs<STRM>();
}

void GPMF::DEVC::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::cout << path_ << " (<datatype> <datasize> <repeat>) : main container" << std::endl;
}

void GPMF::DEVC::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::DEVC::key = "DEVC";

