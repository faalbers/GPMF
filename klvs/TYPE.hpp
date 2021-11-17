#ifndef GPMF_TYPE_H
#define GPMF_TYPE_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class TYPE : public klv
{
public:
    TYPE(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    TYPE(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         type;
};

}
#endif