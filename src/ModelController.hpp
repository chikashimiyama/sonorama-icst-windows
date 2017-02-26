#pragma once

#include "Model.hpp"
#include "Camera.hpp"
#include "TagController.hpp"
#include "StylizerFactory.hpp"
#include "Const.hpp"

template <typename R>
class ModelController{

public:
    ModelController():stylizer(StylizerFactory::getStylizer("sphere")){}
    
    ModelController(const std::string &styleName):
    stylizer(StylizerFactory::getStylizer(styleName)){}
    
    void add(SInt64 id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags){
        auto model = Model<R>(id, vertices, tags);
        std::pair<bool, int> area = getArea(model.getPosition());
        if(area.first){
            distribution[area.second].push_back(model);
        }
    }
    
    void draw(const Camera &camera) const{
        stylizer.stylize();
        auto &visibleAreas = camera.getVisibleAreas();
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i])continue;
            for(auto model :distribution[i]){
                model.draw();
            }
        }
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

private:
    std::unordered_map<std::string, std::pair<const Model<R>* , float>> getRenderTargetMap(const Camera &camera) const{
        auto &visibleAreas = camera.getVisibleLabelAreas();
        auto cameraPos = camera.getPosition();
        std::unordered_map<std::string, std::pair<const Model<R>* , float>> renderTargetMap;
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

    const Stylizer &stylizer;
    std::array<std::vector<Model<R>>, NUM_AREA> distribution;

};
