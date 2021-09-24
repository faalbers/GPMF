#ifndef GPMF_STMP_H
#define GPMF_STMP_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class STMP : public klv
{
public:
    STMP(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    uint64_t            timeStamp; // µs  Increased precision for post stablization
};

}
#endif