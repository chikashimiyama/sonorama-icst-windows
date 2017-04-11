#pragma once

#include "ofxOscSender.h"
#include "ofxSyphon.h"

class SyphonAdapter{
    
public:
    void setup(std::string syphonIP, int syphonPort){
        oscSender.setup(syphonIP, syphonPort);
        ofxOscMessage mes;
        mes.setAddress( "/SwitchSyphonClient" );
        mes.addStringArg( "sonorama" );
        mes.addStringArg( "sonorama" );
        mes.addFloatArg( 1.0 );
        oscSender.sendMessage( mes );
        texture.allocate(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA);
    }
    
    void sendToSyphon(){
        texture.loadScreenData(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
        syphonServer.publishTexture(&texture);
    }
private:
    ofTexture syphonTexture;
    ofxSyphonServer syphonServer;
    ofxOscSender oscSender;
    std::string sendIP;
    int sendPort;
    ofTexture texture;
};
