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
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
	
	camWidth = 640;
	camHeight = 480;
    bUseVideoFile = false;

    // allocate images + setup people tracker
	//colorImg.allocate(camWidth, camHeight);
    grayImg.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
	
	peopleTracker.setup(camWidth, camHeight);
	peopleTracker.loadFont("fonts/times.ttf", 10);
    ofxAddTSPSListeners(this);
    
    bKinect         = false;
    cameraState     = CAMERA_NOT_INITED;
    
    // are we loading from a video file?
    if ( peopleTracker.useVideoFile() || bUseVideoFile ){
        if ( bUseVideoFile && !peopleTracker.useVideoFile()){
            peopleTracker.setUseVideoFile();            
        } else if ( !bUseVideoFile && peopleTracker.useVideoFile() ){
            bUseVideoFile = peopleTracker.useVideoFile();
        }
        
        initVideoFile();
    } else {
        // no kinects connected, let's just try to set up the device
        if ( peopleTracker.useKinect() ){
            // are there any kinects out there?
            kinect.init();
            bKinectConnected = (kinect.numAvailableDevices() >= 1);
        }
        if (kinect.numAvailableDevices() < 1 || !peopleTracker.useKinect()){
            kinect.clear();        
            bKinect = false;
            initVideoInput();
        } else {
            bKinect = true;
            initVideoInput();
        }
    }    

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
    if (peopleTracker.useKinect() && !bKinect){
        bKinect = true;
        initVideoInput();
    } else if (!peopleTracker.useKinect() && bKinect){
        bKinect = false;
        initVideoInput();
    } else if ( ( peopleTracker.useVideoFile() && !bUseVideoFile ) || ( peopleTracker.useVideoFile() && videoFile != peopleTracker.getVideoFile()) ){
        closeVideoInput();
        bUseVideoFile = initVideoFile();
    } else if ( !peopleTracker.useVideoFile() && bUseVideoFile){
        bUseVideoFile = false;
        cameraState = CAMERA_NOT_INITED;
        initVideoInput();        
    }
    
    bool bNewFrame = false;
    
    if ( cameraState != CAMERA_NOT_INITED){
        if ( cameraState == CAMERA_KINECT ){
            kinect.update();
            bNewFrame = true;//kinect.isFrameNew();
        } else if ( cameraState == CAMERA_VIDEOGRABBER ){
            vidGrabber.grabFrame();
            bNewFrame = vidGrabber.isFrameNew();
        } else if ( cameraState == CAMERA_VIDEOFILE ){
            vidPlayer.idleMovie();
            bNewFrame = true; //vidPlayer.isFrameNew();
        }
    }
    
	if (bNewFrame){
        if ( cameraState == CAMERA_KINECT ){   
			grayImg.setFromPixels(kinect.getDepthPixelsRef());
            peopleTracker.update(grayImg);
        } else if ( cameraState == CAMERA_VIDEOGRABBER ){
            peopleTracker.update(vidGrabber);
        } else if ( cameraState == CAMERA_VIDEOFILE ){
            peopleTracker.update(vidGrabber);
        }
        
		// iterate through the people
		for(int i = 0; i < peopleTracker.totalPeople(); i++){
            ofxTSPS::Person* p = peopleTracker.personAtIndex(i);
            if (cameraState == CAMERA_KINECT){
                // distance is in mm, with the max val being 10 m
                // scale it by max to get it in a 0-1 range
                p->depth = (kinect.getDistanceAt( p->highest )/10000.0);
            } else {
                p->depth = p->highest.z / 255.0f;
            }
		}
	}
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
    if ( cameraState == CAMERA_KINECT){  
        cameraState = CAMERA_NOT_INITED;
        kinect.close();
    } else if ( cameraState == CAMERA_VIDEOGRABBER ){ 
        cameraState = CAMERA_NOT_INITED;
        vidGrabber.close();
    }
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
bool tspsApp::initVideoInput(){
    if ( bKinect && cameraState != CAMERA_KINECT ){
        kinect.init();
        bKinectConnected = (kinect.numAvailableDevices() >= 1);
        if (!bKinectConnected){
            bKinect = false;
            peopleTracker.setUseKinect(false);
            return false;
        }
        
        if ( cameraState == CAMERA_VIDEOGRABBER ){
            vidGrabber.close();
            cameraState = CAMERA_NOT_INITED;
        } else if ( cameraState == CAMERA_VIDEOFILE ){
            vidPlayer.stop();
            vidPlayer.closeMovie();
            vidPlayer.close();
        }
        
        if ( cameraState != CAMERA_KINECT){            
            kinect.init();
            //kinect.setVerbose(true);
            bool bOpened = kinect.open();
            if (bOpened){
                cameraState = CAMERA_KINECT;
                //set this so we can access video settings through the interface
                peopleTracker.setVideoGrabber(&kinect, ofxTSPS::CAMERA_KINECT);
                return true;
            } else {
                peopleTracker.setUseKinect(false);
                return false;
            }
        }        
    } else {      
        if ( cameraState != CAMERA_VIDEOGRABBER ){  
            
            if ( cameraState == CAMERA_KINECT ){
                kinect.close();
                kinect.clear();
                cameraState = CAMERA_NOT_INITED;
            } else if (cameraState == CAMERA_VIDEOFILE ){   
                vidPlayer.stop();
                vidPlayer.closeMovie();   
                vidPlayer.close();             
            }
            
            vidGrabber.setVerbose(false);
            //vidGrabber.videoSettings();
            bool bAvailable = vidGrabber.initGrabber(camWidth,camHeight);
            if (bAvailable){ 
                cameraState = CAMERA_VIDEOGRABBER;
                //set this so we can access video settings through the interface
                peopleTracker.setVideoGrabber(&vidGrabber, ofxTSPS::CAMERA_VIDEOGRABBER);
                return true;
            } else {
                return false;
            }
        }
    }    
}

//--------------------------------------------------------------
void tspsApp::closeVideoInput(){
    if ( cameraState == CAMERA_KINECT ){
        kinect.close();
        kinect.clear();
        cameraState = CAMERA_NOT_INITED;
    } else if ( cameraState == CAMERA_VIDEOGRABBER ){
        vidGrabber.close();
        cameraState = CAMERA_NOT_INITED;
    }
}

//------------------------------------------------------------------------
bool tspsApp::initVideoFile(){    
    videoFile = peopleTracker.getVideoFile();
    vidPlayer = ofVideoPlayer();
    bool loaded = vidPlayer.loadMovie( videoFile );
    
    if ( loaded ){
        vidPlayer.play();
        if ( camWidth != vidPlayer.width || camHeight != vidPlayer.height ){            
            camWidth    = vidPlayer.width;
            camHeight   = vidPlayer.height;
            peopleTracker.resize( camWidth, camHeight );
        }
        
        // reallocate
        grayImg.resize(camWidth, camHeight);
        
        cameraState = CAMERA_VIDEOFILE;        
    }
    
    return loaded;
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
