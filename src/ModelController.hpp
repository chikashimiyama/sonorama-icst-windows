#pragma once

#include "Model.hpp"
#include "Camera.hpp"
#include "TagController.hpp"
#include "Const.hpp"

class ModelController{

public:

    ModelController(const std::string &styleName ):
    vboRenderer(styleName){};    

    virtual void add(SInt64 id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags) = 0;
    virtual void draw() const = 0;

    void initVbo(){
        vboRenderer.setup(allVertices,allIndices);
    }
    
    void label(const Camera &camera) const{
        auto &visibleAreas = camera.getVisibleAreas();
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i])continue;
            for(auto model :distribution[i]){
                model.label(camera);
            }
        }
    }
    
    void closestLabel(const Camera &camera) const{
        auto map = getRenderTargetMap(camera);

        for(auto &item :map){
            item.second.first->label(camera);
        }
    }
    
protected:
    void storeInArea(Model &model){
        std::pair<bool, int> area = getArea(model.getPosition());
        if(area.first){
            distribution[area.second].push_back(model);
        }
    }
    
    VboRenderer vboRenderer;
    std::vector<ofVec3f> allVertices;
    std::vector<ofIndexType> allIndices;
    std::array<std::vector<Model>, NUM_AREA> distribution;

private:
    virtual std::vector<ofIndexType> createIndices(int offset, size_t numVertex) = 0;

    std::unordered_map<std::string, std::pair<const Model* , float>> getRenderTargetMap(const Camera &camera) const{
        auto &visibleAreas = camera.getVisibleLabelAreas();
        auto cameraPos = camera.getPosition();
        std::unordered_map<std::string, std::pair<const Model* , float>> renderTargetMap;
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i]) continue;
            for(auto &model :distribution[i]){
                std::string name = model.getName();
                if(name == "") continue;
                
                float distance = cameraPos.distance(model.getPosition());
                
                auto itr = renderTargetMap.find(name);
                if(itr == renderTargetMap.end()){
                    renderTargetMap.emplace(name, std::make_pair(&model, distance));
                }else{
                    auto &modelDist = itr->second;
                    if(distance < modelDist.second){
                        itr->second = std::make_pair(&model, distance);
                        
                    }
                }
            }
        }
        return  std::move(renderTargetMap);
    }
};
