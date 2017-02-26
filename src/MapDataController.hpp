#pragma once
#include "ofMain.h"
#include "Const.hpp"
#include "BuildingRenderer.hpp"
#include "SoundSphereRenderer.hpp"
#include "WayRenderer.hpp"

class MapDataController{
    
public:
    MapDataController():
    buildingController("building"){
        
        std::array<std::string,8> wayTypes={{"primary", "tertiary", "path", "residential", "footway", "waterway", "railway", "leisure"}};
        for(auto wayType : wayTypes){
            wayControllers.emplace(wayType, ModelController<WayRenderer>(wayType));
        }
        loadDataFromXml();
    }

    
    void draw(const Camera &camera){
        for(auto &controller : wayControllers){
            controller.second.draw(camera);
        }
        
        ofEnableDepthTest();
        buildingController.draw(camera);
        ofDisableDepthTest();
    }
    
    void label(const Camera &camera) {
        const std::array<bool, NUM_AREA> &visibleAreas = camera.getVisibleAreas();
        ofSetColor(ofColor::lightGray);
        buildingController.label(camera);
        wayControllers.at("primary").closestLabel(camera);
        wayControllers.at("tertiary").closestLabel(camera);
        wayControllers.at("path").closestLabel(camera);
        wayControllers.at("waterway").closestLabel(camera);
    }
    
private:

    
    void loadDataFromXml(){
        ofFile file;
        file.open("map.osm");
        ofBuffer buffer = file.readToBuffer();
        xml.loadFromBuffer(buffer.getText());
        xml.setTo("meta");
        while(xml.setToSibling()){
            std::string name = xml.getName();
            if(name == "node"){
                addNode(xml);
            }else if(name == "bounds"){
                setBounds(xml);
            }else if(name == "way"){
                addModel(xml);
            }
        }
    }

    void addNode(ofXml &xml){
        
        SInt64 id = std::strtoll(xml.getAttribute("id").c_str(), nullptr, 10);
        float lat = ofToFloat(xml.getAttribute("lat"));
        float lon = ofToFloat(xml.getAttribute("lon"));
        std::unordered_map<std::string, std::string> tags;
        
        if(xml.getNumChildren() > 0){
            xml.setToChild(0);
            do{
                std::string innerName = xml.getName();
                if(innerName == "tag"){
                    std::string k = xml.getAttribute("k");
                    std::string v = xml.getAttribute("v");
                    tags.emplace(k,v);
                }
            }while(xml.setToSibling());
            xml.setToParent();
        }
        nodeController.add(id, lat, lon, tags);
    }
    
    void setBounds(ofXml &xml){
        float minLat = ofToFloat(xml.getAttribute("minlat"));
        float maxLat = ofToFloat(xml.getAttribute("maxlat"));
        float minLon = ofToFloat(xml.getAttribute("minlon"));
        float maxLon = ofToFloat(xml.getAttribute("maxlon"));
        Bounds *bounds = Bounds::getSingleton();
        bounds->setup(minLat,maxLat,minLon,maxLon);
    }
    
    void addModel(ofXml &xml){
        SInt64 id = std::strtoll(xml.getAttribute("id").c_str(), nullptr, 10);
        if(xml.getNumChildren() > 0){
            
            xml.setToChild(0);
            std::vector<ofVec3f> vertices;
            std::unordered_map<std::string, std::string> tags;
            do{
                std::string innerName = xml.getName();
                if(innerName == "nd"){
                    std::string idStr = xml.getAttribute("ref");
                    SInt64 id = std::strtoll(idStr.c_str(), nullptr, 10);
                    vertices.push_back(nodeController.getVertex(id));
                }else if(innerName == "tag"){
                    std::string k = xml.getAttribute("k");
                    std::string v = xml.getAttribute("v");
                    tags.emplace(k,v);
                }
            }while(xml.setToSibling());
            xml.setToParent();
            {
                auto it = tags.find("highway");
                if(it != tags.end()){
                    std::string type = it->second;
                    auto itr = wayControllers.find(type);
                    if(itr != wayControllers.end()){
                        itr->second.add(id, vertices, tags);
                    }
                }
            }
            {
                
                auto itr = tags.find("building:levels");
                if(itr != tags.end()){
                    buildingController.add(id, vertices, tags);
                }else{
                    itr = tags.find("building");
                    if(itr != tags.end()){
                        buildingController.add(id, vertices, tags);
                    }
                }
            }
            std::array<std::string, 3> miscModelTypes = {{"waterway", "railway", "leisure"}};
            for(std::string &miscModelType : miscModelTypes){
                auto itr = tags.find(miscModelType);
                if(itr != tags.end()){
                    std::string type = itr->second;
                    wayControllers.at(miscModelType).add(id, vertices, tags);
                }
            }
        }
    }
    
    ofXml xml;
    
    NodeController nodeController;
    std::unordered_map<std::string, ModelController<WayRenderer>> wayControllers;
    ModelController<BuildingRenderer> buildingController;
    
};

