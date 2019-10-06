#pragma once

#include "VboModelController.hpp"

class WayController : public VboModelController{
public:
    WayController(const std::string &styleName):
    VboModelController(styleName){}
    
    void add(long long id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags) override{
        
		std::shared_ptr<Model> model = std::make_shared<Model>(id, vertices, tags);
        std::vector<ofIndexType> indices = createIndices(allVertices.size(), vertices.size());
        allIndices.insert(std::end(allIndices), std::begin(indices), std::end(indices));

        allVertices.insert(std::end(allVertices), std::begin(vertices), std::end(vertices));
        storeInArea(model);
    }

    void draw() const override{
        vboRenderer.drawElement(GL_LINES);
    }
    
    void draw(Camera &camera) const override{
        draw(); // since vbo is a big static object. frustum cull doesn't make sense
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
