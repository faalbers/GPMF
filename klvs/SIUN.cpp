#include "SIUN.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::SIUN::SIUN(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'c' )
        throw std::runtime_error("SIUN klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("SIUN klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);
    
    char dataBlock[200];
    for ( int index = 0 ; index < dataRepeat; index++ ) {
        fileStream.read((char *) dataBlock, sizeof(dataBlock));
        units.push_back(std::string(dataBlock).substr(0, sampleSize));
    }

    fileStream.close();
}

GPMF::SIUN::SIUN(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'c' )
        error_("SIUN klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    std::string unit;
    size_t stringOffset = 0;
    for ( int index = 0 ; index < dataRepeat; index++ ) {
        std::string unit = dataString.substr(stringOffset, sampleSize);
        stringOffset += (size_t) sampleSize;
        units.push_back(unit);
    }
}

void GPMF::SIUN::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : standard units" << std::endl;
    int index = 1;
    std::cout << dataIndent << "[#] (unit)\n";
    for ( auto unit : units ) {
        std::cout << dataIndent << "[" << index << "] ( '" << unit << "' )" << std::endl;
        index++;
    }
}

void GPMF::SIUN::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::SIUN::key = "SIUN";

