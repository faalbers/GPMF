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

    std::vector<std::shared_ptr<klv>>  getKlvs(std::string findKey, klv *parent = nullptr);
    template<typename T>
    std::vector<T *>     getTypeKlvs(klv *parent = nullptr)
    {
        std::vector<T *> foundTypeKlvs;
        for( auto foundKlv : getKlvs(T::key, parent) ) foundTypeKlvs.push_back((T *) foundKlv.get());
        return foundTypeKlvs;
    }

    void printHierarchy(int pathWith = 35, int valLevel = 10);
    virtual void printData(bool fullLists = false);
    virtual void printHierarchyData(bool fullLists = false);

    std::string key;
    uint8_t     dataType;
    int64_t     sampleSize;
    int64_t	    dataRepeat;

protected:
    friend class GPMF;
    static std::shared_ptr<klv> makeKlv_(std::string filePath, int64_t nextFilePos, std::string pathParent = "/");
    int                         nestLevel_(int level);
    void                        getKlvs_(std::string findKey, std::vector<std::shared_ptr<klv>> &found);

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
