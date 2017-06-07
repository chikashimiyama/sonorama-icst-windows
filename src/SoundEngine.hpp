#pragma once

#include "ofxPd.h"
#include "ofMain.h"
#include "Const.hpp"

class SoundEngine : public pd::PdReceiver{
    
public:
    void setup();
    void audioOut(float *output, int bufferSize, int nChannels);
    
    void startPlayback(int sfID);
    void stopPlayback(int sfID);
    void setVolume(int sfID, float volume);
    void setAngle(int sfID, float angle);
    
private:
    ofxPd pd;
};


inline void SoundEngine::setup(){
    if(!pd.init(2, 0, 44100, TICKS_PER_BUFFER, false)) {
        OF_EXIT_APP(1);
    }
    
    pd.subscribe("toOF");
    pd.addReceiver(*this);
    pd::Patch patch = pd.openPatch(PD_PATCH);
    
}

inline void SoundEngine::audioOut(float *output, int bufferSize, int nChannels){
    pd.audioOut(output, bufferSize, nChannels);
}

inline void SoundEngine::startPlayback(int sfID){
    pd.startMessage();
    pd.addSymbol("start");
    pd.addFloat(static_cast<float>(sfID));
    pd.finishList("fromOF");
}

inline void SoundEngine::stopPlayback(int sfID){
    pd.startMessage();
    pd.addSymbol("stop");
    pd.addFloat(static_cast<float>(sfID));
    pd.finishList("fromOF");
}

inline void SoundEngine::setVolume(int sfID, float volume){
    pd.startMessage();
    pd.addSymbol("vol");
    pd.addFloat(static_cast<float>(sfID));
    pd.addFloat(volume);
    pd.finishList("fromOF");
}

inline void SoundEngine::setAngle(int sfID, float angle){
    pd.startMessage();
    pd.addSymbol("angle");
    pd.addFloat(static_cast<float>(sfID));
    pd.addFloat(angle);
    pd.finishList("fromOF");
}
