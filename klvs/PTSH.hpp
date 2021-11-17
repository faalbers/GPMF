#ifndef GPMF_PTSH_H
#define GPMF_PTSH_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class PTSH : public klv
{
public:
    PTSH(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    PTSH(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif