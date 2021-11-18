#ifndef GPMF_GPSF_H
#define GPMF_GPSF_H
#include <string>
#include <memory>
#include "klv.hpp"

/*
Within the GPS stream: 0 - no lock, 2 or 3 - 2D or 3D Lock
*/

namespace GPMF
{

class GPSF : public klv
{
public:
    GPSF(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    GPSF(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    uint32_t            gpsFix;
};

}
#endif