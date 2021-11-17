#ifndef GPMF_DZOM_H
#define GPMF_DZOM_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class DZOM : public klv
{
public:
    DZOM(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    DZOM(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif