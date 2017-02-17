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


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawLog();
    void drawArea();
    void drawGrid();
    void keyPressed(int key);
    void keyReleased(int key);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofEasyCam camera;
    
private:
    std::pair<bool,int> currentArea;
    void glSetup();
    MapDataController mapDataController;
    ModelController<Model<SoundSphereRenderer>> soundSphereController;
};
