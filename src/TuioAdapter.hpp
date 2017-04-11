#pragma once
#include "ofxOscSender.h"
#include "ofxOSCReceiver.h"

typedef struct {
    int ID;
    int	parentID;
    double x;
    double y;
    double z;
    bool active;
    double age;
    int impactPartner;
    bool bound;
    double originDistance;
    double parentDistance;
    double angle;
    bool newTrigger;
    bool prevTrigger;
    bool canTrigger;
    int colorCode;
    ofColor color;
    
    ofRectangle rect;
    bool direction;
} Blob;

class TuioAdapter{
    
public:
    
    void setup(std::string trackMasterIP, int trackMasterPort, int myPort){
        oscReceiver.setup(myPort);
        startTime = ofGetElapsedTimef();
        initiateSessionWithTrackMaster(trackMasterIP, trackMasterPort);
    }
    
    void processReceivedOSCMessages(){
        while(oscReceiver.hasWaitingMessages()) {
            ofxOscMessage mes;
            oscReceiver.getNextMessage(mes);
            interpretOSCMessage(mes);
        }
    }
    
private:
    
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
        long blobID = -1;
        double now = ofGetElapsedTimef();
        //	point t;
        //	t.x = t.y = t.z = 0.0;
        
        string tempStr = m.getArgAsString( 0 );
        int numArgs = m.getNumArgs();
        
#pragma mark	alive
        if(!strcmp(tempStr.c_str(), "alive")) {
            fseqStatus = false; // check for empty frames
            
            if(numArgs > 1) {
                //			for(i = 0; i < MAX_BLOBS; i++) {
                //				blobStatus[i] = 0;
                //			}
                maxBlobAlive = -1;
                for(i = 1; i < numArgs; i++) {
                    if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                        blobID = m.getArgAsInt32(i);
                        blobID = blobID % MAX_BLOBS;
                        
                        if(blobs[blobID].active == false) { // a new blob is born
                            blobs[blobID].age = now;
                            blobs[blobID].active = true;
                            //						blobs[blobID].rect.setX(blobs[blobID].x);
                            //						blobs[blobID].rect.setY(blobs[blobID].y);
                            blobs[blobID].canTrigger = true; // the first appearance needed for storage
                        }
                        blobStatus[blobID] = 1;	// set tempblob flags
                        if(maxBlobAlive < blobID) {
                            maxBlobAlive = blobID; // store largest blob alive
                        }
                    }
                }
                // check which ones have gone
                for(i = 0; i < MAX_BLOBS; i++){
                    if(blobStatus[i] == false && blobs[i].active == true) {
                        blobs[i].active = false;
                        blobs[i].newTrigger = true;
                        
                        if(blobs[i].age > minimumAge) { // minimum age reached we can trigger now
                            if( blobs[i].rect.width < 0.0){
                                selectedDirection[i] = -1.;
                            }else{
                                selectedDirection[i] = 1;
                            }
                            double interval = blobs[i].age * 0.005;
                            double centre_y = (blobs[i].rect.y * 720);
                            
                            
                            uint64 R = random();  /* Generate a random integer */
                            double stepSize = ((R % 700) + 200) * 0.0001; // scale to max-range of invTable
                            
                            double dimensions_x = blobs[i].rect.width * 1280;
                            double dimensions_y = blobs[i].rect.y * ((R % 700)+20);
                            
                            
                            int direction = selectedDirection[i];
                            double anglePosition = blobs[i].rect.x * 90.0;
                            anglePosition =  fmod(anglePosition, 360.0);
                            
                            //						cout << "blob:  age " << blobs[i].age << " rect.x " << blobs[i].rect.x << " width " << blobs[i].rect.width << endl;
                            
                            
                        }
                        blobs[i].age = -1.0;
                        blobs[i].active = false;
                        
                    }
                }
            }
        }
        
#pragma mark set
        else if(!strcmp(tempStr.c_str(), "set")) { // format: set ID x y accel-x accel-y
            
            if(numArgs > 3) {
                // blob ID
                if(m.getArgType(1) == OFXOSC_TYPE_INT32) {
                    blobID = m.getArgAsInt32(1);
                    blobID = blobID % MAX_BLOBS; // safety!! wrap to existing array sizes
                }
                if(m.getArgType(2) == OFXOSC_TYPE_FLOAT){
                    blobs[blobID].x = m.getArgAsFloat(2);
                }else if(m.getArgType(2) == OFXOSC_TYPE_INT32) {
                    blobs[blobID].x = (double)m.getArgAsInt32(2);
                }
                
                if(m.getArgType(3) == OFXOSC_TYPE_FLOAT){
                    blobs[blobID].y = m.getArgAsFloat(3);
                }else if(m.getArgType(3) == OFXOSC_TYPE_INT32) {
                    blobs[blobID].y = (double)m.getArgAsInt32(3);
                }
                //			printf("tuio received: set %ld %f %f\n", blobID, blobs[blobID].x, blobs[blobID].y);
                
            }
        }
        
#pragma mark	fseq
        else if(!strcmp(tempStr.c_str(), "fseq")) {
            if(fseqStatus == true) { // there was NO alive part in the message
                for(i = 0; i < MAX_BLOBS; i++) { // loop running the blobs
                    blobs[i].active = false;
                }
                return;
            }
            for(i = 0; i < MAX_BLOBS; i++) { // loop running the blobs
                
                if(blobStatus[i] == true) { // we have a live one
                    
                    // collect min and max coords in x and y and delta between min.x and max.x for direction
                    
                    if(blobs[i].canTrigger){ // store the beginning point
                        blobs[i].rect.setX(blobs[i].x);
                        blobs[i].rect.setY(blobs[i].y);
                        blobs[i].canTrigger = false;
                    }else{ // update the following points, thus growing the rectangle, until we reach end of blobmode
                        
                        blobs[i].rect.setWidth(blobs[i].x - blobs[i].rect.x);
                        blobs[i].rect.setHeight(blobs[i].y - blobs[i].rect.y);
                    }
                    //				printf("blob %ld %f %f\n", i, blobs[i].x, blobs[i].y);
                }
            } // end of running the blobs loop
            
            for(i = 0; i < MAX_BLOBS; i++) {
                blobStatus[i] = 0;
            }
            fseqStatus = true;
        } // end "fseq case"
    }

    ofxOscReceiver oscReceiver;
    
    bool fseqStatus;
    long maxBlobAlive;
    Blob blobs[MAX_BLOBS];
    bool blobStatus[MAX_BLOBS];
    int selectedDirection[MAX_BLOBS];
    double startTime;

    int minimumAge;

    
};
