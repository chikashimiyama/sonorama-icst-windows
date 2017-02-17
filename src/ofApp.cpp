#include "ofApp.h"
#include "StylizerFactory.hpp"
#include <functional>
#include <tuple>
#include <array>
#include "sqlite_modern_cpp.h"


void ofApp::glSetup(){
    ofBackground(ofColor::black);
    camera.setNearClip(0.01);
    camera.setFarClip(6000);
    ofSetFrameRate(30);
}

void ofApp::setup(){
    glSetup();
    std::string path = ofToDataPath("sound_spheres.db");
    sqlite::database db(path);
    db << "select * from sound_sphere;"
    >>[this](int num, std::string name, double latitude, double longitude ){
        Bounds * bounds = Bounds::getSingleton();
        std::vector<ofVec3f> position;
        position.push_back(bounds->scale(latitude, longitude));
        
        std::unordered_map<std::string, std::string> tags;
        tags.emplace("name", name);
        soundSphereController.add(num, position, tags);
    };
    currentArea = std::make_pair<bool,int>(false,0);
}



void ofApp::update(){
    ofVec3f pos = camera.getPosition();
    currentArea = getArea(pos);
    
}

void ofApp::draw(){
    camera.begin();
    mapDataController.draw();
    soundSphereController.draw();
    drawArea();
    drawGrid();
    camera.end();
    soundSphereController.label(camera);
    if(currentArea.first){
        mapDataController.labelArea(currentArea.second, camera);
    }
    drawLog();
}

void ofApp::drawLog(){
    FontServer * fontServer = FontServer::getSingleton();
    std::tuple<bool,int,int> areaXY = areaToXY(currentArea);
    if(std::get<0>(areaXY)){
        int x = std::get<1>(areaXY);
        int y = std::get<2>(areaXY);
        std::string str = "x:" + ofToString(x) + " y:" + ofToString(y);
        ofSetColor(ofColor::white);
        fontServer->drawText(ofVec2f(10,20), str);
    }
}

void ofApp::drawGrid(){
    ofSetColor(100,100,200,100);
    for(int i =0;i <= AREA_DIVISION;i++){
        float x = i * DIVIDER - HALF_MAP_SIZE;
        ofDrawLine(x, 0, -HALF_MAP_SIZE , x,0, HALF_MAP_SIZE);
        ofDrawLine(-HALF_MAP_SIZE,0 , x, HALF_MAP_SIZE, 0,x);
    }
}

void ofApp::drawArea(){
    ofSetColor(200,100,100,100);
    auto xy = areaToXY(currentArea);
    if(std::get<0>(xy)){
        int x = std::get<1>(xy);
        int z = std::get<2>(xy);
        
        glBegin(GL_QUADS);
        glVertex3f(x*DIVIDER - HALF_MAP_SIZE, 0, z*DIVIDER - HALF_MAP_SIZE);
        glVertex3f(x*DIVIDER - HALF_MAP_SIZE, 0, (z+1)*DIVIDER - HALF_MAP_SIZE);
        glVertex3f((x+1)*DIVIDER -HALF_MAP_SIZE, 0, (z+1)*DIVIDER - HALF_MAP_SIZE);
        glVertex3f((x+1)*DIVIDER - HALF_MAP_SIZE, 0, z*DIVIDER -HALF_MAP_SIZE);
        glEnd();
    }
    
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
