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

    gpsType gpsEntry;
    for ( int i = (int) dataRepeat; i > 0; i-- ) {
        fileStream.read((char *) &gpsEntry, sizeof(gpsEntry));
        gpsEntry.latitude = _byteswap_ulong(gpsEntry.latitude);
        gpsEntry.longitude = _byteswap_ulong(gpsEntry.longitude);
        gpsEntry.altitude = _byteswap_ulong(gpsEntry.altitude);
        gpsEntry.speed2D = _byteswap_ulong(gpsEntry.speed2D);
        gpsEntry.speed3D = _byteswap_ulong(gpsEntry.speed3D);
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
                << gpsEntry.latitude
                << ", " << gpsEntry.longitude
                << ", " << gpsEntry.altitude
                << ", " << gpsEntry.speed2D
                << ", " << gpsEntry.speed3D
                << " )" << std::endl;
            index++;
        }
    } else {
        for ( index = 0 ; index < 3; index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << gps[index].latitude
                << ", " << gps[index].longitude
                << ", " << gps[index].altitude
                << ", " << gps[index].speed2D
                << ", " << gps[index].speed3D
                << " )" << std::endl;
        }
        std::cout << dataIndent << "     ...\n";
        for ( index = gps.size()-3 ; index < gps.size(); index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << gps[index].latitude
                << ", " << gps[index].longitude
                << ", " << gps[index].altitude
                << ", " << gps[index].speed2D
                << ", " << gps[index].speed3D
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

