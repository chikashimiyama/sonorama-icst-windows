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
#include "Border.hpp"
#include "CardinalDirections.hpp"
#include "Speakers.hpp"
#include "InterSenseController.hpp"

class ofApp : public ofBaseApp{

public:
    ofApp();
    void setup();
    void update();
    void draw();
    void drawContent(const Camera &targetCamera);
    void drawLog();
    void drawCredit();
    void drawArea();
    void drawGrid();
    void keyPressed(int key);
    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested(float * output, int bufferSize, int nChannels);
    void warp();
    void exit();
	
	void onMoveEvent(ofVec2f &movement);

private:
    void loadSoundSpheres();
    
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
    Border border;
    CardinalDirections cardinalDirections;
    Speakers speakers;
    ofVec3f position;
    InterSenseControllerThread interSenseControllerThread;
};
