#include "DEVC.hpp"
#include "../klvs.hpp"
#include <iostream>

GPMF::DEVC::DEVC(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
}

void GPMF::DEVC::printData(bool fullLists)
{
    int levelCount = std::count(path_.begin(), path_.end(), '/');
    std::cout << path_ << " (<type> <size> <repeat>) : main container" << std::endl;
}

void GPMF::DEVC::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::DEVC::key = "DEVC";

