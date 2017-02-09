#pragma once

#include <unordered_map>
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Bounds.hpp"
#include "FontServer.hpp"

#include "NodeController.hpp"
#include "WayController.hpp"
#include "OSMInterpreter.hpp"

class ofApp : public ofBaseApp{

public:
    ofApp();
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofXml xml;
    ofEasyCam camera;
    NodeController nodeController;
    std::unordered_map<std::string, WayController> wayControllers;
    WayController buildingController;
    
private:
    void addNode(ofXml &xml);
    void setBounds(ofXml &xml);
    void checkHighWay(SInt64 id, std::vector<ofVec3f>&vertices, std::unordered_map<std::string, std::string> &tags);
    void addWay(ofXml &xml);
    void loadDataFromXml();
    void glSetup();
};
