#pragma once

#include <unordered_map>
#include "Node.hpp"
#include "VboRenderer.hpp"
#include "Bounds.hpp"
#include "ofMain.h"

template <typename R>
class Model : public TagController {
public:
 
    Model(SInt64 id, std::vector<ofVec3f> vertices, std::unordered_map<std::string, std::string> tags):
    id(id),
    renderer(vertices, tags),
    TagController(tags),
    position(vertices[0]),
    visible(true){}
    
    SInt64 getId() const{
        return id;
    }


    void draw(){
        renderer.draw();
    }
    
    ofVec3f getPosition() const{
        return position;
    }
    
    void label(ofEasyCam &camera) const{
        renderer.label(camera, name);
    }
    
private:
    const UInt64 id;
    R renderer;
    ofVec3f position;
    bool visible;
};

