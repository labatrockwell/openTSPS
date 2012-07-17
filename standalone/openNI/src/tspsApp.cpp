#include "tspsApp.h"

class TSPSPersonAttributes {
public:
	TSPSPersonAttributes(){
		height = 0;
		hasBeard = false;
	}

	float height;
	bool hasBeard; //this is a joke
};

//--------------------------------------------------------------
void tspsApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
	
	camWidth = 640;
	camHeight = 480;
    niPixels.allocate(camWidth, camHeight, 1);
    grayImg.allocate( camWidth, camHeight );
    
    peopleTracker.setProcessor( new ofxTSPSopenNIProcessor() );
	peopleTracker.setup(camWidth, camHeight);
	peopleTracker.loadFont("fonts/times.ttf", 10);
    
    ofxAddTSPSListeners(this);
    
	peopleTracker.setActiveDimensions( ofGetWidth(), ofGetHeight()-68 );

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
    ofPixelsRef pix  = ((ofxTSPSopenNIProcessor* )peopleTracker.getProcessor())->getOpenNI()->getDepthPixels();
    
    // for some reason depth pixels are rgba?
    for ( int i=0; i<pix.getWidth() * pix.getHeight() * pix.getNumChannels(); i+= pix.getNumChannels()){
        niPixels[i / 4] = pix[i];
    }
    grayImg.setFromPixels(niPixels);
    peopleTracker.update( grayImg );
    
    
    // iterate through the people
    for(int i = 0; i < peopleTracker.totalPeople(); i++){
        ofxTSPSPerson* p = peopleTracker.personAtIndex(i);
        //p->depth = p->highest.z / 255.0f;
    }
}

//--------------------------------------------------------------
//delegate methods for people entering and exiting
void tspsApp::onPersonEntered( ofxTSPSEventArgs & tspsEvent ){
	//do something with them
	ofLog(OF_LOG_VERBOSE, "person %d of size %f entered!\n", tspsEvent.person->pid, tspsEvent.person->area);
	drawStatus[0] = 10;
}

//--------------------------------------------------------------
void tspsApp::onPersonWillLeave( ofxTSPSEventArgs & tspsEvent ){
	//do something to clean up
	ofLog(OF_LOG_VERBOSE, "person %d left after being %d frames in the system\n", tspsEvent.person->pid, tspsEvent.person->age);
	drawStatus[2] = 10;
}

//--------------------------------------------------------------
void tspsApp::onPersonUpdated( ofxTSPSEventArgs & tspsEvent ){
	ofLog(OF_LOG_VERBOSE, "updated %d person\n", tspsEvent.person->pid);
	drawStatus[1] = 10;
}

//--------------------------------------------------------------
void tspsApp::draw(){
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	ofPushStyle();
	background.draw(0,0);
	peopleTracker.draw();

	ofPopStyle();

	//draw status bar stuff

	statusBar.draw(0,700);//ofGetHeight()-statusBar.height);
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
void tspsApp::exit(){
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

//--------------------------------------------------------------
void tspsApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void tspsApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void tspsApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void tspsApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void tspsApp::windowResized(int w, int h){}
