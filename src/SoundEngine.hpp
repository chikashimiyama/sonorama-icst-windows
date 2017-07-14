#pragma once

#include "ofxPd.h"
#include "ofMain.h"
#include "Const.hpp"
#include "SoundEvent.hpp"

class SoundEngine : public pd::PdReceiver{
    
public:
	SoundEngine():numActivePlayers(0){}
	
    void setup(const std::string &cityName);
    void audioOut(float *output, int bufferSize, int nChannels);
    
    void print(const std::string &mes);
    void handleEvent(SoundEvent &event);
    void update();
    const std::vector<float> &getAmplitude() const{ return amplitude;}
	
	size_t getNumberOfActivePlayers() const noexcept{return numActivePlayers;}
private:
    void startPlayback(int sfID);
    void stopPlayback(int sfID);

    void setVolume(int sfID, float vol);
    void setAngle(int sfID, float ang);

    void updateLevels();
    void updatePositions();
    
    void setCity(const std::string &cityname);

    ofxPd pd;
    
    // vector to be shared with Pd
    std::vector<float> amplitude;
    std::vector<float> angle;
    std::vector<float> volume;
	
	size_t numActivePlayers;
};



inline void SoundEngine::setup(const std::string &cityName){
    if(!pd.init(NUM_SPEAKERS, 0, SAMPLE_RATE, TICKS_PER_BUFFER, false)) {
        OF_EXIT_APP(1);
    }
    
    pd.subscribe("toOF");
    pd.addReceiver(*this);
    pd::Patch patch = pd.openPatch(PD_PATCH);
    
    ofAddListener(SoundEvent::events, this, &SoundEngine::handleEvent);
    amplitude.resize(NUM_PLAYERS);
    angle.resize(NUM_PLAYERS);
    volume.resize(NUM_PLAYERS);
    
    setCity(cityName);
    
}

inline void SoundEngine::update(){
    updateLevels();
    updatePositions();
    
    pd.sendMessage(PD_RECEIVE_NAME, "update");
    
}

inline void SoundEngine::updateLevels(){
    pd.readArray("amplitude", amplitude);
}

inline void SoundEngine::updatePositions(){
    pd.writeArray("angle", angle);
    pd.writeArray("volume", volume);
}

inline void SoundEngine::handleEvent(SoundEvent &event){
    SoundEventType type = event.getType();
    switch(type){
        case SoundEventType::PLAY:{
            startPlayback(event.getSoundFileID());
			numActivePlayers++;
            break;
        }
        case SoundEventType::STOP:{
            stopPlayback(event.getSoundFileID());
			numActivePlayers--;
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
    ofLog() << "Pd message:" << mes;
}

inline void SoundEngine::setCity(const std::string &cityname){
    pd.startMessage();
    pd.addSymbol(cityname);
    pd.finishMessage(PD_RECEIVE_NAME, "cityname");
}

inline void SoundEngine::setVolume(int sfID, float vol){
    volume[sfID] = vol;
}

inline void SoundEngine::setAngle(int sfID, float ang){
    angle[sfID] = ang;
}
