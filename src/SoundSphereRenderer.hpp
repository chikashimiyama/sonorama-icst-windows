#pragma once

#include "VboRenderer.hpp"


class SoundSphereRenderer : public VboRenderer{
public:
    
    SoundSphereRenderer(std::vector<ofVec3f> baseVertices, const std::unordered_map<std::string, std::string> &tags):
    VboRenderer(baseVertices),
    centerPos(baseVertices[0]){}
    
    void draw() const override{
        ofSetColor(ofColor(255,125, 125, 255));
        ofDrawSphere(centerPos, 1);
        ofSetColor(ofColor(255,125, 125, 100));
        ofDrawSphere(centerPos, 30);
    }
    
    void label(const Camera &camera, const std::string &text) const override{
        ofVec2f posOnScreen = camera.worldToScreen(centerPos, ofGetCurrentViewport());
        ofVec2f textOnScreen = posOnScreen;
        textOnScreen.y -= 200.0;
        
        if(!(
             (0 < textOnScreen.x && textOnScreen.x < WIDTH) &&
             (0 < textOnScreen.y && textOnScreen.y < HEIGHT)
             )
           ){return;}
        
        ofVec2f tipOfLine = textOnScreen;
        tipOfLine.y += 10;
        
        ofSetColor(ofColor(255, 125, 125, 255));
        ofDrawLine(posOnScreen, tipOfLine);
        
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(textOnScreen, text);
        
    }
    
private:
    ofVec3f centerPos;
};
