#include "GPMF.hpp"
#include <iostream>
#include <iomanip>

GPMF::GPMF::GPMF(std::string fileName)
    : mp4_(new MP4::MP4(fileName))
{
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
        //payloads_.push_back(std::make_shared<klv>(mp4_->filePath, chunk.dataOffset));
        payloads_.push_back( klv::makeKlv_(mp4_->filePath, chunk.dataOffset) );
    }
}

void GPMF::GPMF::printHierarchy()
{
    int pathWith = nestLevel() * 5;
    int valWith = 10;
    int lineWith = pathWith + valWith*5 + 13;
    int payloadWith = lineWith/2 - 5;
    std::cout << std::string(lineWith, '-') << std::endl;
    std::cout << std::setw(pathWith) << std::left << "<path>";
    std::cout <<"  |- " << std::setw(valWith) << std::right << "<start>"
        << " -|- " << std::setw(valWith) << "<data>"
        << " -|- " << std::setw(valWith) << "<end>"
        << " -| ( " << std::setw(valWith) << "<datasize>"
        << " )\n";

    int index = 1;
    for ( auto payload : payloads_ ) {
        std::cout << std::string(payloadWith-1, '-')
            << " PAYLOAD " << index << " " << std::string(payloadWith, '-') << std::endl;
        payload->printHierarchy(pathWith, valWith);
        index++;
    }
    std::cout << std::string(lineWith, '-') << std::endl;
}

void GPMF::GPMF::printHierarchyData(bool fullLists)
{
    int index = 1;
    for ( auto payload : payloads_ ) {
        std::cout << std::string(15, '-') << " PAYLOAD " << index << " " << std::string(15, '-') << std::endl;
        payload->printHierarchyData(fullLists);
        index++;
    }
}

int GPMF::GPMF::nestLevel()
{
    int level, maxLevel = 0;
    for ( auto payload : payloads_ ) {
        level = payload->nestLevel_(0);
        if ( level > maxLevel ) maxLevel = level;
    }
    return maxLevel;
}
