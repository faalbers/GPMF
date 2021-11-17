#ifndef GPMF_GYRO_H
#define GPMF_GYRO_H
#include <string>
#include <memory>
#include "klv.hpp"

namespace GPMF
{

class GYRO : public klv
{
public:
    GYRO(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    GYRO(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string                  key;
    std::vector<std::vector<int16_t>>   samples; // (x, y ,z)
};

}
#endif