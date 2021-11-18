#include "klv.hpp"
#include "../klvs.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

GPMF::klv::klv(std::string &dataString, std::string pathParent)
    : filePath_("")
    , filePos_(0)
    , parentPath_(pathParent)
{
    datablock::klvHeaderBlock headerBlock;
    std::string headerBlockString;

    // get klv header
    const char *copyData;
    if ( sizeof(headerBlock) > dataString.size())
        error_("constructor data read too large");
    headerBlockString = dataString.substr(0,sizeof(headerBlock));
    copyData = headerBlockString.c_str();
    memcpy(&headerBlock, copyData, sizeof(headerBlock));
    dataString = dataString.substr(sizeof(headerBlock));

    key = std::string(headerBlock.key).substr(0,4);
    path_ = parentPath_ + key;

    // get data block size
    headerBlock.repeat = _byteswap_ushort(headerBlock.repeat);

    sDataSize_ = (size_t) headerBlock.repeat * (size_t) headerBlock.sampleSize;
    sDataSize_ += (4 - (sDataSize_ % 4)) * std::min(sDataSize_ % 4, (size_t) 1); // uint32_t padding

    dataSize_ = (int64_t) headerBlock.repeat * (int64_t) headerBlock.sampleSize;
    dataSize_ += (4 - (dataSize_ % 4)) * std::min(dataSize_ % 4, (int64_t) 1); // uint32_t padding
    
    //auto nextDataStringSize = dataString.size() - (size_t) dataSize_;
    auto nextDataStringSize = dataString.size() - sDataSize_;
    
    // get data type
    dataType = headerBlock.type;
    sampleSize = (int64_t) headerBlock.sampleSize;
    dataRepeat = (int64_t) headerBlock.repeat;
   
    // nesting
    if (  dataType != 0 ) return;
    do {
        auto child = makeKlv_(dataString, path_+"/");
        if ( child->dataType != 0 )
            dataString = dataString.substr(child->sDataSize_);
        if ( child != nullptr ) children_.push_back(child);
    } while ( dataString.size() > nextDataStringSize );
}

std::string GPMF::klv::getZeroTerminatedString(std::string &dataString)
{
    std::stringstream charss;
    for ( auto charByte : dataString ) {
        if ( charByte == 0 ) break;
        charss << charByte;
    }
    return charss.str();
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

std::shared_ptr<GPMF::klv> GPMF::klv::makeKlv_(std::string &dataString, std::string pathParent)
{
    std::shared_ptr<klv> newKlv;

    // get klv header
    datablock::klvHeaderBlock headerBlock;
    if ( sizeof(headerBlock) > dataString.size())
        error_("constructor data read too large");
    std::string headerBlockString = dataString.substr(0,sizeof(headerBlock));
    const char *copyData = headerBlockString.c_str();
    memcpy(&headerBlock, copyData, sizeof(headerBlock));

    auto foundKey = std::string(headerBlock.key).substr(0,4);;

    if ( foundKey == "DEVC" ) newKlv = std::make_shared<DEVC>(dataString, pathParent);
    else if ( foundKey == "STRM" ) newKlv = std::make_shared<STRM>(dataString, pathParent);
    else if ( foundKey == "DVNM" ) newKlv = std::make_shared<DVNM>(dataString, pathParent);
    else if ( foundKey == "DVID" ) newKlv = std::make_shared<DVID>(dataString, pathParent);
    else if ( foundKey == "LINF" ) newKlv = std::make_shared<LINF>(dataString, pathParent);
    else if ( foundKey == "CASN" ) newKlv = std::make_shared<CASN>(dataString, pathParent);
    else if ( foundKey == "MINF" ) newKlv = std::make_shared<MINF>(dataString, pathParent);
    else if ( foundKey == "FMWR" ) newKlv = std::make_shared<FMWR>(dataString, pathParent);
    else if ( foundKey == "OREN" ) newKlv = std::make_shared<OREN>(dataString, pathParent);
    else if ( foundKey == "DZOM" ) newKlv = std::make_shared<DZOM>(dataString, pathParent);
    else if ( foundKey == "SMTR" ) newKlv = std::make_shared<SMTR>(dataString, pathParent);
    else if ( foundKey == "PTWB" ) newKlv = std::make_shared<PTWB>(dataString, pathParent);
    else if ( foundKey == "PTSH" ) newKlv = std::make_shared<PTSH>(dataString, pathParent);
    else if ( foundKey == "TICK" ) newKlv = std::make_shared<TICK>(dataString, pathParent);
    else if ( foundKey == "STMP" ) newKlv = std::make_shared<STMP>(dataString, pathParent);
    else if ( foundKey == "TSMP" ) newKlv = std::make_shared<TSMP>(dataString, pathParent);
    else if ( foundKey == "ORIN" ) newKlv = std::make_shared<ORIN>(dataString, pathParent);
    else if ( foundKey == "ORIO" ) newKlv = std::make_shared<ORIO>(dataString, pathParent);
    else if ( foundKey == "MTRX" ) newKlv = std::make_shared<MTRX>(dataString, pathParent);
    else if ( foundKey == "STNM" ) newKlv = std::make_shared<STNM>(dataString, pathParent);
    else if ( foundKey == "TYPE" ) newKlv = std::make_shared<TYPE>(dataString, pathParent);
    else if ( foundKey == "TMPC" ) newKlv = std::make_shared<TMPC>(dataString, pathParent);
    else if ( foundKey == "SIUN" ) newKlv = std::make_shared<SIUN>(dataString, pathParent);
    else if ( foundKey == "UNIT" ) newKlv = std::make_shared<UNIT>(dataString, pathParent);
    else if ( foundKey == "SCAL" ) newKlv = std::make_shared<SCAL>(dataString, pathParent);
    else if ( foundKey == "ACCL" ) newKlv = std::make_shared<ACCL>(dataString, pathParent);
    else if ( foundKey == "GYRO" ) newKlv = std::make_shared<GYRO>(dataString, pathParent);
    else if ( foundKey == "GPS5" ) newKlv = std::make_shared<GPS5>(dataString, pathParent);
    else if ( foundKey == "GPSF" ) newKlv = std::make_shared<GPSF>(dataString, pathParent);
    else if ( foundKey == "SHUT" ) newKlv = std::make_shared<SHUT>(dataString, pathParent);
    else if ( foundKey == "WBAL" ) newKlv = std::make_shared<WBAL>(dataString, pathParent);
    else newKlv = std::make_shared<klv>(dataString, pathParent);
    
    return newKlv;
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

void GPMF::klv::error_(std::string message)
{
    std::cout << "GPMF::klv:\n";
    std::cout << "-> " << message << std::endl;
    std::cout << "exit application ..." << std::endl;
    exit(1);
}