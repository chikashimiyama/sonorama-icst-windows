#pragma once

#include "Way.hpp"
#include "TagController.hpp"
#include "StylizerFactory.hpp"

class WayController{

public:
    WayController(const std::string &styleName):
    stylizer(StylizerFactory::getStylizer(styleName)){}
    
    void add(SInt64 id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags){
        ways.emplace_back(id, vertices, tags);
    }
    
    size_t getNumberOfWays() const {
        return ways.size();
    }
    
    void draw(){
        stylizer.stylize();
        for(auto &way: ways){
            way.draw();
          //  way.writeLabel();
        }
    }
    
protected:

    std::vector<Way> ways;
    const Stylizer &stylizer;
    
};
