#include "MTRX.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::MTRX::MTRX(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'f' )
        throw std::runtime_error("MTRX klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( (dataRepeat * sampleSize) != 36 )
        throw std::runtime_error("MTRX klv has wrong size: " + std::to_string(dataRepeat * sampleSize));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("MTRX klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);

    uint32_t intToFloat;
    float matrixEntry;
    for ( int i = 0 ; i < 3 ; i++ ) {
        std::vector<float> row;
        for ( int j = 0 ; j < 3 ; j++ ) {
            fileStream.read((char *) &intToFloat, sizeof(intToFloat));
            intToFloat = _byteswap_ulong(intToFloat);
            matrixEntry = *((float *)&intToFloat);
            matrix[i][j] = matrixEntry;
        }
    }
    
    fileStream.close();
}

GPMF::MTRX::MTRX(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
}

void GPMF::MTRX::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : transformation matrix" << std::endl;
    std::cout << dataIndent << "matrix:" << std::endl;
    std::cout << dataIndent << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << " " << std::endl;
    std::cout << dataIndent << matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << " " << std::endl;
    std::cout << dataIndent << matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << " " << std::endl;
}

void GPMF::MTRX::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::MTRX::key = "MTRX";

