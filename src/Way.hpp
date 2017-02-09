#pragma once

#include <unordered_map>
#include "Node.hpp"
#include "VboRenderer.hpp"
#include "Bounds.hpp"
#include "ofMain.h"

class Way : public TagController {
public:
 
    Way(SInt64 id, std::vector<ofVec3f> vertices, std::unordered_map<std::string, std::string> tags):
    id(id),
    vboRenderer(vertices),
    TagController(tags),
    edge(vertices[0]){}
    
    SInt64 getId() const{
        return id;
    }
    
    void draw(){
        vboRenderer.draw();
    }
    
    void writeLabel(){
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(edge, name);
    }
    
private:
    const UInt64 id;
    VboRenderer vboRenderer;
    ofVec3f edge;
 
};
