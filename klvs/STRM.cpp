#include "STRM.hpp"
#include "../klvs.hpp"
#include <iostream>

GPMF::STRM::STRM(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
}

void GPMF::STRM::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::cout << path_ << " (<datatype> <datasize> <repeat>) : stream container" << std::endl;
}

void GPMF::STRM::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::STRM::key = "STRM";

