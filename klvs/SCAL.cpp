#include "SCAL.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::SCAL::SCAL(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 's' )
        throw std::runtime_error("SCAL klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( sampleSize != 2 )
        throw std::runtime_error("SCAL klv wrong sample size: " + std::to_string(sampleSize));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("SCAL klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);
    fileStream.read((char *) &denominator, sizeof(denominator));
    denominator = _byteswap_ushort(denominator);
    fileStream.close();
}

void GPMF::SCAL::printData(bool fullLists)
{
    int levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : scale denominator" << std::endl;
    std::cout << dataIndent << denominator << std::endl;
}

void GPMF::SCAL::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::SCAL::key = "SCAL";

