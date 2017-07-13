#pragma once

#include "ofxOscSender.h"
#include "ofxSyphon.h"

class SyphonAdapter{
    
public:
    void setup(std::string syphonIP, int syphonPort){
        syphonServer.setName("sonorama");

		fbo.allocate(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA);

        oscSender.setup(syphonIP, syphonPort);
        ofxOscMessage mes;
        mes.setAddress( "/SwitchSyphonClient" );
        mes.addStringArg( "sonoramaDebug" );
        mes.addStringArg( "sonorama" );
        mes.addFloatArg( 1.0 );
        oscSender.sendMessage( mes );
        
    }
	
	void begin()noexcept{
		fbo.begin();
	}
	
	void end() noexcept{
		fbo.end();
	}
	
    void sendToSyphon(){
        syphonServer.publishTexture(&fbo.getTexture());
    }
private:
    ofTexture syphonTexture;
    ofxSyphonServer syphonServer;
    ofxOscSender oscSender;
    std::string sendIP;
    int sendPort;
    ofFbo fbo;
};
