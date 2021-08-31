#ifndef GPMF_STRM_H
#define GPMF_STRM_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class STRM : public klv
{
public:
    STRM(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
};

}
#endif