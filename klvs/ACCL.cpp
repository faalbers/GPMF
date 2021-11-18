#include "ACCL.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>

GPMF::ACCL::ACCL(std::string &dataString, std::string pathParent)
    : klv(dataString, pathParent)
{
    // throw an error if at one point they decide to change the data type
    if ( dataType != 's' )
        error_("ACCL klv wrong data type: "+std::string((char *)&dataType).substr(0,1));
    if ( sampleSize != 6 )
        error_("ACCL klv wrong sample size: " + std::to_string(sampleSize));

    size_t stringOffset = 0;
    int16_t val;
    for ( int i = (int) dataRepeat; i > 0; i-- ) {
        std::vector<int16_t> sample;
        val = _byteswap_ushort(*((uint16_t *) dataString.substr(stringOffset).c_str() ));
        sample.push_back(val);
        stringOffset += 2;
        val = _byteswap_ushort(*((uint16_t *) dataString.substr(stringOffset).c_str() ));
        sample.push_back(val);
        stringOffset += 2;
        val = _byteswap_ushort(*((uint16_t *) dataString.substr(stringOffset).c_str() ));
        sample.push_back(val);
        stringOffset += 2;
        samples.push_back(sample);
    }
}

void GPMF::ACCL::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : accelerometer samples" << std::endl;
    size_t index = 1;
    std::cout << dataIndent << "[#] (up/down, right/left, forward/back)\n";
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

void GPMF::ACCL::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::string GPMF::ACCL::key = "ACCL";

