#ifndef GPMF_STNM_H
#define GPMF_STNM_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class STNM : public klv
{
public:
    STNM(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif