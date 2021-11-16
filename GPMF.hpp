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
    //std::vector<sampleType> getShutter();
    void                    exportGPStoGPX(std::string fileName);

private:
    int     nestLevel();

    std::shared_ptr<MP4::Parser> 	    mp4parser_;
    std::vector<std::shared_ptr<klv>>   payloads_;
};

}

#endif
