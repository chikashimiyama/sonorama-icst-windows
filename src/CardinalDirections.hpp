#pragma once

#include "ofMain.h"
#include "Const.hpp"
#include "FontServer.hpp"

class CardinalDirections{
public:

    
	const std::array<std::string,4>  directions = {{"South", "West", "North", "East"}};
    void label(){
        ofSetColor(255);
        FontServer *fontServer = FontServer::getSingleton();
        float offset = 5;
        for(int i = 0; i < 4; i++){
            fontServer->drawText(ofVec2f(offset, 20), directions[i]);
            offset+=WIDTH;
        }
    }
};
