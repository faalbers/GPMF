#ifndef GPMF_DEVC_H
#define GPMF_DEVC_H
#include <string>
#include <memory>
#include "klv.hpp"
#include "STRM.hpp"

namespace GPMF
{

class DEVC : public klv
{
public:
    DEVC(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    DEVC(std::string &dataString, std::string pathParent = "/");

    std::vector<STRM *>  getStreams();

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    uint32_t            timeScale;
    uint32_t            currentTime;
    uint32_t            duration;
};

}
#endif