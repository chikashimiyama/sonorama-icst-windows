#pragma once

#include "VboRenderer.hpp"

class WayRenderer : public VboRenderer{
    
public:
    
    WayRenderer(std::vector<ofVec3f> baseVertices, const std::unordered_map<std::string, std::string> &tags):
    VboRenderer(baseVertices),
    vertices(baseVertices){}
    
    virtual void label(const Camera &camera, const std::string &text) const override{
        
        if(text.size() == 0 )return;

        ofVec3f position = camera.getPosition();
        
        float minDist = 1000000.0;
        ofVec3f closestVertex = ofVec3f(-100000,-100000,-100000);
        for(const ofVec3f &vertex : vertices){
            float distance = vertex.distance(position);
            if(minDist > distance){
                minDist = distance;
                closestVertex = vertex;
            }
        }
        if(minDist > STREET_LABEL_THRESHOLD) return;
        ofVec2f posOnScreen = camera.worldToScreen(closestVertex, ofGetCurrentViewport());
        if(!isInScreen(posOnScreen)) return;

        ofSetColor(ofColor::orange);
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(posOnScreen, text);
    }
    
private:
    std::vector<ofVec3f> vertices;
};
