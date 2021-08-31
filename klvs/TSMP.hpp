#ifndef GPMF_TSMP_H
#define GPMF_TSMP_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class TSMP : public klv
{
public:
    TSMP(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    uint32_t            totalSampleCount;
};

}
#endif