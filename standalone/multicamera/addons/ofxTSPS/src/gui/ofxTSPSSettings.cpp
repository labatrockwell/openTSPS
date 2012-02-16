/*
 *  ofxTSPSSettings.cpp
 *  TSPSPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 */

#include "ofxTSPSSettings.h"


ofxTSPSSettings::ofxTSPSSettings() {
    videoGrabber = NULL;
    
    // general
    bMinimized  = false;
    
    //camera
    bUseKinect  = false;
    cameraIndex = 0;
    inputType   = TSPS_INPUT_UNDEFINED;
    
    // background
    bLearnBackground = false;
    bTrackOpticalFlow = false;
    
    bLearnBackgroundProgressive = true;
    bAdjustedViewInColor = false;  // ZACK BOKA		
    trackType = TRACK_LIGHT;
    bLearnBackground = true;
    
    threshold = 80;
    highpassBlur = 0;
    highpassNoise = 0;
    highpassAmp = 0;
    smooth = 0; 
    
    bAmplify = bSmooth = bHighpass = false;
    bFindHoles = true;
    
    minBlob = 0.001;
    maxBlob = .05;
    
    haarFile = "HS.xml";
    
    bLearnBackground = true;	
    bCentroidDampen = true;		
    
    oscHost = "localhost";
    oscPort = 12000;
    bUseLegacyOsc = false;
    
    tuioHost = "localhost";
    tuioPort = 3333;
    
    tcpPort = 8888;
    
    quadWarpScaled.reserve(4);
    quadWarpOriginal.reserve(4);
    
    for (int i=0; i<quadWarpScaled.size(); i++){
        quadWarpScaled[i] = ofPoint(0,0);
        quadWarpOriginal[i] = ofPoint(0,0);
    }
}

void ofxTSPSSettings::setVideoGrabber( ofBaseVideo* videoGrabber, tspsInputType type){
    videoGrabber = videoGrabber;
    inputType    = type;
};

ofBaseVideo* ofxTSPSSettings::getVideoGrabber(){
    return videoGrabber;
};

tspsInputType ofxTSPSSettings::getInputType(){
    return inputType;
};

