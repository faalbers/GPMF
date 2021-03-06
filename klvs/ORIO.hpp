#ifndef GPMF_ORIO_H
#define GPMF_ORIO_H
#include <string>
#include <memory>
#include "klv.hpp"

/*
The plan is to have the GPMF_ScaledData() call apply all the SCAL and MTRX manipulations
automatically (only only does scale currently.) MTRX, ORIN and ORIO all can be used.
MRTX might eventually carry the calibration for the orientation, that is why it is an array of floats,
but it doesn't today.
ORIN - describes the stored (input) orientation,
ORIO is the recommended output orientation (which will match HERO5 for consistency.)
In HERO7 we have:
"ORIN": "YxZ",
"ORIO": "ZXY",
lower case indicates a sign reversal.
The above means:
outACCL[0] = inACCL[2];
outACCL[1] = -inACCL[1];
outACCL[2] = inACCL[0];
*/

namespace GPMF
{

class ORIO : public klv
{
public:
    ORIO(std::string filePath, uint64_t filePos, std::string pathParent = "/");
    ORIO(std::string &dataString, std::string pathParent = "/");

    void printData(bool fullLists = false);
    void printHierarchyData(bool fullLists = false);

    static std::string  key;
    std::string         name;
};

}
#endif