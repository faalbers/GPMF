#include "GYRO.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::GYRO::GYRO(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 's' )
        throw std::runtime_error("GYRO klv wrong data type: "+std::string(1, dataType));
    if ( sampleSize != 6 )
        throw std::runtime_error("GYRO klv wrong sample size: " + std::to_string(sampleSize));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("GYRO klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);

    int16_t val;
    for ( int i = (int) dataRepeat; i > 0; i-- ) {
        std::vector<int16_t> sample;
        fileStream.read((char *) &val, sizeof(val));
        val = _byteswap_ushort(val);
        sample.push_back(val);
        fileStream.read((char *) &val, sizeof(val));
        val = _byteswap_ushort(val);
        sample.push_back(val);
        fileStream.read((char *) &val, sizeof(val));
        val = _byteswap_ushort(val);
        sample.push_back(val);
        samples.push_back(sample);
    }

    fileStream.close();
}

void GPMF::GYRO::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : gyroscope samples" << std::endl;
    size_t index = 1;
    std::cout << dataIndent << "[#] (z,x,y)\n";
    if ( fullLists || (!fullLists && samples.size() <= 6) ) {
        for ( auto sample : samples ) {
            std::cout << dataIndent << "[" << index << "] ( "
                << sample[0]
                << ", " << sample[1]
                << ", " << sample[2]
                << " )" << std::endl;
            index++;
        }
    } else {
        for ( index = 0 ; index < 3; index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << samples[index][0]
                << ", " << samples[index][1]
                << ", " << samples[index][2]
                << " )" << std::endl;
        }
        std::cout << dataIndent << "     ...\n";
        for ( index = samples.size()-3 ; index < samples.size(); index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << samples[index][0]
                << ", " << samples[index][1]
                << ", " << samples[index][2]
                << " )" << std::endl;
        }
    }
}

void GPMF::GYRO::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::GYRO::key = "GYRO";

