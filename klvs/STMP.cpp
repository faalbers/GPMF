#include "STMP.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::STMP::STMP(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'J' )
        throw std::runtime_error("STMP klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        throw std::runtime_error("STMP klv has more then value: " + std::to_string(dataRepeat));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("STMP klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);
    
    fileStream.read((char *) &timeStamp, sizeof(timeStamp));
    timeStamp = _byteswap_uint64(timeStamp);
    
    fileStream.close();
}

void GPMF::STMP::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : timestamp in µs" << std::endl;
    std::cout << dataIndent << timeStamp << std::endl;
}

void GPMF::STMP::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::STMP::key = "STMP";

