#include "ofApp.h"
#include "StylizerFactory.hpp"
#include <functional>
#include <tuple>
#include <array>
#include "sqlite_modern_cpp.h"


void ofApp::glSetup(){
    ofBackground(ofColor::black);
    for(int i = 0; i < cameras.size(); i ++){
        cameras[i].setNearClip(NEAR_CLIP);
        cameras[i].setFarClip(FAR_CLIP);
        cameras[i].setFov(FOV);
        cameras[i].setForceAspectRatio(true);
        cameras[i].setAspectRatio(ASPECT_RATIO);
        cameras[i].rotate(-90*i,0,1,0);
    }
    
    debugCam.setNearClip(1);
    debugCam.setFarClip(10000);
    debugCam.disableMouseInput();
    
    ofSetFrameRate(30);
    viewports[0] = ofRectangle(0, 0, SCREEN_WIDTH, HEIGHT);

}

void ofApp::setup(){
    debug = false;
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
       // soundSphereController.add(num, position, tags);
    };
    currentArea = std::make_pair<bool,int>(false,0);
    receiver.setup(RECEIVER_PORT);
    
    int ticksPerBuffer = 8;
    ofSoundStreamSetup(2, 0, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
    if(!pd.init(2, 0, 44100, ticksPerBuffer, false)) {
        OF_EXIT_APP(1);
    }
    
    pd.subscribe("toOF");
    pd.addReceiver(*this);
    pd::Patch patch = pd.openPatch(PD_PATCH);
    
}

void ofApp::update(){
    ofVec3f pos = cameraGroup.getPosition();
    currentArea = getArea(pos);
    for(int i = 0; i < NUM_VIEWPORTS; i ++){
//        cameras[i].move(0, 0.1, 1);
//        cameras[i].rotate(1, 0, 1, 0);
        cameras[i].update();
    }
}

void ofApp::drawContent(const Camera &targetCamera){
    
    mapDataController.draw(targetCamera);
    //soundSphereController.draw(targetCamera);
    //drawArea();
    //drawGrid();
}

void ofApp::draw(){
    if(debug){
        debugCam.enableMouseInput();
        debugCam.begin();
        drawContent(cameras[0]);
        cameras[0].drawFrustum();
        debugCam.end();
    }else{
        debugCam.disableMouseInput();
        for(int i = 0; i < NUM_VIEWPORTS; i++){
            ofPushView();
            ofViewport(ofRectangle(SCREEN_WIDTH * i, 0, SCREEN_WIDTH, HEIGHT));
            cameras[i].begin();
            drawContent(cameras[i]);
            cameras[i].end();
            ofPopView();
        }
    }
    
    for(int i = 0; i < NUM_VIEWPORTS; i++){

        //soundSphereController.label(cameras[i]);
        if(currentArea.first){
            mapDataController.label(cameras[i]);
        }
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
//        fontServer->drawText(ofVec2f(10,40), ofToString(camera.getPosition()));
//        fontServer->drawText(ofVec2f(10,60), "visible area: " + ofToString(camera.getNumVisibleAreas()));
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

void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
    pd.audioIn(input, bufferSize, nChannels);
}

void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
    pd.audioOut(output, bufferSize, nChannels);
}

void ofApp::keyPressed(int key){
    if(key == 'D'){
        debug = !debug;
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
