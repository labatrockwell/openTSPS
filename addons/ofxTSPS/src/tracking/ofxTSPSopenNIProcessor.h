//
//  ofxTSPSopenNIProcessor.h
//  openTSPS
//
//  Created by Brett Renfer on 7/16/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//
#pragma once

#include "ofMain.h"
#include "ofxTSPSopenNIPerson.h"
#include "ofxTSPSProcessor.h"
#include "ofxOpenNI.h"

class ofxTSPSopenNIProcessor : public ofxTSPSProcessor {
public:
    
    ofxTSPSopenNIProcessor();
    ~ofxTSPSopenNIProcessor();
    
    void draw();
    
    // step 0: set camera image
    void setCameraImage( ofBaseImage & image );
    
    // step 1: background
    void captureBackground( ofBaseImage & image );
    void progressiveBackground( ofBaseImage & image, float amount );
    void blankBackground();
    
    // step 2: difference
    ofPixelsRef difference( ofBaseImage & image, TSPSTrackingType trackingType );
    
    // step 3: process
    ofPixelsRef process ( ofBaseImage & image );
    
    // settings
    void setThreshold( float thresh = 100.0 );
    void setBlobSettings( float minimumBlob = 0.0, float maximumBlob = 1.0, bool bFindHoles = false );
    
    //openNI methods
    ofxOpenNI * getOpenNI(){ return &openNIDevice; };
    
protected:
    void setupProcessor();
    
    ofPixels    grayPixels;
    ofxOpenNI   openNIDevice;
    
    // openNI events
    void onUserEvent( ofxOpenNIUserEvent & event );
};
