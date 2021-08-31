#ifndef GPMF_GPS5_H
#define GPMF_GPS5_H

#include <string>
#include <memory>
#include <vector>
#include "../datatypes.hpp"
#include "klv.hpp"

namespace GPMF
{

class GPS5 : public klv
{
public:
    GPS5(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string      key;
    std::vector<gpsType>    gps;
};

}
#endif