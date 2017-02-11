#pragma once

#include <unordered_map>
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Bounds.hpp"
#include "FontServer.hpp"

#include "NodeController.hpp"
#include "WayController.hpp"
#include "MapDataController.hpp"
#include "SoundSphereController.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofEasyCam camera;
    
private:
    void glSetup();
    MapDataController mapDataController;
    SoundSphereController soundSphereController;
};
