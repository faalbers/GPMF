#ifndef GPMF_SCAL_H
#define GPMF_SCAL_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class SCAL : public klv
{
public:
    SCAL(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    int16_t             denominator;
};

}
#endif