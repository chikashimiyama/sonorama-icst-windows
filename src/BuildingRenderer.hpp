#pragma once

#include "VboRenderer.hpp"

class BuildingRenderer: public VboRenderer{
    
public:
    BuildingRenderer(std::vector<ofVec3f> baseVertices, const std::unordered_map<std::string, std::string> &tags):
    VboRenderer(baseVertices, tags){
        std::vector<ofVec3f> ceilingVertices;
        std::vector<ofVec3f> wallVertices;
        
        int level = getLevels(tags);
        
        ofVec3f accum;
        for(auto &vertex : baseVertices){
            accum += vertex;
        }
        centerPos = accum / baseVertices.size();

        for(auto vertex : baseVertices){
            float height = level * LEVEL_HEIGHT;
            
            vertex = (vertex - centerPos) * 0.9 + centerPos;
            ceilingVertices.emplace_back(vertex.x, height, vertex.z);
            wallVertices.emplace_back(vertex.x, 0, vertex.z);
            wallVertices.emplace_back(vertex.x, height, vertex.z);
        }
        
        centerPos.y = HALF_LEVEL_HEIGHT;

        ceilingVbo.setVertexData(&ceilingVertices[0], ceilingVertices.size(), GL_STATIC_DRAW);
        wallVbo.setVertexData(&wallVertices[0], wallVertices.size(), GL_STATIC_DRAW);
    }
    
    void draw() const {
        ofSetColor(125,125,125,50);
        baseVbo.draw(GL_LINE_LOOP, 0, baseVbo.getNumVertices());
        ceilingVbo.draw(GL_LINE_LOOP, 0, ceilingVbo.getNumVertices());

        wallVbo.draw(GL_QUAD_STRIP, 0, wallVbo.getNumVertices());
    }
    
    void label(const Camera &camera, const std::string &text) const{
        if(text.size() == 0 )return;
        if(centerPos.distance(camera.getPosition()) > BUILDING_LABEL_THRESHOLD) return;
        ofVec2f posOnScreen = camera.worldToScreen(centerPos, ofGetCurrentViewport());
        ofVec2f textOnScreen = posOnScreen;
        if(!(
             (0 < textOnScreen.x && textOnScreen.x < WIDTH) &&
             (0 < textOnScreen.y && textOnScreen.y < HEIGHT)
             )
           ){return;}
        
        ofDrawCircle(textOnScreen.x, textOnScreen.y, BULLET_SIZE);
        textOnScreen.x += LABEL_INDENT;
        
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
