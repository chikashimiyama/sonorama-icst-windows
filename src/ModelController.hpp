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
        std::shared_ptr<Model<R>> modelPtr = std::make_shared<Model<R>>(id, vertices, tags);
        models.push_back(modelPtr);
        std::pair<bool, int> area = getArea(modelPtr->getPosition());
        if(area.first){
            distribution[area.second].push_back(modelPtr);
        }
    }

    size_t getNumberOfModels() const {
        return models.size();
    }
    
    void draw(const Camera &camera) const{
        stylizer.stylize();
        auto &visibleAreas = camera.getVisibleAreas();
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i])continue;
            for(auto model :distribution[i]){
                model->draw();
            }
        }
    }
    
    void label(const Camera &camera) const{
        auto &visibleAreas = camera.getVisibleAreas();
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i])continue;
            for(auto model :distribution[i]){
                model->label(camera);
            }
        }([&camera ](const std::shared_ptr<Model<R>> model){model->label(camera);});
    }
    

    
    void traverse(std::function<void(std::shared_ptr<Model<R>> model)> func) const{
        for(std::shared_ptr<Model<R>> model : models){
            func(model);
        }
    }

protected:

    std::vector<std::shared_ptr<Model<R>>> models;
    const Stylizer &stylizer;
    std::array<std::vector<std::shared_ptr<Model<R>>>, NUM_AREA> distribution;

};
