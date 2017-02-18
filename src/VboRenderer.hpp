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



class BuildingRenderer: public VboRenderer{

public:
    BuildingRenderer(std::vector<ofVec3f> baseVertices, const std::unordered_map<std::string, std::string> &tags):
    VboRenderer(baseVertices, tags){
        std::vector<ofVec3f> ceilingVertices;
        std::vector<ofVec3f> wallVertices;
        
        int level = getLevels(tags);
        ofVec3f accum;
        for(auto &vertex : baseVertices){
            float height = level * 30.0;
            accum += vertex;
            ceilingVertices.emplace_back(vertex.x, vertex.y + height, vertex.z);
            wallVertices.emplace_back(vertex.x, vertex.y, vertex.z);
            wallVertices.emplace_back(vertex.x, vertex.y + height, vertex.z);
        }
        centerPos = accum / baseVertices.size();
        
        ceilingVbo.setVertexData(&ceilingVertices[0], ceilingVertices.size(), GL_STATIC_DRAW);
        wallVbo.setVertexData(&wallVertices[0], wallVertices.size(), GL_STATIC_DRAW);
    }
    
    void draw() const {
        baseVbo.draw(GL_POLYGON, 0, baseVbo.getNumVertices());
        ceilingVbo.draw(GL_POLYGON, 0, ceilingVbo.getNumVertices());
        wallVbo.draw(GL_QUAD_STRIP, 0, wallVbo.getNumVertices());
    }
    
    void label(const Camera &camera, const std::string &text) const{
        ofVec2f posOnScreen = camera.worldToScreen(centerPos, ofGetCurrentViewport());
        ofVec2f textOnScreen = posOnScreen;
        if(!(
             (0 < textOnScreen.x && textOnScreen.x < WIDTH) &&
             (0 < textOnScreen.y && textOnScreen.y < HEIGHT)
             )
           ){return;}
        
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(textOnScreen, text);
    }
    
protected:
    ofVbo ceilingVbo;
    ofVbo wallVbo;
    ofVec3f centerPos;
    
    int getLevels(const std::unordered_map<std::string, std::string> &tags) const{
        auto itr = tags.find("building:levels");
        if(itr != tags.end()){
            std::string level = itr->second;
            return ofToInt(level);
        }else{
            return 1;
        }
    }
};


class SoundSphereRenderer{
public:
    
    SoundSphereRenderer(std::vector<ofVec3f> baseVertices, const std::unordered_map<std::string, std::string> &tags):centerPos(baseVertices[0]){}

    void draw() const {
        ofSetColor(ofColor(255,125, 125, 255));
        ofDrawSphere(centerPos, 1);
        ofSetColor(ofColor(255,125, 125, 100));
        ofDrawSphere(centerPos, 30);
    }
    
    void label(const Camera &camera, const std::string &text) const{
        ofVec2f posOnScreen = camera.worldToScreen(centerPos, ofGetCurrentViewport());
        ofVec2f textOnScreen = posOnScreen;
        textOnScreen.y -= 200.0;
        
        if(!(
             (0 < textOnScreen.x && textOnScreen.x < WIDTH) &&
             (0 < textOnScreen.y && textOnScreen.y < HEIGHT)
             )
           ){return;}
        
        ofVec2f tipOfLine = textOnScreen;
        tipOfLine.y += 10;
        
        ofSetColor(ofColor(255, 125, 125, 255));
        ofDrawLine(posOnScreen, tipOfLine);
        
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(textOnScreen, text);
            
    }
    
private:
    ofVec3f centerPos;
};
