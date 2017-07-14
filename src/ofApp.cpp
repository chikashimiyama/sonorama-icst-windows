#include "ofApp.h"
#include "StylizerFactory.hpp"
#include <functional>
#include <tuple>
#include <array>
#include "sqlite_modern_cpp.h"
#include "InterSenseController.hpp"
#pragma mark public functions

ofApp::ofApp():
soundSphereController(soundEngine),
logger(cameras, soundSphereController, soundEngine){
}

void ofApp::setup(){
	ofSetWindowPosition(-1200, 10);

    glSetup();
    loadSoundSpheres();
    
    ofSoundStreamSetup(NUM_SPEAKERS, 0, this, SAMPLE_RATE, ofxPd::blockSize()*TICKS_PER_BUFFER, 3);
    soundEngine.setup(CITY_NAME);
    
    debug = false;
    currentArea = std::make_pair<bool,int>(false,0);
    position.y = CAMERA_HEIGHT;
	for(int i = 0; i < cameras.size(); i++){
		cameras[i].setup(i, position); // ID position
    }
	
    if(TUIO_ENABLED){
        tuioAdapter.setup(TRACK_MASTER_IP, TRACK_MASTER_PORT, MY_TUIO_PORT);
    }else if(INTER_SENSE_ENABLED){
        interSenseControllerThread.startStream();
    }
    
    syphonAdapter.setup(SYPHON_IP, SYPHON_PORT);
	ofAddListener(TuioAdapter::moveEvent, this, &ofApp::onMoveEvent);
    
}

void ofApp::onMoveEvent(ofVec2f &movement){
	float radian = (movement.x/4.0 - 0.5) * TWO_PI;
	float y = (movement.y - 0.5) * -2.0;
	float z = cos(radian) * TOUCH_TO_MOVE;
	float x = -sin(radian) * TOUCH_TO_MOVE;
	
	position.z += z;
	position.x += x;
	position.y += y;
	
	position.y = ofClamp(position.y, 200, 500);
	
}

void ofApp::update(){
    if(TUIO_ENABLED){
        tuioAdapter.processReceivedOSCMessages();
    }else if(INTER_SENSE_ENABLED){
        position.z += interSenseControllerThread.getLeftRight();
        position.x += interSenseControllerThread.getFrontBack();
    }
	soundSphereController.updateSound(position);

    currentArea = getArea(position);
    for(auto &camera: cameras){
        camera.setPosition(position);
        camera.update();
    }
    
    soundEngine.update();
    soundSphereController.updateLevels();

    warp();
	logger.update();
}

void ofApp::warp(){
    if(position.distance(INITIAL_POS) > AREA_RADIUS){
        position = INITIAL_POS;
    }
}

void ofApp::drawContent(const Camera &camera){
    mapDataController.draw(camera);
    soundSphereController.draw();
    border.draw();

    //speakers.draw(position);
    //drawGrid();
    //drawArea();
}

void ofApp::draw(){
	
	//begin captureing to FBO
	syphonAdapter.begin();
	ofClear(0);
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
    
    
    if(TUIO_ENABLED) tuioAdapter.draw();
    cardinalDirections.label();
    drawCredit();
	syphonAdapter.end();
	syphonAdapter.sendToSyphon();

	// on console screen
	logger.draw();
}

void ofApp::drawCredit(){
    ofSetColor(180);
    FontServer * fontServer = FontServer::getSingleton();
    fontServer->drawText(ofVec2f(CREDIT_X,CREDIT_Y), CREDIT_TEXT);
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
            position.x += 10.0;
            break;
        case 'd':
            position.x -= 10.0;
            break;
        case 's':
            position.z -= 10.0;
            break;
		case 'u':
			syphonAdapter.connectSyphon();
			break;
		case '>':{
			soundEngine.louder();
			break;
		}
		case '<':{
			soundEngine.softer();
			break;
		}
        default:
            return;
    }
    soundSphereController.updateSound(position);
}



#pragma mark private functions

void ofApp::glSetup(){
    ofBackground(ofColor::black);
    ofSetCircleResolution(CIRCLE_RESOLUTION);


    ofSetFrameRate(30);
    viewports[0] = ofRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
}

void ofApp::loadSoundSpheres(){
    std::string path = ofToDataPath(DATA_BASE_FILE);
    sqlite::database db(path);
    db << "select * from sound_sphere;"
    >>[this](int num, std::string title, std::string location, double latitude, double longitude, std::string wavfile ){
        Bounds * bounds = Bounds::getSingleton();
        std::vector<ofVec3f> position;
        position.push_back(bounds->scale(latitude, longitude));
        
        std::unordered_map<std::string, std::string> tags;
        tags.emplace("name", title);
        tags.emplace("location", location);
        soundSphereController.add(num, position, tags);
    };
}

void ofApp::exit(){
    if(INTER_SENSE_ENABLED){
        interSenseControllerThread.stopStream();
        interSenseControllerThread.close();
    }
}


