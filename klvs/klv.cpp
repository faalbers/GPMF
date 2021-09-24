#include "klv.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

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
        //auto child = std::make_shared<klv>(filePath, childFilePos, path_+"/");
        auto child = makeKlv_(filePath_, childFilePos, path_+"/");
        if ( child != nullptr ) {
            childFilePos = child->fileNextPos_;
            children_.push_back(child);
        }
    } while ( childFilePos < fileNextPos_ );
}

void GPMF::klv::printHierarchy(int pathWith, int valLevel)
{
    std::cout << std::setw(pathWith) << std::left << path_;
    std::cout << "  |- " <<  std::setw(valLevel) << std::right << filePos_
        << " -|- " << std::setw(valLevel) << fileDataPos_
        << " -|- " << std::setw(valLevel) << fileNextPos_ << " -| "
        << "( " << std::setw(valLevel) << dataSize_ << " )";
    std::cout << std::endl;
    for ( auto child : children_ )
        child->printHierarchy(pathWith, valLevel);
}

void GPMF::klv::printData(bool fullLists)
{
    auto levelCount = std::count(path_.begin(), path_.end(), '/');
    std::string dataIndent = std::string((levelCount)*5+1, ' ');
    std::cout << path_ << " ('" << dataType << "' " << sampleSize << " " << dataRepeat << ") : klv not defined yet" << std::endl;
}

void GPMF::klv::printHierarchyData(bool fullLists)
{
    printData(fullLists);
    for ( auto child : children_ ) child->printHierarchyData(fullLists);
}

std::shared_ptr<GPMF::klv>   GPMF::klv::makeKlv_(std::string filePath_, int64_t nextFilePos, std::string pathParent)
{
    std::shared_ptr<klv> newAtom;

    std::ifstream fileStream(filePath_, std::ios::binary);
    if ( fileStream.fail() ) throw std::runtime_error("KLV can not open file: "+filePath_);

    // get file size and set file position to start of klv
    fileStream.seekg(nextFilePos, fileStream.beg);
    char    charKey[4];;
    fileStream.read((char *) &charKey, sizeof(charKey));
    fileStream.close();
    std::string key = std::string(charKey).substr(0,4);

    if ( key == "DEVC" ) newAtom = std::make_shared<DEVC>(filePath_, nextFilePos, pathParent);
    else if ( key == "DVNM" ) newAtom = std::make_shared<DVNM>(filePath_, nextFilePos, pathParent);
    else if ( key == "TICK" ) newAtom = std::make_shared<TICK>(filePath_, nextFilePos, pathParent);
    else if ( key == "STRM" ) newAtom = std::make_shared<STRM>(filePath_, nextFilePos, pathParent);
    else if ( key == "STMP" ) newAtom = std::make_shared<STMP>(filePath_, nextFilePos, pathParent);
    else if ( key == "TSMP" ) newAtom = std::make_shared<TSMP>(filePath_, nextFilePos, pathParent);
    else if ( key == "ORIN" ) newAtom = std::make_shared<ORIN>(filePath_, nextFilePos, pathParent);
    else if ( key == "ORIO" ) newAtom = std::make_shared<ORIO>(filePath_, nextFilePos, pathParent);
    else if ( key == "MTRX" ) newAtom = std::make_shared<MTRX>(filePath_, nextFilePos, pathParent);
    else if ( key == "STNM" ) newAtom = std::make_shared<STNM>(filePath_, nextFilePos, pathParent);
    else if ( key == "TYPE" ) newAtom = std::make_shared<TYPE>(filePath_, nextFilePos, pathParent);
    else if ( key == "TMPC" ) newAtom = std::make_shared<TMPC>(filePath_, nextFilePos, pathParent);
    else if ( key == "SIUN" ) newAtom = std::make_shared<SIUN>(filePath_, nextFilePos, pathParent);
    else if ( key == "UNIT" ) newAtom = std::make_shared<UNIT>(filePath_, nextFilePos, pathParent);
    else if ( key == "SCAL" ) newAtom = std::make_shared<SCAL>(filePath_, nextFilePos, pathParent);
    else if ( key == "ACCL" ) newAtom = std::make_shared<ACCL>(filePath_, nextFilePos, pathParent);
    else if ( key == "GYRO" ) newAtom = std::make_shared<GYRO>(filePath_, nextFilePos, pathParent);
    else if ( key == "GPS5" ) newAtom = std::make_shared<GPS5>(filePath_, nextFilePos, pathParent);
    else if ( key == "GPSF" ) newAtom = std::make_shared<GPSF>(filePath_, nextFilePos, pathParent);
    else if ( key == "SHUT" ) newAtom = std::make_shared<SHUT>(filePath_, nextFilePos, pathParent);
    else newAtom = std::make_shared<klv>(filePath_, nextFilePos, pathParent);

    return newAtom;
}

int GPMF::klv::nestLevel_(int level)
{
    level++;
    int childLevel, maxChildLevel = level;
    for (auto child : children_ ) {
        childLevel = child->nestLevel_(level);
        if ( childLevel > maxChildLevel ) maxChildLevel = childLevel;
    }
    return maxChildLevel;
}

void GPMF::klv::getKlvs_(std::string findKey, std::vector<std::shared_ptr<klv>> &found)
{
    for ( auto child : children_ ) {
        if ( child->key == findKey ) found.push_back(child);
        child->getKlvs_(findKey, found);
    }
}
