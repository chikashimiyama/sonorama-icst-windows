#pragma once
#include "ofMain.h"
#include "Const.hpp"
#include "Blob.hpp"

class TouchPrompter{
public:
	void setup(){
		iconImage.load(TOUCH_ICON_IMAGE_FILE);
		touched = -1;
		for(int i = 0; i < NUMBER_OF_TOUCH_ICONS;i++){
			detectionPanels.push_back(getRectFromIconID(i, DETECTION_PANEL_SIZE));
		}
	}
	
	void update(std::unordered_map<int, Blob> &blobMap){
		for(auto &blob : blobMap){
		
		}
	}
	
	void draw(){
		if(touched < 0){
			drawUntouched();
		}else{
			drawTouched();
		}
		
		drawDetectionPanel();
	}
	
	int getTouched(){
		return touched;
	}
	
	int getTouchedScreen(){
		int degreeOfIcon = touched * TOUCH_ICON_DISTANCE + TOUCH_ICON_OFFSET;
		return degreeOfIcon / FOV;
	}

private:
	ofRectangle getRectFromIconID(int i, float size){
		float x = TOUCH_ICON_OFFSET + TOUCH_ICON_DISTANCE * i - size / 2;
		float y = TOUCH_ICON_VOFFSET - size / 2;
		return ofRectangle(x, y, size, size );
	}
	void drawUntouched(){
		ofSetColor(TOUCH_READY_COLOR);
		for (int i = 0; i < NUMBER_OF_TOUCH_ICONS; i++) {
			iconImage.draw(getRectFromIconID(i, TOUCH_ICON_SIZE));
		}
	}
	void drawTouched(){
		for (int i = 0; i < NUMBER_OF_TOUCH_ICONS; i++) {
			if(i == touched){
				ofSetColor(TOUCH_UNAVAILABLE_COLOR);
			}else{
				ofSetColor(TOUCH_ACTIVE_COLOR);
			}
			iconImage.draw(getRectFromIconID(i, DETECTION_PANEL_SIZE));
		}
	}
	void drawDetectionPanel(){
		for (auto &panel: detectionPanels){
			ofSetColor(255,0,0,40);
			ofDrawRectangle(panel);
		}

	}

	int touched;
	std::vector<ofRectangle> detectionPanels;
	ofImage iconImage;
};