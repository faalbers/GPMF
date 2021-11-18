#include "MINF.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::MINF::MINF(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'c' )
        throw std::runtime_error("MINF klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("MINF klv can not parse file: "+filePath);
    char dataBlock[200];
    fileStream.seekg(fileDataPos_, fileStream.beg);
    fileStream.read((char *) dataBlock, sizeof(dataBlock));
    name = std::string(dataBlock).substr(0, sampleSize * dataRepeat);
    fileStream.close();
}

GPMF::MINF::MINF(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'c' )
        error_("MINF klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    name = dataString.substr(0, sampleSize * dataRepeat);
}

void GPMF::MINF::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : model info" << std::endl;
    std::cout << dataIndent << getZeroTerminatedString(name) << std::endl;
}

void GPMF::MINF::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::MINF::key = "MINF";

