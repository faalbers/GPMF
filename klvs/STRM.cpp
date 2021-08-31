#include "STRM.hpp"
#include "../klvs.hpp"
#include <iostream>

GPMF::STRM::STRM(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
}

void GPMF::STRM::printData(bool fullLists)
{
    int levelCount = std::count(path_.begin(), path_.end(), '/');
    std::cout << path_ << " (<type> <size> <repeat>) : stream container" << std::endl;
}

void GPMF::STRM::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::STRM::key = "STRM";

