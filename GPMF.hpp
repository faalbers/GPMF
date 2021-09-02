#ifndef GPMF_GPMF_H
#define GPMF_GPMF_H

#include <memory>
#include <vector>
#include "MP4.hpp"
#include "klvs.hpp"

namespace GPMF
{

class GPMF
{
public:
    GPMF(std::string fileName);

    void     printHierarchy();                              // Hierarchy print out of all klvs in file
    void     printHierarchyData(bool fullLists = false);    // Data print out. Only data that is handled so far
    
    std::vector<sampleType> getAcceleration();
    std::vector<sampleType> getGyroscope();
    std::vector<sampleType> getGPS();
    void                    exportGPStoGPX(std::string fileName);
    
    std::vector<std::shared_ptr<klv>>  getKlvs(std::string findKey, klv *parent = nullptr);
    template<typename T>
    std::vector<T *>     getTypeKlvs(klv *parent = nullptr)
    {
        std::vector<T *> foundTypeKlvs;
        for( auto foundKlv : getKlvs(T::key, parent) ) foundTypeKlvs.push_back((T *) foundKlv.get());
        return foundTypeKlvs;
    }

private:
    int                                 nestLevel();

	std::shared_ptr<MP4::MP4> 		    mp4_;
    std::vector<std::shared_ptr<klv>>   payloads_;
};

}

#endif
