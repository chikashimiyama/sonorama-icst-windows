#pragma once
#include "Const.hpp"
#include "ofxOscSender.h"
#include "ofxOSCReceiver.h"


class TuioAdapter{
    
public:
    void setup(std::string trackMasterIP, int trackMasterPort, int myPort){
        oscReceiver.setup(myPort);
        initiateSessionWithTrackMaster(trackMasterIP, trackMasterPort);
		scale = 0.1;
    }
	
    void processReceivedOSCMessages(){
        while(oscReceiver.hasWaitingMessages()) {
            ofxOscMessage mes;
            oscReceiver.getNextMessage(mes);
            interpretOSCMessage(mes);
        }
    }

	void draw(){
		if(alive){
			ofSetColor(255,255,255);
			ofNoFill();
			ofSetLineWidth(3);
			ofDrawCircle(point, scale* 500);
			alive = false;
		}
	}
	
	static ofEvent<ofVec2f> moveEvent;

private:

    
    void initiateSessionWithTrackMaster(std::string trackMasterIP, int trackMasterPort){
        ofxOscSender oscSender;
        oscSender.setup(trackMasterIP, trackMasterPort);
		
        ofxOscMessage m;
		
        m.setAddress( "/trackerMaster/requestTuiostream" );
        m.addIntArg( MY_TUIO_PORT );
        m.addIntArg( 1 );
        oscSender.sendMessage( m );
		
		m.clear();
		m.setAddress("trackerMaster/analysis/touchcluster/maxCount");
		m.addIntArg(1);
		oscSender.sendMessage(m);
    }
    
    void interpretOSCMessage(const ofxOscMessage &mes){
        std::string oscAddress = mes.getAddress();
        if(oscAddress == "/tuio/2Dcur"){
			long i, j, k;
			string firstArg = mes.getArgAsString( 0 );
			
			if(firstArg == "set") { // format: set ID x y accel-x accel-y
				if(mes.getNumArgs() != 6) return -1;
				if(mes.getArgType(2) != OFXOSC_TYPE_FLOAT) return -1;
				if(mes.getArgType(3) != OFXOSC_TYPE_FLOAT) return -1;
				float x = mes.getArgAsFloat(2);
				float y = mes.getArgAsFloat(3);
				ofVec2f p = ofVec2f(x,y);
				ofNotifyEvent(moveEvent, p, this);
				
				point = ofPoint(x * WIDTH, y * HEIGHT);
				alive = true;
				scale += 0.04;
				if(scale >= 1.0) scale = 0.1;
			}
		}
    }
    

	ofPoint point;
	bool alive;
	float scale;
    ofxOscReceiver oscReceiver;
};
