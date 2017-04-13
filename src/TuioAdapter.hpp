#pragma once
#include "ofxOscSender.h"
#include "ofxOSCReceiver.h"
#include "Blob.hpp"
#include "TouchPrompter.hpp"

class TuioAdapter{
    
public:
    void draw(){
		drawBlobs();
		touchPrompter.draw();
    }
    
    void setup(std::string trackMasterIP, int trackMasterPort, int myPort){
        oscReceiver.setup(myPort);
        initiateSessionWithTrackMaster(trackMasterIP, trackMasterPort);
		touchPrompter.setup();
    }
	
	
	
    void processReceivedOSCMessages(){
        while(oscReceiver.hasWaitingMessages()) {
            ofxOscMessage mes;
            oscReceiver.getNextMessage(mes);
            interpretOSCMessage(mes);
        }
    }
	void evaluateTouch(){
		touchPrompter.evaluate(blobs);
	}
    
private:
	void drawBlobs(){
		for(auto &blob : blobs){
			blob.second.draw();
		}
	}
    
    void initiateSessionWithTrackMaster(std::string trackMasterIP, int trackMasterPort){
        ofxOscSender oscSender;
        oscSender.setup(trackMasterIP, trackMasterPort);
        ofxOscMessage m;
        m.setAddress( "/trackerMaster/requestTuiostream" );
        m.addIntArg( MY_TUIO_PORT );
        oscSender.sendMessage( m );
    }
    
    void interpretOSCMessage(const ofxOscMessage &mes){
        std::string oscAddress = mes.getAddress();
        if(oscAddress == "/tuio/2Dcur"){
            parseTuio(mes);
        }
    }
    
    
    void parseTuio(const ofxOscMessage &m){
        long i, j, k;
        string firstArg = m.getArgAsString( 0 );
        
        if(firstArg == "alive"){
            processAliveMessage(m);
        }else if(firstArg == "set") { // format: set ID x y accel-x accel-y
            processSetMessage(m);
        }else if(firstArg == "fseq"){
            processFseqMessage(m);
        }
    }

    void processAliveMessage(const ofxOscMessage &m){
        int numArgs = m.getNumArgs();
		
		if(numArgs <= 1) return;

		for(int i = 0; i < numArgs; i++){
			if(m.getArgType(i) != OFXOSC_TYPE_INT32) continue;
			int blobID = m.getArgAsInt32(i);
			auto result = blobs.find(blobID);
			if(result == blobs.end()){ // not in the map
				blobs.emplace(blobID, Blob()); // added to map
			}else{
				result->second.absence = 0;
				result->second.age += 1;

			}
		}
    }
    
    void processSetMessage(const ofxOscMessage &m){
		int numArgs = m.getNumArgs();
		if(numArgs != 6) return;
		if(m.getArgType(1) != OFXOSC_TYPE_INT32) return;
		if(m.getArgType(2) != OFXOSC_TYPE_FLOAT) return;
		if(m.getArgType(3) != OFXOSC_TYPE_FLOAT) return;
		
		int blobID = m.getArgAsInt32(1);
		blobs[blobID].position = ofPoint(m.getArgAsFloat(2) * WIDTH, m.getArgAsFloat(3) * HEIGHT);
    }
    
    void processFseqMessage(const ofxOscMessage &m){
		int numArgs = m.getNumArgs();
		if(numArgs != 2) return;
		if(m.getArgType(1) != OFXOSC_TYPE_INT32) return;

		for(auto it = blobs.begin(); it != blobs.end();){
			it->second.absence++;
			if(it->second.absence > ABSENCE_TO_DEATH){
				it = blobs.erase(it);
			}else{
				it++;
			}
		}
    }

    std::unordered_map<int, Blob> blobs; // ID and blob object
	TouchPrompter touchPrompter;
    ofxOscReceiver oscReceiver;
};
