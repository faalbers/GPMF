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

private:
	std::shared_ptr<MP4::MP4> 		    mp4_;
    std::vector<std::shared_ptr<klv>>   payloads_;
};

}

#endif
