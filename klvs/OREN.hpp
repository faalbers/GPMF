#ifndef GPMF_OREN_H
#define GPMF_OREN_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class OREN : public klv
{
public:
    OREN(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    OREN(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif