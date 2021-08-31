#include "GPMF.hpp"

GPMF::GPMF::GPMF(std::string fileName)
    : mp4_(new MP4::MP4(fileName))
{
}
