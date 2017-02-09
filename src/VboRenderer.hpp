#pragma once

#include "ofMain.h"

class VboRenderer{

public:
    VboRenderer(std::vector<ofVec3f> vertices){
        vbo.setVertexData(&vertices[0], vertices.size(), GL_STATIC_DRAW);
    }
    
    void draw() const{
        vbo.draw(GL_LINE_STRIP, 0, vbo.getNumVertices());
    };

private:
    ofVbo vbo;
};
