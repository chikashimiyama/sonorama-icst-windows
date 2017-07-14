#pragma once
#include "ofMain.h"
#include "Const.hpp"
#include "Camera.hpp"

class Logger{
	
public:
	Logger(const std::array<Camera, NUM_VIEWPORTS> &cameras, const SoundSphereController &soundSphereController);
	void update() noexcept;
	void draw() const noexcept;
	void onMoveEvent(ofVec2f &mov);
	
protected:
	std::string log;
	const std::array<Camera, NUM_VIEWPORTS> &cameras;
	const SoundSphereController &soundSphereController;
private:
	std::string getCameraLog() const noexcept;
	std::string getTouchLog() const noexcept;
	std::string getSoundLog() const noexcept;
	std::string getFPSLog() const noexcept;
	std::string getManualNavigation() const noexcept;
	
	ofVec2f movement;
};

inline Logger::Logger(const std::array<Camera, NUM_VIEWPORTS> &cameras, const SoundSphereController &soundSphereController):
cameras(cameras),
movement(ofVec2f(0,0)),
soundSphereController(soundSphereController){

	ofAddListener(TuioAdapter::moveEvent, this, &Logger::onMoveEvent);
}

inline void Logger::onMoveEvent(ofVec2f &mov){
	movement = mov;
}

inline void Logger::update() noexcept{
	log = "";
	log += getCameraLog();
	log += getTouchLog();
	log += getSoundLog();
	log += getFPSLog();
	log += getManualNavigation();
}

inline void Logger::draw() const noexcept{
	ofDrawBitmapString(log, 20, 20);
}

inline std::string Logger::getCameraLog() const noexcept{
	std::string cameraLog = "Cameras:\n";
	cameraLog += "   division:" + ofToString(AREA_DIVISION) +"\n";
	cameraLog += "   number of areas:" + ofToString(NUM_AREA) +"\n";
	cameraLog += "   number of rendered area per camera:\n";
	
	if(cameras.size() != 4) {
		return std::move(cameraLog);
	};
	
	for(int i = 0; i < 4; i++){
		int visArea = cameras[i].getNumVisibleAreas();
		std::string direction = CARDINAL_DIRECTIONS[i];
		cameraLog += "      " + direction + " camera: " + ofToString(visArea) + "\n";
		
	}
	cameraLog += "\n";
	return std::move(cameraLog);
}

inline std::string Logger::getTouchLog() const noexcept{
	std::string touchLog = "Last Touch:\n";
	
	if(movement == ofVec2f(0,0)){
		touchLog += "   Not detected";
	}else{
		int angle = static_cast<int>((movement.x/4.0 - 0.5) * 360);
		touchLog += "   x: " + ofToString(movement.x) + " y: " + ofToString(movement.y) + " degree: " + ofToString(angle) ;
	}
	
	touchLog += "\n\n";
	return std::move(touchLog);

}

inline std::string Logger::getSoundLog() const noexcept{
	std::string soundLog ="Sound Spheres:\n";
	
	size_t active = soundSphereController.getNumberOfActiveSpheres();
	soundLog += "   Active spheres: " + ofToString(active);
	soundLog += "\n\n";
	return std::move(soundLog);
}

inline std::string Logger::getFPSLog() const noexcept{
	std::string fpsLog = "Current FPS: " + ofToString(ofGetFrameRate());
	fpsLog += "\n\n";
	return std::move(fpsLog);
}

inline std::string Logger::getManualNavigation() const noexcept{
	std::string navi = "Key commands: \n   w ... to north\n   a ... to west\n   s ... to south\n   d ... to east\n   u ... reconnect to syphon";
	navi += "\n";
	return std::move(navi);
}


