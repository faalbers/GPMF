#ifndef GPMF_TICK_H
#define GPMF_TICK_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class TICK : public klv
{
public:
    TICK(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    uint32_t            inTime;
};

}
#endif