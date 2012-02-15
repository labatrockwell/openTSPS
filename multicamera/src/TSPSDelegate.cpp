//
//  TSPSDelegate.cpp
//  openTSPS
//
//  Created by rockwell on 2/14/12.
//  Copyright 2012 Rockwell Group. All rights reserved.
//

#include "TSPSDelegate.h"

//------------------------------------------------------------------------
TSPSDelegate::TSPSDelegate( int which ){
	camWidth = 640;
	camHeight = 480;
    
    // allocate images + setup people tracker
	colorImg.allocate(camWidth, camHeight);
    grayImg.allocate(camWidth, camHeight);
	
	peopleTracker.setup(camWidth, camHeight, "settings/settings"+ofToString(which)+".xml");
	peopleTracker.loadFont("fonts/times.ttf", 10);
	peopleTracker.setListener( this );
    
    bKinect         = false;
    cameraState     = CAMERA_NOT_INITED;
    
	peopleTracker.setActiveDimensions( ofGetWidth(), ofGetHeight()-68 );
}

//------------------------------------------------------------------------
TSPSDelegate::~TSPSDelegate(){
};

//------------------------------------------------------------------------
bool TSPSDelegate::openCamera( int which, bool _bKinect ){
    bKinect = _bKinect;
    return initVideoInput( which );
};

//------------------------------------------------------------------------
void TSPSDelegate::update(){
    if (peopleTracker.useKinect() && !bKinect){
        bKinect = true;
        initVideoInput( cameraIndex );
    } else if (!peopleTracker.useKinect() && bKinect){
        bKinect = false;
        initVideoInput( cameraIndex );
    }
    
    bool bNewFrame = false;
    
    if ( cameraState != CAMERA_NOT_INITED){
        if ( cameraState == CAMERA_KINECT ){
            kinect.update();
            bNewFrame = true;//kinect.isFrameNew();
        } else {
            vidGrabber.grabFrame();
            bNewFrame = vidGrabber.isFrameNew();
        }
    }   
    
	if (bNewFrame){     
        if ( cameraState == CAMERA_KINECT ){   
			grayImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
			colorImg = grayImg;
            peopleTracker.update(grayImg);
        } else {
            colorImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);
            peopleTracker.update(colorImg);
        }
        
		//iterate through the people
		for(int i = 0; i < peopleTracker.totalPeople(); i++){
			ofxTSPSPerson* p = peopleTracker.personAtIndex(i);
            if (cameraState == CAMERA_KINECT) p->depth = kinect.getDistanceAt( p->centroid );
		}
	}
}

//------------------------------------------------------------------------
void TSPSDelegate::draw(){    
	peopleTracker.draw();
}

//------------------------------------------------------------------------
void TSPSDelegate::disableEvents(){
    peopleTracker.disableGuiEvents();
};

//------------------------------------------------------------------------
void TSPSDelegate::enableEvents(){
    peopleTracker.enableGuiEvents();    
};

//------------------------------------------------------------------------
void TSPSDelegate::personEntered( ofxTSPSPerson* newPerson, ofxTSPSScene* scene ){    
	//do something with them
	ofLog(OF_LOG_VERBOSE, "person %d of size %f entered!\n", newPerson->pid, newPerson->area);
}

//------------------------------------------------------------------------
void TSPSDelegate::personMoved( ofxTSPSPerson* activePerson, ofxTSPSScene* scene ){
    
	//do something with the moving person
	ofLog(OF_LOG_VERBOSE, "person %d of moved to (%f,%f)!\n", activePerson->pid, activePerson->boundingRect.x, activePerson->boundingRect.y);
}

//------------------------------------------------------------------------
void TSPSDelegate::personWillLeave( ofxTSPSPerson* leavingPerson, ofxTSPSScene* scene ){
	//do something to clean up
	ofLog(OF_LOG_VERBOSE, "person %d left after being %d frames in the system\n", leavingPerson->pid, leavingPerson->age);
}

//------------------------------------------------------------------------
void TSPSDelegate::personUpdated( ofxTSPSPerson* updatedPerson, ofxTSPSScene* scene ){
	ofLog(OF_LOG_VERBOSE, "updated %d person\n", updatedPerson->pid);
}

//------------------------------------------------------------------------
bool TSPSDelegate::initVideoInput( int which ){
    bool bNewCameraIndex = false;
    
    if ( cameraIndex != which ) bNewCameraIndex = true;
    cameraIndex = which;
    
    if ( bKinect && !cameraState == CAMERA_KINECT || bKinect && bNewCameraIndex ){
        // not inited yet
        if (cameraState != CAMERA_KINECT){
            kinect.init();
        // just switching cameras
        } else {
            kinect.close();
        }
        bKinectConnected = (kinect.numTotalDevices() >= cameraIndex);
        if (!bKinectConnected){
            bKinect = false;
            ofLog( OF_LOG_ERROR, "NO KINECT AT THIS INDEX!");
            return false;
        }
        
        if ( cameraState == CAMERA_VIDEOGRABBER ){
            vidGrabber.close();
            cameraState = CAMERA_NOT_INITED;
        }
            
        kinect.init();
        kinect.setVerbose(true);
        ofLog(OF_LOG_VERBOSE, "opening Kinect %d", which);
        bool bOpened = kinect.open( which );
        if (bOpened){
            cameraState = CAMERA_KINECT;
            //set this so we can access video settings through the interface
            peopleTracker.setVideoGrabber(&kinect, TSPS_INPUT_KINECT);
        }
        
        return bOpened;
    } else {      
        if ( cameraState == CAMERA_NOT_INITED || cameraState == CAMERA_KINECT || cameraState == CAMERA_VIDEOGRABBER && bNewCameraIndex){
            
            if ( cameraState == CAMERA_KINECT ){
                kinect.close();
                kinect.clear();
                cameraState = CAMERA_NOT_INITED;
            }
            
            if (cameraState == CAMERA_VIDEOGRABBER && bNewCameraIndex) vidGrabber.close();
            
            vidGrabber.setVerbose(false);
            vidGrabber.setDeviceID( which );
            vidGrabber.videoSettings();
            bool bAvailable = vidGrabber.initGrabber(camWidth,camHeight);
            if (bAvailable){ 
                cameraState = CAMERA_VIDEOGRABBER;
                //set this so we can access video settings through the interface
                peopleTracker.setVideoGrabber(&vidGrabber, TSPS_INPUT_VIDEO);
            }
            return bAvailable;
        }
        return true; // already inited, good to go
    }
    
};

//--------------------------------------------------------------
void TSPSDelegate::closeVideoInput(){
    if ( cameraState == CAMERA_KINECT ){
        kinect.close();
        kinect.clear();
        cameraState = CAMERA_NOT_INITED;
    } else if ( cameraState == CAMERA_VIDEOGRABBER ){
        vidGrabber.close();
        cameraState = CAMERA_NOT_INITED;
    }
}