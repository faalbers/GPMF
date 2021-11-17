#ifndef GPMF_LINF_H
#define GPMF_LINF_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class LINF : public klv
{
public:
    LINF(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    LINF(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif