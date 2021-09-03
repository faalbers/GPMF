#include "TMPC.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::TMPC::TMPC(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'f' )
        throw std::runtime_error("TMPC klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        throw std::runtime_error("TMPC klv has more then value: " + std::to_string(dataRepeat));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("TMPC klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);
    
    fileStream.read((char *) &temperature, sizeof(temperature));
    temperature = _byteswap_ulong(temperature);
    
    fileStream.close();
}

void GPMF::TMPC::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : sensor temperature" << std::endl;
    std::cout << dataIndent << temperature << std::endl;
}

void GPMF::TMPC::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::TMPC::key = "TMPC";

