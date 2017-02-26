#pragma once

#include <unordered_map>
#include "ofMain.h"
#include "Bounds.hpp"
#include "FontServer.hpp"
#include "ofxPd.h"
#include "NodeController.hpp"
#include "ModelController.hpp"
#include "MapDataController.hpp"
#include "SoundSphere.hpp"
#include "Camera.hpp"
#include "OSCReceiver.hpp"

class ofApp : public ofBaseApp, public pd::PdReceiver{

public:
    void setup();
    void update();
    void draw();
    void drawContent(const Camera &targetCamera);
    void drawLog();
    void drawArea();
    void drawGrid();
    void keyPressed(int key);
    void keyReleased(int key);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested(float * output, int bufferSize, int nChannels);

    
private:
    ofxPd pd;
    ofEasyCam debugCam;
    Camera camera;
    Camera camera2;
    bool debug;
    std::pair<bool,int> currentArea;
    void glSetup();
    MapDataController mapDataController;
    ModelController<SoundSphereRenderer> soundSphereController;
    std::array<ofRectangle, NUM_VIEWPORTS> viewports;
    OSCReceiver receiver;
};
