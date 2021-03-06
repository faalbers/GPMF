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
    klv(std::string &dataString, std::string pathParent = "/");

    template<typename T>
    std::vector<T *>     getTypeKlvs()
    {
        std::vector<T *> foundTypeKlvs;
        std::vector<std::shared_ptr<klv>> foundKlvs;
        getKlvs_(T::key, foundKlvs);
        for( auto foundKlv : foundKlvs ) foundTypeKlvs.push_back((T *) foundKlv.get());
        return foundTypeKlvs;
    }

    static std::string getZeroTerminatedString(std::string &dataString);

    void printHierarchy(int pathWith = 35, int valLevel = 10);
    virtual void printData(bool fullLists = false);
    virtual void printHierarchyData(bool fullLists = false);

    std::string key;
    uint8_t     dataType;
    int64_t     sampleSize;
    int64_t	    dataRepeat;

protected:
    friend class GPMF;
    static std::shared_ptr<klv> makeKlv_(std::string &dataString, std::string pathParent = "/");
    int                         nestLevel_(int level);
    void                        getKlvs_(std::string findKey, std::vector<std::shared_ptr<klv>> &found);
    static void                 error_(std::string message);

    //int64_t                             size_;
    std::string                         filePath_;
    std::string                         path_;
    std::string                         parentPath_;
    int64_t                             filePos_, fileDataPos_, fileNextPos_;
    int64_t                             dataSize_;
    std::vector<std::shared_ptr<klv>>   children_;

    size_t                              sDataPos_;
    size_t                              sDataSize_;
};

}

#endif
