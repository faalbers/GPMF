#ifndef GPMF_KLV_H
#define GPMF_KLV_H

#include "../datatypes.hpp"
#include <string>
#include <vector>
#include <memory>

namespace GPMF
{

class klv
{
public:
    klv(std::string filePath, uint64_t filePos, std::string pathParent = "/");

    std::string key;
    uint8_t     dataType;
    int64_t     sampleSize;
    int64_t	    dataRepeat;

protected:
    //int64_t                             size_;
    std::string                         filePath_;
    std::string                         path_;
    std::string                         parentPath_;
    int64_t                             filePos_, fileDataPos_, fileNextPos_;
    int64_t                             dataSize_;
    std::vector<std::shared_ptr<klv>>   children_;
};

}

#endif
