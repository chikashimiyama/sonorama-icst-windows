#pragma once

#include "ofMain.h"
#include "Camera.hpp"
#include "Model.hpp"
/* abstract class for controlling multiple 3D models */

class ModelController{
    
public:
    ModelController(){}
    
    // add a new model
    virtual void add(long long id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags) = 0;
    
    // draw all models
    virtual void draw() const = 0;
    
    // draw visible models from the camera
    virtual void draw(Camera &camera) const = 0;
    
    
    void label(const Camera &camera) const{
        auto &visibleAreas = camera.getVisibleAreas();
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i])continue;
            for(auto model :distribution[i]){
                model->label(camera);
            }
        }
    }
    
    void closestLabel(const Camera &camera) const{
        auto targets = getLabelTargetModelsFromCameraPosition(camera);
        for(auto &model :targets){
            model.second.first->label(camera);
        }
    }
    
protected:
    void storeInArea(std::shared_ptr<Model> model){
        std::pair<bool, int> area = getArea(model->getPosition());
        if(area.first){
            distribution[area.second].push_back(model);
        }
    }
    
    std::array<std::vector<std::shared_ptr<Model>>, NUM_AREA> distribution;

private:
    std::unordered_map<std::string, std::pair<std::shared_ptr<Model> , float>> getLabelTargetModelsFromCameraPosition(const Camera &camera) const{
        auto &visibleAreas = camera.getVisibleAreas();
        auto cameraPos = camera.getPosition();
        std::unordered_map<std::string, std::pair<std::shared_ptr<Model> , float>> renderTargets;
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i]) continue;
            
            
            for(auto &model :distribution[i]){
                std::string name = model->getName();
                if(name == "") continue;
                
                float distance = cameraPos.distance(model->getPosition());
                
                // try to find in the std::map if there is a model named "name"
                auto itr = renderTargets.find(name);
                if(itr == renderTargets.end()){
                    //if not add model to std::map
                    renderTargets.emplace(name, std::make_pair(model, distance));
                }else{
                    // if already exists replace with the new one if the new one is closer to the camera
                    auto &modelDist = itr->second;
                    if(distance < modelDist.second){
                        itr->second = std::make_pair(model, distance);
                    }
                }
            }
        }
        return   renderTargets;
    }

};

