#ifndef GPMF_SMTR_H
#define GPMF_SMTR_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class SMTR : public klv
{
public:
    SMTR(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    SMTR(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif