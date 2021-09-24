#ifndef GPMF_SHUT_H
#define GPMF_SHUT_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class SHUT : public klv
{
public:
    SHUT(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::vector<float>  samples; // (sec)
};

}
#endif