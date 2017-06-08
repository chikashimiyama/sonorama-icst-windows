#pragma once

#include "ofMain.h"

enum class SoundEventType{ PLAY, STOP, VOLUME, ANGLE};

class SoundEvent : public ofEventArgs {
    
public:

    static ofEvent <SoundEvent> events;

    SoundEvent(SoundEventType tp, int sfID, float param = 0.0):
    type(tp),
    soundFileID(sfID),
    parameter(param)
    {}

    SoundEventType getType(){return type;}
    int getSoundFileID(){return soundFileID;}
    float getParameter(){return parameter;}
    
private:
    const SoundEventType type;
    const int soundFileID;
    const float parameter;
};
