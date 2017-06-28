#pragma once

#include "ofMain.h"
#include "Const.hpp"
#include "FontServer.hpp"

class CardinalDirections{
public:

    
    const std::string direction = "ENWS";
    void label(){
        ofSetColor(TOUCH_READY_COLOR);
        FontServer *fontServer = FontServer::getSingleton();
        float offset = WIDTH / 2.0;
        for(int i = 0; i < 4; i++){
            fontServer->drawText(ofVec2f(offset, 20), direction.substr(i,1));
            offset+=WIDTH;
        }
    }
};
