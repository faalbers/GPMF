#ifndef GPMF_FMWR_H
#define GPMF_FMWR_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class FMWR : public klv
{
public:
    FMWR(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    FMWR(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif