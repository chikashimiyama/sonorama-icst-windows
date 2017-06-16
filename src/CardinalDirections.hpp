#pragma once

#include "ofMain.h"
#include "FontServer.hpp"

class CardinalDirections{
public:
    void draw(const ofVec3f &position){
        ofSetLineWidth(1);
        ofSetColor(255,0,0,255);
        ofVec3f pos = position;
        pos.y = 0;
        ofPushMatrix();
        ofTranslate(pos);
        for(int i = 0; i < 4;i++){
            ofRotateY(i * 45);
            ofDrawLine(-10000, 0, 10000 , 0);
        }
        ofPopMatrix();
        
    }
    
    void label(){
        
        
    }
    
};
