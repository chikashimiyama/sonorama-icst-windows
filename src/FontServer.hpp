#pragma once

#include "ofMain.h"

class FontServer{
    
public:
    static FontServer * getSingleton(){
        if(!theInstance){
            theInstance = new FontServer;
            theInstance->initFont();
        }
        return theInstance;
    };
    
    void drawText(ofVec3f pos, std::string str) const{
        ofPushMatrix();
        ofTranslate(pos);
        font.drawString(str,0,-100);
        ofPopMatrix();
    }
    
private:
    FontServer(){}; // cannot instantiate by constructor
    void initFont(){
        font.load("zurich.ttf", 7);
    }

    ofTrueTypeFont font;
    static FontServer * theInstance;
};

