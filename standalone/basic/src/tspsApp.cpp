#include "tspsApp.h"

//--------------------------------------------------------------
void tspsApp::setup(){
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
	
	peopleTracker.setup();
	peopleTracker.loadFont("fonts/times.ttf", 10);
    ofxAddTSPSListeners(this);
    
	//load GUI / interface images
	personEnteredImage.loadImage("graphic/triggers/PersonEntered_Active.png");
	personUpdatedImage.loadImage("graphic/triggers/PersonUpdated_Active.png");
	personLeftImage.loadImage("graphic/triggers/PersonLeft_Active.png");
	statusBar.loadImage("graphic/bottomBar.png");
	background.loadImage("graphic/background.png");
	timesBoldItalic.loadFont("fonts/timesbi.ttf", 16);
    
	drawStatus[0] = 0;
	drawStatus[1] = 0;
	drawStatus[2] = 0;
}

//--------------------------------------------------------------
void tspsApp::update(){
    peopleTracker.update();
}

//--------------------------------------------------------------
//delegate methods for people entering and exiting
void tspsApp::onPersonEntered( ofxTSPS::EventArgs & tspsEvent ){
	//do something with them
	ofLog(OF_LOG_VERBOSE, "person %d of size %f entered!\n", tspsEvent.person->pid, tspsEvent.person->area);
	drawStatus[0] = 10;
}

//--------------------------------------------------------------
void tspsApp::onPersonWillLeave( ofxTSPS::EventArgs & tspsEvent ){
	//do something to clean up
	ofLog(OF_LOG_VERBOSE, "person %d left after being %d frames in the system\n", tspsEvent.person->pid, tspsEvent.person->age);
	drawStatus[2] = 10;
}

//--------------------------------------------------------------
void tspsApp::onPersonUpdated( ofxTSPS::EventArgs & tspsEvent ){
	ofLog(OF_LOG_VERBOSE, "updated %d person\n", tspsEvent.person->pid);
	drawStatus[1] = 10;
}

//--------------------------------------------------------------
void tspsApp::draw(){
    // bg image
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	background.draw(0,0);
    
    // render TSPS interface
	peopleTracker.draw();

	//draw status bar stuff
	statusBar.draw(0,700);
	if (drawStatus[0] > 0){
		drawStatus[0]--;
		personEnteredImage.draw(397,728);
	}
	if (drawStatus[1] > 0){
		drawStatus[1]--;
		personUpdatedImage.draw(533,728);
	}
	if (drawStatus[2] > 0){
		drawStatus[2]--;
		personLeftImage.draw(666,728);
	}

	ofSetColor(0, 169, 157);
	char numPeople[1024];
	sprintf(numPeople, "%i", peopleTracker.totalPeople());
	timesBoldItalic.drawString(numPeople,350,740);
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