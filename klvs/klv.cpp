#include "klv.hpp"
#include <fstream>
#include <iostream>

GPMF::klv::klv(std::string filePath, uint64_t filePos, std::string pathParent)
    : filePath_(filePath)
    , filePos_(filePos)
    , parentPath_(pathParent)
{
    uint64_t childFilePos;

    std::ifstream fileStream(filePath, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("Atom can not parse file: "+filePath);

    // get klv header
    datablock::klvHeaderBlock headerBlock;
    fileStream.seekg(filePos, fileStream.beg);
    fileStream.read((char *) &headerBlock, sizeof(headerBlock));
    fileDataPos_ = fileStream.tellg();
    fileStream.close();

    key = std::string(headerBlock.key).substr(0,4);
    path_ = parentPath_ + key;

    // get data block size
    headerBlock.repeat = _byteswap_ushort(headerBlock.repeat);
    dataSize_ = (int64_t) headerBlock.repeat * (int64_t) headerBlock.sampleSize;
    dataSize_ += (4 - (dataSize_ % 4)) * std::min(dataSize_ % 4, (int64_t) 1); // uint32_t padding
    fileNextPos_ = fileDataPos_ + dataSize_;

    // get data type
    dataType = headerBlock.type;
    sampleSize = (int64_t) headerBlock.sampleSize;
    dataRepeat = (int64_t) headerBlock.repeat;

    #ifdef GPMF_PARSE_PATH
    if ( dataType == 0 ) std::cout << path_ << " <null><size><repeat>\n";
    else std::cout << path_ << " '" << dataType << "' " << sampleSize << " " << dataRepeat << std::endl;
    #endif

    // nesting
    if (  dataType != 0 ) return;
    childFilePos = fileDataPos_;
    do {
        auto child = std::make_shared<klv>(filePath, childFilePos, path_+"/");
        if ( child != nullptr ) {
            childFilePos = child->fileNextPos_;
            children_.push_back(child);
        }
    } while ( childFilePos < fileNextPos_ );
}