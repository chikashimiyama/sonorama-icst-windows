#pragma once

#include "ofMain.h"
#include "Const.hpp"
#include "Camera.hpp"

class VboRenderer{

public:
    VboRenderer(std::vector<ofVec3f> vertices, const std::unordered_map<std::string, std::string> &tags){
        baseVbo.setVertexData(&vertices[0], vertices.size(), GL_STATIC_DRAW);
    }
    
    void draw() const{
        baseVbo.draw(GL_LINE_STRIP, 0, baseVbo.getNumVertices());
    };

protected:
    ofVbo baseVbo;
};



