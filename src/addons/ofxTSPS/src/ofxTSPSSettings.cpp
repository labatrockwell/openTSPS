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
		single->bLearnBackground = false;
		single->bTrackOpticalFlow = false;
		
		single->bLearnBackgroundProgressive = true;
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
		
		single->tuioHost = "localhost";
		single->tuioPort = 3333;
		
		single->tcpPort = 8888;

    }
	return single;
	
}

