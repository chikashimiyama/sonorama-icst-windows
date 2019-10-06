#pragma once

#include "ofxOscSender.h"

class SyphonAdapter{
    
public:
    void setup(std::string syphonIP, int syphonPort){
		oscSender.setup(syphonIP, syphonPort);
		
		fbo.allocate(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA);


		//syphonServer.setName("sonorama");
		//syphonServer.publishTexture(&fbo.getTexture());
		//connectSyphon();

    }
	
	/*
	void connectSyphon()  noexcept{
		ofxOscMessage mes;
		mes.setAddress("/SwitchSyphonClient");
		mes.addStringArg(APP_NAME);
		mes.addStringArg("sonorama");
		mes.addFloatArg( 1.0 );
		oscSender.sendMessage( mes ,false);
	}
	*/
	
	void begin()noexcept{
		fbo.begin();
	}
	
	void end() noexcept{
		fbo.end();
	}
	
    void sendToSyphon(){
        //syphonServer.publishTexture(&fbo.getTexture());
    }
private:
    ofTexture syphonTexture;
    //ofxSyphonServer syphonServer;
    ofxOscSender oscSender;
    std::string sendIP;
    int sendPort;
    ofFbo fbo;
};
