#pragma once

#include "ofMain.h"
#include "Const.hpp"
#include "Camera.hpp"

class VboRenderer{

public:
    VboRenderer(std::vector<ofVec3f> vertices){
        baseVbo.setVertexData(&vertices[0], vertices.size(), GL_STATIC_DRAW);
    }
    
    virtual void draw() const {
        baseVbo.draw(GL_LINE_STRIP, 0, baseVbo.getNumVertices());
    }
    
    virtual void label(const Camera &camera, const std::string &text) const{};
    
protected:
    ofVbo baseVbo;
    
    bool isInScreen(ofVec2f point) const{
        bool xOK = 0 < point.x && point.x < WIDTH;
        bool yOK = 0 < point.y && point.y < HEIGHT;
        return xOK && yOK;
    }
    
};



