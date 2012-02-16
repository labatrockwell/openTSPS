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
	
    // get number of cameras
    ofxKinect kTester;
    kTester.init();
    
    int numKinects = kTester.numAvailableDevices();
    
    if (numKinects >= 1){
        kTester.clear();
        
        for (int i=0; i<numKinects; i++){
            delegates.push_back( new TSPSDelegate(i) );
            delegates.back()->disableEvents();            
            bool bConnectedSuccessfully = delegates.back()->openCamera( i, true );
            if ( !bConnectedSuccessfully ){
                ofLog( OF_LOG_ERROR, "Kinect "+ofToString(i)+" failed, aborting setup!");
                TSPSDelegate * d = delegates[i];
                delegates.erase( delegates.begin() + i );
                delete d;
                break;
            }
        }        
    } else {
        kTester.clear();
        
        // is there a smart way to count avail cameras?
        // for now: trying up to 4
        for (int i=0; i<MAX_CAMERAS; i++){            
            delegates.push_back( new TSPSDelegate(i) );
            bool bConnectedSuccessfully = delegates.back()->openCamera( i, false );
            delegates.back()->disableEvents();
            if ( !bConnectedSuccessfully ){
                TSPSDelegate * d = delegates[i];
                delegates.erase( delegates.begin() + i );
                delete d;
                break;
            }
        }
    }
    
    // which delegate is getting drawn
    currentDelegate = 0;
    delegates[currentDelegate]->enableEvents();
        
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
    for (int i=0; i<delegates.size(); i++){
        delegates[i]->update();
    }
}

//--------------------------------------------------------------
void tspsApp::draw(){
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	ofPushStyle();
	background.draw(0,0);
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
    
    delegates[currentDelegate]->draw();

    /*
	ofSetColor(0, 169, 157);
	char numPeople[1024];
	sprintf(numPeople, "%i", peopleTracker.totalPeople());
    
	timesBoldItalic.drawString(numPeople,350,740);
     */
}

//--------------------------------------------------------------
void tspsApp::exit(){
}

//--------------------------------------------------------------
void tspsApp::keyPressed  (int key){

	switch (key){
		case ' ':{
			//peopleTracker.relearnBackground();
		} break;
		case 'f':{
			ofToggleFullscreen();
		} break;
        case '+':{
            delegates[currentDelegate]->disableEvents();
            currentDelegate++;
            if (currentDelegate >= delegates.size()){
                currentDelegate = 0;
            }
            delegates[currentDelegate]->enableEvents();
        } break;
        case '-':{
            delegates[currentDelegate]->disableEvents();
            currentDelegate--;
            if (currentDelegate <= 0){
                currentDelegate = delegates.size() - 1;
            }
            delegates[currentDelegate]->enableEvents();
        }
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
