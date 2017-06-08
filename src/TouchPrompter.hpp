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
			detectionCircles.push_back(getCircleCenterFromIconID(i));
		}
        touchReadyColor = TOUCH_READY_COLOR;
        alphaIncrement = 0.05;
	}
	
	void evaluate(std::unordered_map<int, Blob> &blobMap){
		if(touched < 0){
			
			for(int i = 0; i<  detectionCircles.size(); i++){
				int count = 0;
				auto panel = detectionCircles[i];
				for(auto &blob : blobMap){
					if(blob.second.age > BLOB_MATURITY_THRESH){
                        ofVec2f blobPos = blob.second.position;
                        if(detectionCircles[i].distance(blobPos) < 100.0){
                            // hit
                        }
					}
					count++;
				}
				if(count >= 2){
					touched = i;
					break;
				}
			}
		}
	}
	
	void draw(){
        if(touchReadyColor.a > 1.0f) alphaIncrement = -0.02f;
        if(touchReadyColor.a < 0.0f) alphaIncrement = 0.02f;
        touchReadyColor.a += alphaIncrement;
        ofSetColor(touchReadyColor);
        
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
	ofVec2f getCircleCenterFromIconID(int i){
		float x = TOUCH_ICON_OFFSET + TOUCH_ICON_DISTANCE * i;
		float y = TOUCH_ICON_VOFFSET;
        return ofVec2f(x, y);
	}
	void drawUntouched(){
		for (int i = 0; i < NUMBER_OF_TOUCH_ICONS; i++) {
            ofVec2f center = getCircleCenterFromIconID(i);
			iconImage.draw(center.x - HALF_RADIUS, center.y - HALF_RADIUS, CIRCLE_RADIUS, CIRCLE_RADIUS);
		}
	}
	void drawTouched(){
		for (int i = 0; i < NUMBER_OF_TOUCH_ICONS; i++) {
			if(i == touched){
				ofSetColor(TOUCH_UNAVAILABLE_COLOR);
			}else{
				ofSetColor(TOUCH_ACTIVE_COLOR);
			}
            ofVec2f center = getCircleCenterFromIconID(i);
            iconImage.draw(center.x - HALF_RADIUS, center.y - HALF_RADIUS, CIRCLE_RADIUS, CIRCLE_RADIUS);
		}
	}
	void drawDetectionPanel(){
        ofNoFill();
        ofSetLineWidth(CIRCLE_LINE_WIDTH);
    
		for (auto &circle: detectionCircles){
            ofDrawCircle(circle,CIRCLE_RADIUS);

		}
        ofFill();
        ofSetLineWidth(0);
	}

	int touched;
	std::vector<ofVec2f> detectionCircles;
	ofImage iconImage;
    ofFloatColor touchReadyColor;
    float alphaIncrement;
};
