#pragma once

#include "ofMain.h"

class Stylizer{
public:
    Stylizer(ofColor color, float width):
    color(color),
    width(width){}
    
    void stylize() const{
        ofSetLineWidth(width);
        ofSetColor(color);
    }
    
protected:
    const ofColor color;
    const float width;
};
