#ifndef GPMF_SCAL_H
#define GPMF_SCAL_H
#include <string>
#include <memory>
#include <vector>
#include "klv.hpp"

namespace GPMF
{

class SCAL : public klv
{
public:
    SCAL(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    SCAL(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string      key;
    std::vector<int32_t>    denominators;
};

}
#endif