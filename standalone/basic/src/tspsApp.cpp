#include "tspsApp.h"
#include "ofAppGLFWWindow.h"

//--------------------------------------------------------------
void tspsApp::setup(){
    // retina support, some day
    ofAppGLFWWindow * window = (ofAppGLFWWindow*) ofGetWindowPtr();
    if ( window->getPixelScreenCoordScale() != 1 ){
        ofSetWindowShape(1024 * window->getPixelScreenCoordScale(), 768 * window->getPixelScreenCoordScale());
    }
    
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