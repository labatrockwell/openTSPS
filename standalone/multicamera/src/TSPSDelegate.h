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
    CAMERA_VIDEOGRABBER,
    CAMERA_VIDEOFILE
};


class TSPSDelegate : public ofxPersonListener{
public:
    // setup which delegate + optionally force using a file instead of live input
    TSPSDelegate( int which = 0, bool bUseVideoFile = false );
    ~TSPSDelegate();
    
    bool openCamera( int which, bool bKinect=false );
    
    void update();
    void draw();
    
    void disableEvents();
    void enableEvents();
    
    // kinect, webcam, or video file?
    bool bKinect, bKinectConnected, bUseVideoFile;
    
    ofxCvColorImage		colorImg;
    ofxCvGrayscaleImage	grayImg;	
    
protected:
	ofxTSPSPeopleTracker peopleTracker;    
    
    void personEntered( ofxTSPSPerson* newPerson, ofxTSPSScene* scene );
    void personMoved( ofxTSPSPerson* activePerson, ofxTSPSScene* scene );
    void personWillLeave( ofxTSPSPerson* leavingPerson, ofxTSPSScene* scene );
    void personUpdated( ofxTSPSPerson* updatedPerson, ofxTSPSScene* scene );
    
    int cameraState;
    
    string videoFile;
    
    ofVideoGrabber 		vidGrabber;
    ofVideoPlayer       vidPlayer;
    ofxKinect           kinect;
    
    int     cameraIndex;
    bool    initVideoInput( int which = 0 );
    bool    initVideoFile();
    void    closeVideoInput();
    int camWidth, camHeight;   
};