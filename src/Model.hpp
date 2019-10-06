#pragma once

#include <unordered_map>
#include "Node.hpp"
#include "VboRenderer.hpp"
#include "Bounds.hpp"
#include "Camera.hpp"
#include "ofMain.h"

/* 
    this class represents the position and optionally shape of POI
    The way of drawing POI should be externally defined by a functor (default = nullptr).
 */

class Model : public TagController {
public:
 
    Model(long long id, std::vector<ofVec3f> vertices, std::unordered_map<std::string, std::string> tags):
    TagController(tags),
    id(id),
    visible(true),
    position(calcAverage(vertices)){}
    

	long long getId() const{
        return id;
    }
    
    ofVec3f getPosition() const {
        return position;
    }
    
    virtual void draw() const{
    }
    
    virtual void label(const Camera &camera) const{
        if(name.size() == 0 )return;
        
        ofRectangle viewPort = ofRectangle(camera.getCameraID() * WIDTH, 0, WIDTH, HEIGHT);
        ofVec2f posOnScreen = camera.worldToScreen(position, viewPort);
        if(!viewPort.inside(posOnScreen))return;
      
        ofDrawCircle(posOnScreen.x, posOnScreen.y, BULLET_SIZE);
        posOnScreen.x += LABEL_INDENT;
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(posOnScreen, name);
    }
    
protected:
    const long long id;
    ofVec3f position;
    bool visible;

private:
    ofVec3f calcAverage(std::vector<ofVec3f>  &vertices){
        ofVec3f p;
        for(ofVec3f vec : vertices){
            p += vec;
        }
        return p / static_cast<float>(vertices.size());
    }
};

