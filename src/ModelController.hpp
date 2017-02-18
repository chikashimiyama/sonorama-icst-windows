#pragma once

#include "Model.hpp"
#include "Camera.hpp"
#include "TagController.hpp"
#include "StylizerFactory.hpp"

template <typename T>
class ModelController{

public:
    ModelController():stylizer(StylizerFactory::getStylizer("sphere")){}
    
    ModelController(const std::string &styleName):
    stylizer(StylizerFactory::getStylizer(styleName)){}
    
    void add(SInt64 id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags){
        models.emplace_back(id, vertices, tags);
    }
    
    size_t getNumberOfModels() const {
        return models.size();
    }
    
    void draw(){
        stylizer.stylize();
        traverse([](T &model){model.draw();});
    }
    
    void label(const Camera &cam){
        traverse([&cam ](T &model){model.label(cam);});
    }
    
    void traverse(std::function<void(T &model)> func){
        for(auto &model : models){func(model);}
    }

protected:

    std::vector<T> models;
    const Stylizer &stylizer;
};
