#ifndef GPMF_MINF_H
#define GPMF_MINF_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class MINF : public klv
{
public:
    MINF(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    MINF(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif