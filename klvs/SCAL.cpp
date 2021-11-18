#include "SCAL.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::SCAL::SCAL(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( !(dataType == 's' || dataType == 'l') )
        throw std::runtime_error("SCAL klv wrong data type: "+std::string((char *)&dataType).substr(0,1));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("SCAL klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);

    if ( dataType == 's') {
        int16_t denominator;
        for ( int index = 0 ; index < dataRepeat; index++ ) {
            fileStream.read((char *) &denominator, sizeof(denominator));
            denominator = _byteswap_ushort(denominator);
            denominators.push_back((int32_t) denominator);
        }
    } else if ( dataType == 'l') {
        int32_t denominator;
        for ( int index = 0 ; index < dataRepeat; index++ ) {
            fileStream.read((char *) &denominator, sizeof(denominator));
            denominator = _byteswap_ulong(denominator);
            denominators.push_back((int32_t) denominator);
        }
    }
    fileStream.close();
}

GPMF::SCAL::SCAL(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( !(dataType == 's' || dataType == 'l') )
        error_("SCAL klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( dataRepeat != 1 )
        error_("SCAL klv has more then value: " + std::to_string(dataRepeat));

    //inTime = _byteswap_ulong(*((uint32_t *) dataString.c_str()));
    size_t stringOffset = 0;
    if ( dataType == 's') {
        int16_t denominator;
        for ( int index = 0 ; index < dataRepeat; index++ ) {
            denominator = _byteswap_ushort(*((uint16_t *) dataString.substr(stringOffset).c_str() ));
            denominators.push_back((int32_t) denominator);
            stringOffset += 2;
        }
    } else if ( dataType == 'l') {
        int32_t denominator;
        for ( int index = 0 ; index < dataRepeat; index++ ) {
            denominator = _byteswap_ulong(denominator);
            denominator = _byteswap_ulong(*((uint32_t *) dataString.substr(stringOffset).c_str() ));
            denominators.push_back((int32_t) denominator);
            stringOffset += 4;
        }
    }

}

void GPMF::SCAL::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : scale denominators" << std::endl;
    int index = 1;
    std::cout << dataIndent << "[#] (denominator)\n";
    for ( auto denominator : denominators ) {
        std::cout << dataIndent << "[" << index << "] ( " << denominator << " )" << std::endl;
        index++;
    }
}

void GPMF::SCAL::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::SCAL::key = "SCAL";

