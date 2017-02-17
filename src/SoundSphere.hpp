#pragma once
#include "ofMain.h"

class SoundSphere{
  
public:
    SoundSphere():id(0),name("unknown"),position(ofVec3f()){}
    
    SoundSphere(UInt32 id, std::string name, ofVec3f position):
    id(id),
    name(name),
    position(position){
        
        radius = 5;
    }
    
    
    void setRadius(float r){
        radius = r;
    }
    
    void draw() const{
        ofSetColor(ofColor(255,125, 125, 255));
        ofDrawSphere(position, 1);

        ofSetColor(ofColor(255,125, 125, 100));
        ofDrawSphere(position, radius);
    }
    
    void label(ofEasyCam &cam){
        
    }
    
protected:
    
    const int id;
    const std::string name;
    const ofVec3f position;
    float radius;

};


