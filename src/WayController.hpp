#pragma once

#include "ModelController.hpp"

class WayController : public ModelController{
public:
    WayController(const std::string &styleName):
    ModelController(styleName){}
    
    void add(SInt64 id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags) override{
        
        auto model = Model(id, vertices, tags);
        std::vector<ofIndexType> indices = createIndices(allVertices.size(), vertices.size());
        allIndices.insert(std::end(allIndices), std::begin(indices), std::end(indices));

        allVertices.insert(std::end(allVertices), std::begin(vertices), std::end(vertices));
        storeInArea(model);

    }

    void draw() const override{
        vboRenderer.drawElement(GL_LINES);
    }
    
private:
    std::vector<ofIndexType> createIndices(int offset, size_t numVertex) override{
        std::vector<ofIndexType> indices;
        for(int i = 0; i < numVertex-1; i++){
            indices.push_back(i+offset);
            indices.push_back(i+1+offset);
        }
        return indices;
    }
    
};
