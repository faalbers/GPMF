#include "WBAL.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::WBAL::WBAL(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'S' )
        throw std::runtime_error("WBAL klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("WBAL klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);

    uint16_t whiteBalance;
    for ( int index = 0 ; index < dataRepeat; index++ ) {
        fileStream.read((char *) &whiteBalance, sizeof(whiteBalance));
        whiteBalance = _byteswap_ushort(whiteBalance);
        whiteBalances.push_back(whiteBalance);
    }

    fileStream.close();
}

void GPMF::WBAL::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : White Balance temperature (Kelvin)" << std::endl;
    int index = 1;
    std::cout << dataIndent << "[#] (white balance)\n";
    for ( auto whiteBalance : whiteBalances ) {
        std::cout << dataIndent << "[" << index << "] ( " << whiteBalance << " )" << std::endl;
        index++;
    }
}

void GPMF::WBAL::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::WBAL::key = "WBAL";
