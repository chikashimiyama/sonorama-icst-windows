#pragma once


#include "TagController.hpp"

class Node: public TagController{
    
public:
    
    Node(float lat, float lon, std::unordered_map<std::string, std::string> tags):
    lat(lat),
    lon(lon),
    TagController(tags)
    {};
    
    float getLat() const{return lat;}
    float getLon() const{return lon;}
    
protected:
    const float lat;
    const float lon;

};
