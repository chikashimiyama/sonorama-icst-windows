#pragma once

#include "Model.hpp"
#include "Camera.hpp"
#include "TagController.hpp"
#include "StylizerFactory.hpp"

template <typename R>
class ModelController{

public:
    ModelController():stylizer(StylizerFactory::getStylizer("sphere")){}
    
    ModelController(const std::string &styleName):
    stylizer(StylizerFactory::getStylizer(styleName)){}
    
    void add(SInt64 id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags){
        models.emplace_back(id, vertices, tags);
    }
    
    void prepareDistribution(){
        for(int i = 0;i < models.size(); i++){
            ofVec3f position = models[i].getPosition();
            std::pair<bool, int> area = getArea(position);
            if(area.first){
                distribution[area.second].push_back(&models[i]);
            }
        }
    }
    
    size_t getNumberOfModels() const {
        return models.size();
    }
    
    void draw() const{
        stylizer.stylize();
        traverse([](const Model<R> &model){model.draw();});
    }
    
    void drawAreas(const std::array<bool, NUM_AREA> &visibleAreas, const Camera &camera) const{

    }
    
    void label(const Camera &camera) const{
        traverse([&camera ](const Model<R> &model){model.label(camera);});
    }
    
    void labelAreas(const std::array<bool, NUM_AREA> &visibleAreas, const Camera &camera) const{
        for(int i = 0; i < NUM_AREA;i++){
            if(!visibleAreas[i])continue;
            for(auto model :distribution[i]){
                model->label(camera);
            }
        }
    }
    
    void traverse(std::function<void(const Model<R> &model)> func) const{
        for(auto &model : models){func(model);}
    }

protected:

    std::vector<Model<R>> models;
    const Stylizer &stylizer;
    std::array<std::vector<const Model<R>* >, NUM_AREA> distribution;

};
