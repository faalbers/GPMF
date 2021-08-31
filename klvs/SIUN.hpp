#ifndef GPMF_SIUN_H
#define GPMF_SIUN_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class SIUN : public klv
{
public:
    SIUN(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         units;
};

}
#endif