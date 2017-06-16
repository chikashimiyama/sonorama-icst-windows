#pragma once

#include "ofMain.h"
#include "Const.hpp"
#include "Camera.hpp"
#include "StylizerFactory.hpp"


class VboRenderer{

public:
    VboRenderer(const std::string &styleName):
    stylizer(StylizerFactory::getStylizer(styleName)){}
    
    void draw(int drawMode,int first, int total) const{
        stylizer.stylize();
        vbo.draw(drawMode, first, total);
    }
    
    void drawElement(int drawMode) const{
        stylizer.stylize();
        vbo.drawElements(drawMode, vbo.getNumIndices());
    }
    

    void setup(std::vector<ofVec3f> vertices, std::vector<ofIndexType> indices){
        vbo.setVertexData(&vertices[0], vertices.size(), GL_STATIC_DRAW);
        vbo.setIndexData(&indices[0],  indices.size(), GL_STATIC_DRAW);
    }
    

    
protected:
    ofVbo vbo;
    Stylizer stylizer;
    
};



