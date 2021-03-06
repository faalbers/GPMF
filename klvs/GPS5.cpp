#include "GPS5.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::GPS5::GPS5(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 'l' )
        error_("GPS5 klv wrong data type: "+std::string(1, dataType));
    if ( sampleSize != 20 )
        error_("GPS5 klv wrong sample size: " + std::to_string(sampleSize));

    size_t stringOffset = 0;
    int32_t val;
    for ( int i = (int) dataRepeat; i > 0; i-- ) {
        std::vector<int32_t> gpsEntry;
        val = _byteswap_ulong(*((uint32_t *) dataString.substr(stringOffset).c_str() ));
        gpsEntry.push_back(val);
        stringOffset += 4;
        val = _byteswap_ulong(*((uint32_t *) dataString.substr(stringOffset).c_str() ));
        gpsEntry.push_back(val);
        stringOffset += 4;
        val = _byteswap_ulong(*((uint32_t *) dataString.substr(stringOffset).c_str() ));
        gpsEntry.push_back(val);
        stringOffset += 4;
        val = _byteswap_ulong(*((uint32_t *) dataString.substr(stringOffset).c_str() ));
        gpsEntry.push_back(val);
        stringOffset += 4;
        val = _byteswap_ulong(*((uint32_t *) dataString.substr(stringOffset).c_str() ));
        gpsEntry.push_back(val);
        stringOffset += 4;
        samples.push_back(gpsEntry);
    }
}

void GPMF::GPS5::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : GPS samples" << std::endl;
    size_t index = 1;
    std::cout << dataIndent << "[#] (Lat., Long., Alt., 2D speed, 3D speed)\n";
    if ( fullLists || (!fullLists && samples.size() <= 6) ) {
        for ( auto gpsEntry : samples ) {
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
                << samples[index][0]
                << ", " << samples[index][1]
                << ", " << samples[index][2]
                << ", " << samples[index][3]
                << ", " << samples[index][4]
                << " )" << std::endl;
        }
        std::cout << dataIndent << "     ...\n";
        for ( index = samples.size()-3 ; index < samples.size(); index++ ) {
            std::cout << dataIndent << "[" << index+1 << "] ( "
                << samples[index][0]
                << ", " << samples[index][1]
                << ", " << samples[index][2]
                << ", " << samples[index][3]
                << ", " << samples[index][4]
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

