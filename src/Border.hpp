
#pragma once

class Border{
public:
    void draw(){
        ofSetColor(255, 100, 50, 30);
        ofNoFill();
        ofSetLineWidth(5);
        ofPushMatrix();
        ofTranslate(0,0,0);
        ofRotate(90,1,0,0);
        ofDrawCircle(0, 0, AREA_RADIUS);
        ofPopMatrix();
        ofFill();
    }
};
