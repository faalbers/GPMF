#ifndef GPMF_PTWB_H
#define GPMF_PTWB_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class PTWB : public klv
{
public:
    PTWB(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    PTWB(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif