#ifndef GPMF_UNIT_H
#define GPMF_UNIT_H
#include <string>
#include <memory>
#include <vector>
#include "klv.hpp"

namespace GPMF
{

class UNIT : public klv
{
public:
    UNIT(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string          key;
    std::vector<std::string>    units;
};

}
#endif