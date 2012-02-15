//
//  TSPSDelegate.h
//  openTSPS
//
//  Created by rockwell on 2/14/12.
//  Copyright 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxTSPS.h"

// kinect support
#include "ofxKinect.h"

enum {
    CAMERA_NOT_INITED,
    CAMERA_KINECT,
    CAMERA_VIDEOGRABBER
};


class TSPSDelegate : public ofxPersonListener{
public:
    TSPSDelegate( int which = 0 );
    ~TSPSDelegate();
    
    bool openCamera( int which, bool bKinect=false );
    
    void update();
    void draw();
    
    void disableEvents();
    void enableEvents();
    
    // kinect or live video?
    bool bKinect, bKinectConnected;
    
    ofxCvColorImage		colorImg;
    ofxCvGrayscaleImage	grayImg;	
    
private:
	ofxTSPSPeopleTracker peopleTracker;    
    
    void personEntered( ofxTSPSPerson* newPerson, ofxTSPSScene* scene );
    void personMoved( ofxTSPSPerson* activePerson, ofxTSPSScene* scene );
    void personWillLeave( ofxTSPSPerson* leavingPerson, ofxTSPSScene* scene );
    void personUpdated( ofxTSPSPerson* updatedPerson, ofxTSPSScene* scene );
    
    int cameraState;
    
    ofVideoGrabber 		vidGrabber;
    ofxKinect           kinect;
    
    int     cameraIndex;
    bool    initVideoInput( int which = 0 );
    void    closeVideoInput();
    
    int camWidth, camHeight;
    
    
};