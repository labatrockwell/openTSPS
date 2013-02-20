#include "tspsApp.h"

//--------------------------------------------------------------
void tspsApp::setup(){
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
    
    // this needs to get cleaned up!
    ofxXmlSettings defaults;
    if ( defaults.loadFile("settings/defaults.xml") ){
        defaults.pushTag("defaults");
        id = defaults.getValue("deviceIndex", 0);
    }
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