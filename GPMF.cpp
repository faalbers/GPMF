#include "GPMF.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#ifdef GPMF_PARSE_TIME
#include <chrono>
#endif


GPMF::GPMF::GPMF(std::string fileName)
    : mp4parser_(std::make_shared<MP4::Parser>(fileName))
    , udtaPayload_(nullptr)
{
    #ifdef GPMF_PARSE_TIME
    auto start = std::chrono::high_resolution_clock::now();
    #endif
    
    std::string GPMFdata;

    // get udta GPMF payload
    GPMFdata = mp4parser_->getUserData("GPMF");
    if ( GPMFdata == "" )
        error_("constructor can not find GPMF user data");
    udtaPayload_ = klv::makeKlv_(GPMFdata);

    // get GPMF sample payloads
    auto GPMFtrack = mp4parser_->getTrack("gpmd");
    if ( GPMFtrack == nullptr )
        error_("constructor can't find GPMF track in file !");

    // find largest sample buffer size
    size_t bufferSize = 0;
    for ( auto sample : GPMFtrack->samples )
        if ( sample.second.size > bufferSize ) bufferSize = sample.second.size;
    auto buffer = new char[bufferSize];

    DEVC *currentPayload = nullptr;
    std::string filePath = "";
    std::ifstream fileRead;
    for ( auto sample : GPMFtrack->samples ) {
        if ( sample.second.filePath != filePath ) {
            filePath = sample.second.filePath;
            if ( fileRead.is_open() ) fileRead.close();
            fileRead.open(filePath, std::ios::binary);
            if ( fileRead.fail() ) error_("constructer can not read file: "+filePath);
        }
        fileRead.seekg(sample.second.filePos, fileRead.beg);
        fileRead.read((char *) buffer, (size_t) sample.second.size);
        GPMFdata = std::string(buffer, (size_t) sample.second.size);
        auto payload = klv::makeKlv_(GPMFdata);
        //currentPayload = (DEVC *) payload.get();
        //currentPayload->timeScale = GPMFtrack->mediaTimeScale;
        //currentPayload->currentTime = sample.second.time;
        //currentPayload->duration = sample.second.duration;
        payloads_.push_back( payload );
    }
    if ( fileRead.is_open() ) fileRead.close();
    delete[] buffer;
/*
    DEVC *currentPayload = nullptr;
    for ( auto sample : GPMFtrack->samples ) {
        auto payload = klv::makeKlv_(sample.second.filePath, sample.second.filePos);
        currentPayload = (DEVC *) payload.get();
        currentPayload->timeScale = GPMFtrack->mediaTimeScale;
        currentPayload->currentTime = sample.second.time;
        currentPayload->duration = sample.second.duration;
        payloads_.push_back( payload );
    }
*/
    #ifdef GPMF_PARSE_TIME
    auto end = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "\nGPMF Time : " << ms_int.count() << "ms\n";
    #endif
}

std::vector<GPMF::sampleType> GPMF::GPMF::getAcceleration()
{
    DEVC *currentPayload = nullptr;
    sampleEntryType sampleEntry;
    std::vector<sampleType> accelerations;
    for ( auto payload : payloads_ ) {
        currentPayload = (DEVC *) payload.get();
        ACCL *acclFound = nullptr;
        SCAL *scalFound = nullptr;
        SIUN *siunFound = nullptr;
        STMP *stmpFound = nullptr;
        for ( auto strm : payload->getTypeKlvs<STRM>() ) {
            for ( auto accl : strm->getTypeKlvs<ACCL>()) {
                acclFound = accl;
                for ( auto scal : strm->getTypeKlvs<SCAL>())
                    scalFound = scal;
                for ( auto siun : strm->getTypeKlvs<SIUN>())
                    siunFound = siun;
                for ( auto stmp : strm->getTypeKlvs<STMP>())
                    stmpFound = stmp;
            }
        }
        if ( acclFound == nullptr || scalFound == nullptr || siunFound == nullptr )
            continue;
        float sampleDeltaTimeF = (float) currentPayload->duration/acclFound->samples.size();
        float sampleDuration = sampleDeltaTimeF / currentPayload->timeScale;
        float sampleTimeF = (float) currentPayload->currentTime;
        for ( auto sample : acclFound->samples ) {
            sampleType acceleration;

            sampleEntry.value = sampleTimeF / currentPayload->timeScale;
            sampleEntry.unit = "sec";
            sampleEntry.info = "current time";
            acceleration.time = sampleEntry;

            sampleEntry.value = sampleDuration;
            sampleEntry.unit = "sec";
            sampleEntry.info = "duration";
            acceleration.duration = sampleEntry;

            sampleEntry.value = (float) sample[0] / scalFound->denominators[0];
            sampleEntry.unit = siunFound->units[0];
            sampleEntry.info = "up/down";
            acceleration.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[1] / scalFound->denominators[0];
            sampleEntry.unit = siunFound->units[0];
            sampleEntry.info = "right/left";
            acceleration.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[2] / scalFound->denominators[0];
            sampleEntry.unit = siunFound->units[0];
            sampleEntry.info = "forward/back";
            acceleration.entries.push_back(sampleEntry);

            acceleration.timeStamp = 0;
            if ( stmpFound != nullptr ) acceleration.timeStamp = stmpFound->timeStamp;

            accelerations.push_back(acceleration);

            sampleTimeF += sampleDeltaTimeF;
        }
    }
    return accelerations;
}

std::vector<GPMF::sampleType> GPMF::GPMF::getGyroscope()
{
    DEVC *currentPayload = nullptr;
    sampleEntryType sampleEntry;
    std::vector<sampleType> gyroscopes;
    for ( auto payload : payloads_ ) {
        currentPayload = (DEVC *) payload.get();
        GYRO *gyroFound = nullptr;
        SCAL *scalFound = nullptr;
        SIUN *siunFound = nullptr;
        STMP *stmpFound = nullptr;
        for ( auto strm : payload->getTypeKlvs<STRM>() ) {
            for ( auto gyro : strm->getTypeKlvs<GYRO>()) {
                gyroFound = gyro;
                for ( auto scal : strm->getTypeKlvs<SCAL>())
                    scalFound = scal;
                for ( auto siun : strm->getTypeKlvs<SIUN>())
                    siunFound = siun;
                for ( auto stmp : strm->getTypeKlvs<STMP>())
                    stmpFound = stmp;
            }
        }
        if ( gyroFound == nullptr || scalFound == nullptr || siunFound == nullptr )
            continue;
        float sampleDeltaTimeF = (float) currentPayload->duration/gyroFound->samples.size();
        float sampleDuration = sampleDeltaTimeF / currentPayload->timeScale;
        float sampleTimeF = (float) currentPayload->currentTime;
        for ( auto sample : gyroFound->samples ) {
            sampleType gyroscope;

            sampleEntry.value = sampleTimeF / currentPayload->timeScale;
            sampleEntry.unit = "sec";
            sampleEntry.info = "current time";
            gyroscope.time = sampleEntry;

            sampleEntry.value = sampleDuration;
            sampleEntry.unit = "sec";
            sampleEntry.info = "duration";
            gyroscope.duration = sampleEntry;

            sampleEntry.value = (float) sample[0] / scalFound->denominators[0];
            sampleEntry.unit = siunFound->units[0];
            sampleEntry.info = "z rotation";
            gyroscope.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[1] / scalFound->denominators[0];
            sampleEntry.unit = siunFound->units[0];
            sampleEntry.info = "x rotation";
            gyroscope.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[2] / scalFound->denominators[0];
            sampleEntry.unit = siunFound->units[0];
            sampleEntry.info = "y rotation";
            gyroscope.entries.push_back(sampleEntry);

            gyroscope.timeStamp = 0;
            if ( stmpFound != nullptr ) gyroscope.timeStamp = stmpFound->timeStamp;
            
            gyroscopes.push_back(gyroscope);

            sampleTimeF += sampleDeltaTimeF;
        }
    }
    return gyroscopes;
}

std::vector<GPMF::sampleType> GPMF::GPMF::getGPS()
{
    DEVC *currentPayload = nullptr;
    sampleEntryType sampleEntry;
    std::vector<sampleType> GPSs;
    for ( auto payload : payloads_ ) {
        currentPayload = (DEVC *) payload.get();
        GPS5 *gps5Found = nullptr;
        SCAL *scalFound = nullptr;
        UNIT *unitFound = nullptr;
        GPSF *gpsfFound = nullptr;
        for ( auto strm : payload->getTypeKlvs<STRM>() ) {
            for ( auto gps5 : strm->getTypeKlvs<GPS5>()) {
                gps5Found = gps5;
                for ( auto scal : strm->getTypeKlvs<SCAL>())
                    scalFound = scal;
                for ( auto unit : strm->getTypeKlvs<UNIT>())
                    unitFound = unit;
                for ( auto gpsf : strm->getTypeKlvs<GPSF>())
                    gpsfFound = gpsf;
            }
        }
        if ( gps5Found == nullptr || scalFound == nullptr || unitFound == nullptr )
            continue;
        if ( gpsfFound->gpsFix == 0 ) continue;
        float sampleDeltaTimeF = (float) currentPayload->duration/gps5Found->samples.size();
        float sampleDuration = sampleDeltaTimeF / currentPayload->timeScale;
        float sampleTimeF = (float) currentPayload->currentTime;
        for ( auto sample : gps5Found->samples ) {
            sampleType GPS;

            sampleEntry.value = sampleTimeF / currentPayload->timeScale;
            sampleEntry.unit = "sec";
            sampleEntry.info = "current time";
            GPS.time = sampleEntry;

            sampleEntry.value = sampleDuration;
            sampleEntry.unit = "sec";
            sampleEntry.info = "duration";
            GPS.duration = sampleEntry;

            sampleEntry.value = (float) sample[0] / scalFound->denominators[0];
            sampleEntry.unit = unitFound->units[0];
            sampleEntry.info = "latitude";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[1] / scalFound->denominators[1];
            sampleEntry.unit = unitFound->units[1];
            sampleEntry.info = "longitude";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[2] / scalFound->denominators[2];
            sampleEntry.unit = unitFound->units[2];
            sampleEntry.info = "altitude";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[3] / scalFound->denominators[3];
            sampleEntry.unit = unitFound->units[3];
            sampleEntry.info = "2D speed";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[4] / scalFound->denominators[4];
            sampleEntry.unit = unitFound->units[4];
            sampleEntry.info = "3D speed";
            GPS.entries.push_back(sampleEntry);
            
            GPSs.push_back(GPS);

            sampleTimeF += sampleDeltaTimeF;
        }
    }
    return GPSs;
}

void GPMF::GPMF::exportGPStoGPX(std::string fileName)
{
    std::ofstream out(fileName+".gpx", std::ios::binary);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<gpx>\n\t<trk>\n\t\t<trkseg>\n";
    auto sampels = getGPS();
    int fixedWith = 10;
    float fracseconds;
    int seconds, minutes, hours;
    for ( auto sampel : sampels ) {
        seconds = (int) sampel.time.value;
        fracseconds = sampel.time.value - (float) seconds;
        minutes = seconds / 60;
        hours = minutes / 60;
        seconds %= 60;
        minutes %= 60;
        hours %= 24;
        fracseconds += seconds;
        //out << std::setprecision(9);
        out << std::fixed << std::setprecision(7);
        out << "\t\t\t<trkpt lat=\"" << sampel.entries[0].value;
        out << "\" lon=\"" << sampel.entries[1].value << "\">";
        out << "<ele>" << sampel.entries[2].value << "</ele>";
        out << "<time>2007-01-01T" << std::setfill('0')
        << std::setw(2) << hours << ":"
        << std::setw(2) << minutes << ":" 
        << std::setprecision(3) << std::setw(6) << fracseconds << "Z</time></trkpt>\n";
    }
    out << "\t\t</trkseg>\n\t</trk>\n</gpx>\n";
    out.close();
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
    
    if ( udtaPayload_ != nullptr ) {
        std::cout << std::endl
                << std::string(15, '-') << " GLOBAL " << index << " " << std::string(15, '-') << std::endl;
        udtaPayload_->printHierarchyData();
    }

    for ( auto payload : payloads_ ) {
        std::cout << std::endl
            << std::string(15, '-') << " PAYLOAD " << index << " " << std::string(15, '-') << std::endl;
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

void GPMF::GPMF::error_(std::string message)
{
    std::cout << "GPMF::GPMF:\n";
    std::cout << "-> " << message << std::endl;
    std::cout << "exit application ..." << std::endl;
    exit(1);
}