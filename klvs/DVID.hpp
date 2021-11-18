#ifndef GPMF_DVID_H
#define GPMF_DVID_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class DVID : public klv
{
public:
    DVID(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    DVID(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    uint32_t            inTime;
};

}
#endif