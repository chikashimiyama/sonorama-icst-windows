#include "ofApp.h"
#include "StylizerFactory.hpp"
#include <functional>
#include <array>
ofApp::ofApp():
buildingController("building"){
    
    std::array<std::string,8> wayTypes={{"primary", "tertiary", "path", "residential", "footway", "waterway", "railway", "leisure"}};
    for(auto wayType : wayTypes){
        wayControllers.emplace(wayType, WayController(wayType));
    }
}


void ofApp::addNode(ofXml &xml){
    
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

void ofApp::setBounds(ofXml &xml){
    float minLat = ofToFloat(xml.getAttribute("minlat"));
    float maxLat = ofToFloat(xml.getAttribute("maxlat"));
    float minLon = ofToFloat(xml.getAttribute("minlon"));
    float maxLon = ofToFloat(xml.getAttribute("maxlon"));
    Bounds *bounds = Bounds::getSingleton();
    bounds->setup(minLat,maxLat,minLon,maxLon);
}

void ofApp::addWay(ofXml &xml){
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
            auto itr = tags.find("highway");
            if(itr != tags.end()){
                std::string type = itr->second;
                auto itr = wayControllers.find(type);
                if(itr != wayControllers.end()){
                    itr->second.add(id, vertices, tags);
                }
            }
        }
        {
            auto itr = tags.find("building");
            if(itr != tags.end()){
                std::string type = itr->second;
                buildingController.add(id, vertices, tags);
            }
        }
        std::array<std::string, 3> miscWayTypes = {{"waterway", "railway", "leisure"}};
        for(std::string &miscWayType : miscWayTypes){
            auto itr = tags.find(miscWayType);
            if(itr != tags.end()){
                std::string type = itr->second;
                wayControllers.at(miscWayType).add(id, vertices, tags);
            }
        }
    }
}

void ofApp::loadDataFromXml(){
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
            addWay(xml);
        }
    }
}

void ofApp::glSetup(){
    ofBackground(ofColor::black);
    camera.setNearClip(0.01);
    camera.setFarClip(6000);
    ofSetFrameRate(30);
}

void ofApp::setup(){
    loadDataFromXml();
    glSetup();

}

void ofApp::update(){

}

void ofApp::draw(){
    camera.begin();
    for(auto &controller : wayControllers){
        controller.second.draw();
    }
    buildingController.draw();
    camera.end();
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
