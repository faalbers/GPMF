#include "GPMF.hpp"

GPMF::GPMF::GPMF(std::string fileName)
    : mp4_(new MP4::MP4(fileName))
{
    //mp4_->printHierarchyData();
    MP4::trak *GPMFtrack = nullptr;
    for ( auto track : mp4_->getTracks()) {
        for ( auto sampleD : track->getSampleDescriptions()) {
            if ( sampleD.dataFormat == "gpmd") {
                for ( auto dataReference : track->getDataReferences()) {
                    if ( dataReference.ID == sampleD.dataReferenceIndex )
                        if ( dataReference.reference == "" )
                            GPMFtrack = track;
                }
            }
        }
    }
    if ( GPMFtrack == nullptr )
        throw std::runtime_error("GPMF: Can't find GPMF track in file !");
    
    //auto samples = GPMFtrack->getSamples();
    for ( auto chunk : GPMFtrack->getChunks() ) {
        #ifdef GPMF_PARSE_PATH
        std::cout << "PAYLOAD: " << chunk.ID << std::endl;
        #endif
        payloads_.push_back(std::make_shared<klv>(mp4_->filePath, chunk.dataOffset));
    }
}
