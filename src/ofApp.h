#pragma once

#include <unordered_map>
#include "ofMain.h"
#include "Bounds.hpp"
#include "FontServer.hpp"
#include "ofxPd.h"
#include "NodeController.hpp"
#include "ModelController.hpp"
#include "MapDataController.hpp"
#include "SoundSphereController.hpp"
#include "Camera.hpp"
#include "TuioAdapter.hpp"
#include "SyphonAdapter.hpp"
#include "SoundEngine.hpp"

class ofApp : public ofBaseApp{

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
    void initializeSessionsWithExternalSystems();
    void loadSoundSpheres();
    
    ofEasyCam debugCam;
    ofNode cameraGroup;
    std::array<Camera, NUM_VIEWPORTS> cameras;
    bool debug;
    std::pair<bool,int> currentArea;
    void glSetup();
    MapDataController mapDataController;
    SoundSphereController soundSphereController;
    std::array<ofRectangle, NUM_VIEWPORTS> viewports;
    TuioAdapter tuioAdapter;
    SoundEngine soundEngine;
    SyphonAdapter syphonAdapter;
    ofVec3f position;
	
};
