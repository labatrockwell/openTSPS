#include "tspsApp.h"

//--------------------------------------------------------------
void tspsApp::setup(){
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
    ofxTSPS::Delegate::setup();
}

//--------------------------------------------------------------
void tspsApp::update(){
    ofxTSPS::Delegate::update();
}

//--------------------------------------------------------------
void tspsApp::draw(){
    ofxTSPS::Delegate::draw();
}

//--------------------------------------------------------------
void tspsApp::keyPressed  (int key){

	switch (key){
		case ' ':{
			peopleTracker.relearnBackground();
		} break;
		case 'f':{
			ofToggleFullscreen();
		} break;
	}
}