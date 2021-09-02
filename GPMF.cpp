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
    
    auto samples = GPMFtrack->getSamples();
    DEVC *currentPayload = nullptr;
    for ( auto chunk : GPMFtrack->getChunks() ) {
        #ifdef GPMF_PARSE_PATH
        std::cout << "PAYLOAD: " << chunk.ID << std::endl;
        #endif
        auto payload = klv::makeKlv_(mp4_->filePath, chunk.dataOffset);
        currentPayload = (DEVC *) payload.get();
        for ( auto sample : samples ) {
            if ( sample.ID == chunk.currentSampleID ) {
                currentPayload->timeScale = GPMFtrack->getMediaTimeScale();
                currentPayload->currentTime = sample.currentTime;
                currentPayload->duration = sample.duration;
            }
        }
        payloads_.push_back( payload );
    }
}

std::vector<GPMF::sampleType> GPMF::GPMF::getAcceleration()
{
    int index = 1;
    DEVC *currentPayload = nullptr;
    sampleEntryType sampleEntry;
    std::vector<sampleType> accelerations;
    for ( auto payload : payloads_ ) {
        currentPayload = (DEVC *) payload.get();
        STRM *stream;
        ACCL *accel;
        for ( auto strm : getTypeKlvs<STRM>(payload.get()) )
            for ( auto accl : getTypeKlvs<ACCL>(strm) ) {
                stream = strm;
                accel = accl;
            }
        SCAL *scal = getTypeKlvs<SCAL>(stream)[0];
        SIUN *siun = getTypeKlvs<SIUN>(stream)[0];
        float sampleDeltaTimeF = (float) currentPayload->duration/accel->samples.size();
        float sampleDuration = sampleDeltaTimeF / currentPayload->timeScale;
        float sampleTimeF = (float) currentPayload->currentTime;
        for ( auto sample : accel->samples ) {
            sampleType acceleration;

            sampleEntry.value = sampleTimeF / currentPayload->timeScale;
            sampleEntry.unit = "sec";
            sampleEntry.info = "current time";
            acceleration.time = sampleEntry;

            sampleEntry.value = sampleDuration;
            sampleEntry.unit = "sec";
            sampleEntry.info = "duration";
            acceleration.duration = sampleEntry;

            sampleEntry.value = (float) sample[0] / scal->denominators[0];
            sampleEntry.unit = siun->units[0];
            sampleEntry.info = "up/down";
            acceleration.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[1] / scal->denominators[0];
            sampleEntry.unit = siun->units[0];
            sampleEntry.info = "right/left";
            acceleration.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[2] / scal->denominators[0];
            sampleEntry.unit = siun->units[0];
            sampleEntry.info = "forward/back";
            acceleration.entries.push_back(sampleEntry);
            
            accelerations.push_back(acceleration);

            sampleTimeF += sampleDeltaTimeF;
        }
        index++;
    }
    return accelerations;
}

std::vector<GPMF::sampleType> GPMF::GPMF::getGyroscope()
{
    int index = 1;
    DEVC *currentPayload = nullptr;
    sampleEntryType sampleEntry;
    std::vector<sampleType> gyroscopes;
    for ( auto payload : payloads_ ) {
        currentPayload = (DEVC *) payload.get();
        STRM *stream;
        GYRO *gyrosc;
        for ( auto strm : getTypeKlvs<STRM>(payload.get()) )
            for ( auto gyro : getTypeKlvs<GYRO>(strm) ) {
                stream = strm;
                gyrosc = gyro;
            }
        SCAL *scal = getTypeKlvs<SCAL>(stream)[0];
        SIUN *siun = getTypeKlvs<SIUN>(stream)[0];
        float sampleDeltaTimeF = (float) currentPayload->duration/gyrosc->samples.size();
        float sampleDuration = sampleDeltaTimeF / currentPayload->timeScale;
        float sampleTimeF = (float) currentPayload->currentTime;
        for ( auto sample : gyrosc->samples ) {
            sampleType gyroscope;

            sampleEntry.value = sampleTimeF / currentPayload->timeScale;
            sampleEntry.unit = "sec";
            sampleEntry.info = "current time";
            gyroscope.time = sampleEntry;

            sampleEntry.value = sampleDuration;
            sampleEntry.unit = "sec";
            sampleEntry.info = "duration";
            gyroscope.duration = sampleEntry;

            sampleEntry.value = (float) sample[0] / scal->denominators[0];
            sampleEntry.unit = siun->units[0];
            sampleEntry.info = "z rotation";
            gyroscope.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[1] / scal->denominators[0];
            sampleEntry.unit = siun->units[0];
            sampleEntry.info = "x rotation";
            gyroscope.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[2] / scal->denominators[0];
            sampleEntry.unit = siun->units[0];
            sampleEntry.info = "y rotation";
            gyroscope.entries.push_back(sampleEntry);
            
            gyroscopes.push_back(gyroscope);

            sampleTimeF += sampleDeltaTimeF;
        }
        index++;
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
        STRM *stream = nullptr;
        GPS5 *gps5c = nullptr;
        for ( auto strm : getTypeKlvs<STRM>(payload.get()) )
            for ( auto gps5 : getTypeKlvs<GPS5>(strm) ) {
                stream = strm;
                gps5c = gps5;
            }
        if ( stream == nullptr ) continue;
        SCAL *scal = getTypeKlvs<SCAL>(stream)[0];
        UNIT *unit = getTypeKlvs<UNIT>(stream)[0];
        GPSF *lock = getTypeKlvs<GPSF>(stream)[0];
        if ( lock->gpsFix == 0 ) continue;
        float sampleDeltaTimeF = (float) currentPayload->duration/gps5c->samples.size();
        float sampleDuration = sampleDeltaTimeF / currentPayload->timeScale;
        float sampleTimeF = (float) currentPayload->currentTime;
        for ( auto sample : gps5c->samples ) {
            sampleType GPS;
            
            sampleEntry.value = sampleTimeF / currentPayload->timeScale;
            sampleEntry.unit = "sec";
            sampleEntry.info = "current time";
            GPS.time = sampleEntry;

            sampleEntry.value = sampleDuration;
            sampleEntry.unit = "sec";
            sampleEntry.info = "duration";
            GPS.duration = sampleEntry;

            sampleEntry.value = (float) sample[0] / scal->denominators[0];
            sampleEntry.unit = unit->units[0];
            sampleEntry.info = "latitude";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[1] / scal->denominators[1];
            sampleEntry.unit = unit->units[1];
            sampleEntry.info = "longitude";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[2] / scal->denominators[2];
            sampleEntry.unit = unit->units[2];
            sampleEntry.info = "altitude";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[3] / scal->denominators[3];
            sampleEntry.unit = unit->units[3];
            sampleEntry.info = "2D speed";
            GPS.entries.push_back(sampleEntry);

            sampleEntry.value = (float) sample[4] / scal->denominators[4];
            sampleEntry.unit = unit->units[4];
            sampleEntry.info = "3D speed";
            GPS.entries.push_back(sampleEntry);

            GPSs.push_back(GPS);

            sampleTimeF += sampleDeltaTimeF;
        }
    }
    return GPSs;
}

std::vector<std::shared_ptr<GPMF::klv>> GPMF::GPMF::getKlvs(std::string findKey, klv *parent)
{
    std::vector<std::shared_ptr<klv>> found;
    if ( parent != nullptr ) parent->getKlvs_(findKey, found);
    else for ( auto payload : payloads_ ) {
        if ( payload->key == findKey ) found.push_back(payload);
        payload->getKlvs_(findKey, found);
    }
    return found;
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
