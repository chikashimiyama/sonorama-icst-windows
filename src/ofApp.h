#pragma once

#include <unordered_map>
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Bounds.hpp"
#include "FontServer.hpp"

#include "NodeController.hpp"
#include "ModelController.hpp"
#include "MapDataController.hpp"
#include "SoundSphere.hpp"
#include "Camera.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawContent();
    void drawLog();
    void drawArea();
    void drawGrid();
    void keyPressed(int key);
    void keyReleased(int key);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    
    
private:
    ofEasyCam debugCam;
    Camera camera;
    bool debug;
    std::pair<bool,int> currentArea;
    void glSetup();
    MapDataController mapDataController;
    ModelController<SoundSphereRenderer> soundSphereController;
    
    ofLight light;
};
