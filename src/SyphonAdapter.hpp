#pragma once

#include "ofxOscSender.h"
#include "ofxSyphon.h"

class SyphonAdapter{
    
public:
    void setup(std::string ip, int port){
        ofxOscMessage mes;
        mes.setAddress( "/SwitchSyphonClient" );
        mes.addStringArg( "sonorama" );
        mes.addStringArg( "sonorama" );
        mes.addFloatArg( 1.0 );
        oscSender.sendMessage( mes );
        sendIP = ip;
        sendPort = port;
    }
    
private:
    ofTexture syphonTexture;
    ofxSyphonServer syphonServer;
    ofxOscSender oscSender;
    std::string sendIP;
    int sendPort;
};
