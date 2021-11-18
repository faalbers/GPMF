#include "DVID.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::DVID::DVID(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'L' )
        throw std::runtime_error("DVID klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        throw std::runtime_error("DVID klv has more then value: " + std::to_string(dataRepeat));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("DVID klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);
    
    fileStream.read((char *) &inTime, sizeof(inTime));
    inTime = _byteswap_ulong(inTime);
    
    fileStream.close();
}

GPMF::DVID::DVID(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'L' )
        error_("TSMP klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        error_("TSMP klv has more then value: " + std::to_string(dataRepeat));

    inTime = _byteswap_ulong(*((uint32_t *) dataString.c_str()));
}

void GPMF::DVID::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : device ID" << std::endl;
    std::cout << dataIndent << inTime << std::endl;
}

void GPMF::DVID::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::DVID::key = "DVID";

