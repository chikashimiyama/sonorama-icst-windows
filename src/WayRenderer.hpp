#pragma once

#include "VboRenderer.hpp"

class WayRenderer : public VboRenderer{
    
public:
    
    WayRenderer(std::vector<ofVec3f> baseVertices, const std::unordered_map<std::string, std::string> &tags):
    VboRenderer(baseVertices){
        for(auto vertex : baseVertices){
            centerPos += vertex;
        }
        float divider = static_cast<float>(baseVertices.size());
        centerPos = ofVec3f(centerPos.x / divider, centerPos.y/divider, centerPos.z/divider );
    }
    
    virtual void label(const Camera &camera, const std::string &text) const override{
        ofVec2f posOnScreen = camera.worldToScreen(centerPos, ofGetCurrentViewport());
        if(!isInScreen(posOnScreen)) return;
        
        ofDrawCircle(posOnScreen.x, posOnScreen.y, BULLET_SIZE);
        posOnScreen.x += LABEL_INDENT;
        
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(posOnScreen, text);
 
    }
    
private:
    ofVec3f centerPos;
};
