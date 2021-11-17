#include "GPSF.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::GPSF::GPSF(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'L' )
        throw std::runtime_error("GPSF klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        throw std::runtime_error("GPSF klv has more then value: " + std::to_string(dataRepeat));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("GPSF klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);
    
    fileStream.read((char *) &gpsFix, sizeof(gpsFix));
    gpsFix = _byteswap_ulong(gpsFix);
    
    fileStream.close();
}

GPMF::GPSF::GPSF(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
}

void GPMF::GPSF::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : total sample count" << std::endl;
    std::cout << dataIndent << gpsFix << std::endl;
}

void GPMF::GPSF::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::GPSF::key = "GPSF";

