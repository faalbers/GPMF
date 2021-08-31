#ifndef GPMF_DEVC_H
#define GPMF_DEVC_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class DEVC : public klv
{
public:
    DEVC(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
};

}
#endif