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
    
    void draw() const{
        stylizer.stylize();
        traverse([](const T &model){model.draw();});
    }
    
    void label(const Camera &camera) const{
        traverse([&camera ](const T &model){model.label(camera);});
    }
    
    void traverse(std::function<void(const T &model)> func) const{
        for(auto &model : models){func(model);}
    }

protected:

    std::vector<T> models;
    const Stylizer &stylizer;
};
