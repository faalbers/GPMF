#ifndef GPMF_ACCL_H
#define GPMF_ACCL_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class ACCL : public klv
{
public:
    ACCL(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string                  key;
    std::vector<std::vector<int16_t>>   samples; // (x, y ,z)
};

}
#endif