#pragma once

#include "ofMain.h"
#include "Const.hpp"

class FontServer{
    
public:
    static FontServer * getSingleton(){
        if(!theInstance){
            theInstance = new FontServer;
            theInstance->initFont();
        }
        return theInstance;
    };
    
    void drawText(const ofVec2f &pos, const std::string &str) const{
        font.drawString(str, pos.x, pos.y);
    }
    
private:
    FontServer(){}; // cannot instantiate by constructor
    void initFont(){
        font.load(FONT, 8);
    }

    ofTrueTypeFont font;
    static FontServer * theInstance;
};

