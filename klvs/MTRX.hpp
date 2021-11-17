#ifndef GPMF_MTRX_H
#define GPMF_MTRX_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class MTRX : public klv
{
public:
    MTRX(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    MTRX(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    float               matrix[3][3];
};

}
#endif