#ifndef GPMF_GPMF_H
#define GPMF_GPMF_H

#include <memory>
#include "MP4.hpp"

namespace GPMF
{

class GPMF
{
public:
    GPMF(std::string fileName);

private:
	std::shared_ptr<MP4::MP4> 		    mp4_;
};

}

#endif
