#pragma once
#include "ofxOscReceiver.h"


class OSCReceiver{
    
public:
    void setup(int port){
        receiver.setup(port);
    }
    
    void decue(){
        if(receiver.hasWaitingMessages()){
            ofxOscMessage msg;
            receiver.getNextMessage(msg);
            std::string address = msg.getAddress();
            
            if(address == ""){
                
            }else if(address == ""){
                
            }
        }
    }

private:
    ofxOscReceiver receiver;
};
