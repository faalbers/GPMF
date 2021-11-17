#ifndef GPMF_SIUN_H
#define GPMF_SIUN_H
#include <string>
#include <memory>
#include <vector>
#include "klv.hpp"

namespace GPMF
{

class SIUN : public klv
{
public:
    SIUN(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    SIUN(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string          key;
    std::vector<std::string>    units;
};

}
#endif