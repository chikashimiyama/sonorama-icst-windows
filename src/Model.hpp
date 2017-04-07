#pragma once

#include <unordered_map>
#include "Node.hpp"
#include "VboRenderer.hpp"
#include "Bounds.hpp"
#include "Camera.hpp"
#include "ofMain.h"

class Model : public TagController {
public:
 
    Model(SInt64 id, std::vector<ofVec3f> vertices, std::unordered_map<std::string, std::string> tags):
    TagController(tags),
    id(id),
    visible(true),
    position(calcAverage(vertices)){}
    

    SInt64 getId() const{
        return id;
    }
    
    ofVec3f getPosition() const {
        return position;
    }
    
    void label(const Camera &camera) const{
        
    }
    
    
private:
    ofVec3f calcAverage(std::vector<ofVec3f>  &vertices){
        ofVec3f p;
        for(ofVec3f vec : vertices){
            p += vec;
        }
        return p / static_cast<float>(vertices.size());
    }
    
    const UInt64 id;
    ofVec3f position;
    bool visible;
    
};

