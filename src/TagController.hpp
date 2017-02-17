#pragma once
#include <unordered_map>

// "tag" is a specific data format of OSM XML, which consists of k and v pairs.

class TagController{
    
public:
    TagController(std::unordered_map<std::string, std::string> tags):
    tags(tags),
    name(extractName(tags))
    {}
    
    std::string getValue(std::string k) const{
        auto itr = tags.find(k);
        if(itr == tags.end()) return "";
        return itr->second;
    }
    
    virtual void draw() = 0;
    
protected:
    const std::string name;
    
private:
    const std::unordered_map<std::string, std::string> tags;
    
    std::string extractName(std::unordered_map<std::string, std::string> &tags){
        auto itr = tags.find("name");
        if(itr != tags.end()) return itr->second;
        return "";
    }
};
