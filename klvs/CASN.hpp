#ifndef GPMF_CASN_H
#define GPMF_CASN_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class CASN : public klv
{
public:
    CASN(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    CASN(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif