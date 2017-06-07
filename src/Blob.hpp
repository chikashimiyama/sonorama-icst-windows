#pragma once

class Blob {
public:

	Blob():
	age(0),
	absence(0){};
	
    int ID; // ID provided by tuio, the first number represents the screen
	int age; // the age of blob, young blob should not be trusted
	int absence; // if this point exceeds the threshold, considered as dead.
	long frameOfBirth;
	ofPoint position;
	
	void draw(){
		if(age > BLOB_MATURITY_THRESH){
			ofSetColor(ofColor::blue); // matured
		}else if(absence > 2){
			ofSetColor(ofColor::red); // deadly
		}else{
			ofSetColor(ofColor::orange);
		}
		ofDrawCircle(position, BLOB_SIZE);
	}
};
