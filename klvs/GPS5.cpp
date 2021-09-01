#include "GPS5.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::GPS5::GPS5(std::string filePath, uint64_t filePos, std::string pathParent)
    : klv(filePath, filePos, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'l' )
        throw std::runtime_error("GPS5 klv wrong data type: "+std::string(1, dataType));
    if ( sampleSize != 20 )
        throw std::runtime_error("GPS5 klv wrong sample size: " + std::to_string(sampleSize));

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("GPS5 klv can not parse file: "+filePath);
    fileStream.seekg(fileDataPos_, fileStream.beg);

    int32_t val;
    for ( int i = (int) dataRepeat; i > 0; i-- ) {
        std::vector<int32_t> gpsEntry;
        fileStream.read((char *) &val, sizeof(val));
        gpsEntry.push_back(_byteswap_ulong(val));
        fileStream.read((char *) &val, sizeof(val));
        gpsEntry.push_back(_byteswap_ulong(val));
        fileStream.read((char *) &val, sizeof(val));
        gpsEntry.push_back(_byteswap_ulong(val));
        fileStream.read((char *) &val, sizeof(val));
        gpsEntry.push_back(_byteswap_ulong(val));
        fileStream.read((char *) &val, sizeof(val));
        gpsEntry.push_back(_byteswap_ulong(val));
        gps.push_back(gpsEntry);
    }

    fileStream.close();
}

void GPMF::GPS5::printData(bool fullLists)
{
    int levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : GPS samples" << std::endl;
    int index = 1;
    std::cout << dataIndent << "[#] (Lat., Long., Alt., 2D speed, 3D speed)\n";
    if ( fullLists || (!fullLists && gps.size() <= 6) ) {
        for ( auto gpsEntry : gps ) {
            std::cout << dataIndent << "[" << index << "] ( "
                << gpsEntry[0]
                << ", " << gpsEntry[1]
                << ", " << gpsEntry[2]
                << ", " << gpsEntry[3]
                << ", " << gpsEntry[4]
                << " )" << std::endl;
            index++;
        }
    } else {
        for ( index = 0 ; index < 3; index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << gps[index][0]
                << ", " << gps[index][1]
                << ", " << gps[index][2]
                << ", " << gps[index][3]
                << ", " << gps[index][4]
                << " )" << std::endl;
        }
        std::cout << dataIndent << "     ...\n";
        for ( index = gps.size()-3 ; index < gps.size(); index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << gps[index][0]
                << ", " << gps[index][1]
                << ", " << gps[index][2]
                << ", " << gps[index][3]
                << ", " << gps[index][4]
                << " )" << std::endl;
        }
    }
}

void GPMF::GPS5::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::GPS5::key = "GPS5";

