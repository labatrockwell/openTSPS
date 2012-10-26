#include "tspsApp.h"

//--------------------------------------------------------------
void tspsApp::setup(){
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
	
	peopleTracker.setup();
    // add your custom source
    peopleTracker.setSource(source);
    
    // setup OpenNI source
    source.openSource(640,480);
    
    // add gestures + hands, which we will send as custom data
    source.addGestureGenerator();
    source.addHandsGenerator();
    source.addAllGestures();
    ofAddListener(source.gestureEvent, this, &tspsApp::onOpenNIGesture);
    ofAddListener(source.handEvent, this, &tspsApp::onOpenNIHand);
    
    // setup layout stuff + add this as a TSPS listener
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
    source.update();
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
void tspsApp::onOpenNIGesture( ofxOpenNIGestureEvent & e ){    
    // this is a little clunky right now!
    // you can either make your own string, pass in a vector of strings,
    // or pass in a map that will get passed into a message. open to suggestions!
        
    map<string,string> params;
    params["timestampMillis"]   = ofToString(e.timestampMillis);
    params["progress"]          = ofToString(e.progress);
    params["worldPositionX"]    = ofToString(e.worldPosition.x);
    params["worldPositionY"]    = ofToString(e.worldPosition.y);
    params["positionX"]         = ofToString((float) e.position.x / ofGetWidth());
    params["positionY"]         = ofToString((float) e.position.y / ofGetHeight());
    
    // trigger custom event!
    peopleTracker.triggerCustomEvent( "openNI"+e.gestureName, params );
}

//--------------------------------------------------------------
void tspsApp::onOpenNIHand( ofxOpenNIHandEvent & e ){
    // this is a little clunky right now!
    // you can either make your own string, or pass in a vector of strings
    // that will get passed into a message. open to suggestions!
    
    // only send it if it has a good status
    if ( e.handStatus == HAND_TRACKING_STARTED || e.handStatus == HAND_TRACKING_UPDATED){
        
        map<string,string> params;
        params["timestampMillis"]   = ofToString(e.timestampMillis);
        params["id"]                = ofToString(e.id);
        params["worldPositionX"]    = ofToString(e.worldPosition.x);
        params["worldPositionY"]    = ofToString(e.worldPosition.y);
        params["positionX"]         = ofToString((float) e.position.x / ofGetWidth());
        params["positionY"]         = ofToString((float) e.position.y / ofGetHeight());
        
        // trigger custom event!
        peopleTracker.triggerCustomEvent( "openNIHand", params );        
    } else if ( e.handStatus == HAND_TRACKING_STOPPED ){
        map<string,string> params;
        params["timestampMillis"]   = ofToString(e.timestampMillis);
        params["id"]                = ofToString(e.id);
        params["worldPositionX"]    = ofToString(e.worldPosition.x);
        params["worldPositionY"]    = ofToString(e.worldPosition.y);
        params["positionX"]         = ofToString((float) e.position.x / ofGetWidth());
        params["positionY"]         = ofToString((float) e.position.y / ofGetHeight());
        
        // trigger custom event!
        peopleTracker.triggerCustomEvent( "openNIHandStopped", params );
    }
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