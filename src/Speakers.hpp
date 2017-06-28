#pragma once
#include "ofMain.h"

class Speakers{
public:
    void draw(const ofVec3f &position){
        ofSetColor(125,125,255,125);
        ofVec3f pos = position;
        pos.y = 300;
        ofPushMatrix();
        ofTranslate(pos);
        for(int i = 0; i < NUM_SPEAKERS; i++){
            ofPushMatrix();
            ofRotateY(-22.5 + i * 45);
            ofDrawBox(0,0,300 ,20,20,20);
            ofPopMatrix();

        }
        ofPopMatrix();
    }
};
