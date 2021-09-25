#ifndef GPMF_WBAL_H
#define GPMF_WBAL_H
#include <string>
#include <memory>
#include <vector>
#include "klv.hpp"

namespace GPMF
{

class WBAL : public klv
{
public:
    WBAL(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string      key;
    std::vector<uint16_t>   whiteBalances;
};

}
#endif