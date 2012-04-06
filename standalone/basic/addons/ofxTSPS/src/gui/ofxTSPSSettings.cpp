/*
 *  ofxTSPSSettings.cpp
 *  TSPSPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 */

#include "ofxTSPSSettings.h"

bool ofxTSPSSettings::instanceFlag = false;
ofxTSPSSettings* ofxTSPSSettings::single = NULL;

ofxTSPSSettings* ofxTSPSSettings::getInstance()
{
    if(! instanceFlag)
    {
        single = new ofxTSPSSettings();
        instanceFlag = true;
        
        // general
        single->bMinimized  = false;
        
        //camera
        single->bUseKinect  = false;
        single->cameraIndex = 0;
        single->inputType   = TSPS_INPUT_UNDEFINED;
        
        // background
		single->bLearnBackground = false;
        single->bBlankBackground = false;
		single->bTrackOpticalFlow = false;
		
		single->bLearnBackgroundProgressive = true;
		single->bAdjustedViewInColor = false;  // ZACK BOKA		
		single->trackType = TRACK_LIGHT;
		single->bLearnBackground = true;
		
		single->threshold = 80;
		single->highpassBlur = 0;
		single->highpassNoise = 0;
		single->highpassAmp = 0;
		single->smooth = 0; 
		
		single->bAmplify = single->bSmooth = single->bHighpass = false;
		single->bFindHoles = true;
		
		single->minBlob = 0.001;
		single->maxBlob = .05;
		
		single->haarFile = "HS.xml";
		
		single->bLearnBackground = true;	
		single->bCentroidDampen = true;		
		
		single->oscHost = "localhost";
		single->oscPort = 12000;
		single->bUseLegacyOsc = false;
		
		single->tuioHost = "localhost";
		single->tuioPort = 3333;
		
		single->tcpPort = 8888;
        
        single->quadWarpScaled.reserve(4);
        single->quadWarpOriginal.reserve(4);
        
        for (int i=0; i<single->quadWarpScaled.size(); i++){
            single->quadWarpScaled[i] = ofPoint(0,0);
            single->quadWarpOriginal[i] = ofPoint(0,0);
        }
    }
	return single;
}


void ofxTSPSSettings::setVideoGrabber( ofBaseVideo* videoGrabber, tspsInputType type){
    if (instanceFlag){
        single->videoGrabber = videoGrabber;
        single->inputType    = type;
    } else {
        ofLog(OF_LOG_ERROR, "settings not initialized?");
    }
};

ofBaseVideo* ofxTSPSSettings::getVideoGrabber(){
    if (instanceFlag){
        return single->videoGrabber;
    } else {
        return NULL;
    }
};

tspsInputType ofxTSPSSettings::getInputType(){
    if (instanceFlag){
        return single->inputType;
    } else {
        return TSPS_INPUT_UNDEFINED;
    }
};

