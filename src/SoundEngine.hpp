#pragma once

#include "ofxPd.h"
#include "ofMain.h"
#include "Const.hpp"
#include "SoundEvent.hpp"

class SoundEngine : public pd::PdReceiver{
    
public:
    void setup();
    void audioOut(float *output, int bufferSize, int nChannels);
    
    void startPlayback(int sfID);
    void stopPlayback(int sfID);
    void setVolume(int sfID, float volume);
    void setAngle(int sfID, float angle);
    void print(const std::string &mes);
    void handleEvent(SoundEvent &event);
    void updateLevels();
    const std::vector<float> &getAmplitude() const{ return amplitude;}
    
private:
    ofxPd pd;
    std::vector<float> amplitude;
};


inline void SoundEngine::setup(){
    if(!pd.init(NUM_SPEAKERS, 0, SAMPLE_RATE, TICKS_PER_BUFFER, false)) {
        OF_EXIT_APP(1);
    }
    
    pd.subscribe("toOF");
    pd.addReceiver(*this);
    pd::Patch patch = pd.openPatch(PD_PATCH);
    
    ofAddListener(SoundEvent::events, this, &SoundEngine::handleEvent);
    amplitude.resize(NUM_PLAYERS);
}

inline void SoundEngine::updateLevels(){
    pd.readArray("amplitude", amplitude);
}


inline void SoundEngine::handleEvent(SoundEvent &event){
    SoundEventType type = event.getType();
    switch(type){
        case SoundEventType::PLAY:{
            startPlayback(event.getSoundFileID());
            break;
        }
        case SoundEventType::STOP:{
            stopPlayback(event.getSoundFileID());
            break;
        }
        case SoundEventType::VOLUME:{
            setVolume(event.getSoundFileID(), event.getParameter());
            break;
        }
        case SoundEventType::ANGLE:{
            setAngle(event.getSoundFileID(), event.getParameter());
            break;
        }
        default:{
            ofLog(OF_LOG_ERROR) << "Invalid type";
        }
    }
    
}

inline void SoundEngine::audioOut(float *output, int bufferSize, int nChannels){
    pd.audioOut(output, bufferSize, nChannels);
}

inline void SoundEngine::startPlayback(int sfID){
    pd.startMessage();
    pd.addFloat(static_cast<float>(sfID));
    pd.addSymbol("start");
    pd.finishList(PD_RECEIVE_NAME);
}

inline void SoundEngine::stopPlayback(int sfID){
    pd.startMessage();
    pd.addFloat(static_cast<float>(sfID));
    pd.addSymbol("stop");
    pd.finishList(PD_RECEIVE_NAME);
}

inline void SoundEngine::print(const std::string &mes){
    ofLog() << mes;
}

inline void SoundEngine::setVolume(int sfID, float volume){
    pd.startMessage();
    pd.addFloat(static_cast<float>(sfID));
    pd.addSymbol("vol");
    pd.addFloat(volume);
    pd.finishList(PD_RECEIVE_NAME);
}

inline void SoundEngine::setAngle(int sfID, float angle){
    pd.startMessage();
    pd.addFloat(static_cast<float>(sfID));
    pd.addSymbol("angle");
    pd.addFloat(angle);
    pd.finishList(PD_RECEIVE_NAME);
}
