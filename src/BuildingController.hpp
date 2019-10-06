#pragma once

#include "VboModelController.hpp"

class BuildingController : public VboModelController{
    
public:
    BuildingController(const std::string &styleName):
    VboModelController(styleName){}
    
    void add(long long id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags) override{
        
        auto model = std::make_shared<Model>(id, vertices, tags);
        ofVec3f pos = model->getPosition();
        
        std::string levelsStr = model->getValue("building:levels");
        int levels = levelsStr == "" ? 1 : ofToInt(levelsStr);
        std::vector<ofIndexType> indices = createIndices(allVertices.size(), vertices.size());

        // scale down for anti flicker
        for(auto &vertex:vertices){
            auto delta = vertex - pos;
            delta *= ANTI_FLICKER_SCALE_DOWN_RATIO;
            vertex = pos + delta;
        }
        
        allVertices.insert(std::end(allVertices), std::begin(vertices), std::end(vertices));
        ceilingIndexGroup.push_back(std::make_pair<size_t, size_t>(allVertices.size(), vertices.size()));

        std::vector<ofVec3f> ceiling = createCeiling(vertices, levels * LEVEL_HEIGHT);
        allVertices.insert(std::end(allVertices), std::begin(ceiling), std::end(ceiling));
        
        allIndices.insert(std::end(allIndices), std::begin(indices), std::end(indices));


        std::pair<bool, int> area = getArea(model->getPosition());
        if(area.first){
            distribution[area.second].push_back(model);
        }
    }
    

    
    void draw() const override{
        vboRenderer.drawElement(GL_QUADS);
        drawCeilings();
    }
    
    void draw(Camera &camera) const override{
        draw(); // since vbo is a big static object. frustum cull doesn't make sense
    }
    


private:
    std::vector<ofVec3f> createCeiling(const std::vector<ofVec3f> &baseVertices, float height){
        
        std::vector<ofVec3f> ceiling;
        for(auto &vertex: baseVertices){
            ceiling.emplace_back(vertex.x, vertex.y + height, vertex.z);
        }
        return std::move(ceiling);
    }
    
    std::vector<ofIndexType> createIndices(int offset, size_t numVertex) override{
        std::vector<ofIndexType> indices;
        for(ofIndexType i = 0; i < numVertex; i++){
            ofIndexType start = i;
            ofIndexType end = i + 1;
            if(end == numVertex) end = 0;
            indices.push_back(start + offset);
            indices.push_back(end + offset);
            indices.push_back(end+numVertex + offset);
            indices.push_back(start+numVertex + offset);

        }
        return indices;
    }
    
    void drawCeilings() const{
        
        for(auto &group : ceilingIndexGroup){
            vboRenderer.draw(GL_POLYGON,  group.first , group.second);
        }
    }
    

    std::vector<std::pair<size_t, size_t>> ceilingIndexGroup; // offset + size of one builing vertex group
};


