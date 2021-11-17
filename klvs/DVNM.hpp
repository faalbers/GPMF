#ifndef GPMF_DVNM_H
#define GPMF_DVNM_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class DVNM : public klv
{
public:
    DVNM(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    DVNM(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif