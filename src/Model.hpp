#pragma once

#include <unordered_map>
#include "Node.hpp"
#include "VboRenderer.hpp"
#include "Bounds.hpp"
#include "Camera.hpp"
#include "ofMain.h"

template <typename R>
class Model : public TagController {
public:
 
    Model(SInt64 id, std::vector<ofVec3f> vertices, std::unordered_map<std::string, std::string> tags):
    TagController(tags),
    id(id),
    renderer(vertices, tags),
    visible(true){
        ofVec3f p;
        for(ofVec3f vec : vertices){
            p += vec;
        }
        position = p / static_cast<float>(vertices.size());
    }
    
    SInt64 getId() const{
        return id;
    }

    virtual void draw() const{
        renderer.draw();
    }
    
    ofVec3f getPosition() const{
        return position;
    }
    
    void label(const Camera &camera) const{
        renderer.label(camera, name);
    }
    
private:
    const UInt64 id;
    R renderer;
    ofVec3f position;
    bool visible;
};

