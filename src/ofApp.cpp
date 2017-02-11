#include "ofApp.h"
#include "StylizerFactory.hpp"
#include <functional>
#include <array>


void ofApp::glSetup(){
    ofBackground(ofColor::black);
    camera.setNearClip(0.01);
    camera.setFarClip(6000);
    ofSetFrameRate(30);
}

void ofApp::setup(){
    glSetup();
}

void ofApp::update(){
    
}

void ofApp::draw(){
    camera.begin();
    mapDataController.draw();
    camera.end();
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
