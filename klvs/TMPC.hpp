#ifndef GPMF_TMPC_H
#define GPMF_TMPC_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class TMPC : public klv
{
public:
    TMPC(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    float               temperature;
};

}
#endif