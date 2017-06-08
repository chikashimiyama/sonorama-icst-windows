#include "ofApp.h"
#include "StylizerFactory.hpp"
#include <functional>
#include <tuple>
#include <array>
#include "sqlite_modern_cpp.h"

#pragma mark public functions

ofApp::ofApp():
soundSphereController(soundEngine){}

void ofApp::setup(){

    glSetup();
    loadSoundSpheres();
    
    ofSoundStreamSetup(2, 0, this, SAMPLE_RATE, ofxPd::blockSize()*TICKS_PER_BUFFER, 3);
    soundEngine.setup();
    
    debug = false;
    currentArea = std::make_pair<bool,int>(false,0);
    position.y = CAMERA_HEIGHT;
	for(int i = 0; i < cameras.size(); i++){
		cameras[i].setup(i, position); // ID position
    }
	
    tuioAdapter.setup(TRACK_MASTER_IP, TRACK_MASTER_PORT, MY_TUIO_PORT);
    syphonAdapter.setup(SYPHON_IP, SYPHON_PORT);

}

void ofApp::update(){
    tuioAdapter.processReceivedOSCMessages();
	tuioAdapter.evaluateTouch();
    ofVec3f pos = cameraGroup.getPosition();
    currentArea = getArea(pos);
    
    for(auto &camera: cameras){
        camera.setPosition(position);
        camera.update();
    }
    
    soundEngine.updateLevels();
    soundSphereController.updateLevel();
    
    warp();
    
}

void ofApp::warp(){
    if(position.distance(INITIAL_POS) > AREA_RADIUS){
        position = INITIAL_POS;
    }
}

void ofApp::drawContent(const Camera &camera){
    mapDataController.draw(camera);
    soundSphereController.draw();
    drawBorder();
    //drawGrid();
    //drawArea();
}

void ofApp::draw(){
  
    for(int i = 0; i < NUM_VIEWPORTS; i++){
        ofPushView();
            ofViewport(ofRectangle(WIDTH * i, 0, WIDTH, SCREEN_HEIGHT));
            cameras[i].begin();
            drawContent(cameras[i]);
            cameras[i].end();
        ofPopView();
      
        ofViewport(ofRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        soundSphereController.label(cameras[i]);
        if(currentArea.first){
            mapDataController.label(cameras[i]);
        }
    }
    tuioAdapter.draw();
    drawCredit();
    
    syphonAdapter.sendToSyphon();

//  drawLog();
}

void ofApp::drawBorder(){
    ofSetColor(255, 255, 255, 50);
    ofNoFill();
    ofSetLineWidth(40);
    ofPushMatrix();
    ofTranslate(0,600,0);
    ofRotate(90,1,0,0);
    ofDrawCircle(0, 0, AREA_RADIUS);
    ofPopMatrix();
    ofFill();

}

void ofApp::drawCredit(){
    ofSetColor(255);
    FontServer * fontServer = FontServer::getSingleton();
    fontServer->drawText(ofVec2f(5,10), CREDIT_TEXT);
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
}

void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
    soundEngine.audioOut(output, bufferSize, nChannels);
}

void ofApp::keyPressed(int key){
    switch(key){
        case 'w':
            position.z += 10.0;
            break;
        case 'a':
            position.x -= 10.0;
            break;
        case 'd':
            position.x += 10.0;
            break;
        case 's':
            position.z -= 10.0;
            break;
        default:
            return;
    }
    soundSphereController.updateSound(position);
}

void ofApp::keyReleased(int key){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

#pragma mark private functions

void ofApp::glSetup(){
    ofBackground(ofColor::black);
    ofSetCircleResolution(CIRCLE_RESOLUTION);

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
    viewports[0] = ofRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
}

void ofApp::loadSoundSpheres(){
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
}


