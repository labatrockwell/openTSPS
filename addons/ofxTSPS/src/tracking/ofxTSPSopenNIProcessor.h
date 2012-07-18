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
#include "ofxCvBlobTracker.h"

class ofxTSPSopenNIProcessor : public ofxTSPSProcessor {
public:
    
    ofxTSPSopenNIProcessor();
    ~ofxTSPSopenNIProcessor();
    
    void exit();
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
    
    //openNI methods
    ofxOpenNI * getOpenNI(){ return &openNIDevice; };
    
protected:
    void setupProcessor();
    
    ofPixels    grayPixels;
    ofxOpenNI   openNIDevice;
    
    // contour finding 
    ofxCvGrayscaleImage blobImage;
    ofxCvContourFinder  contourFinder;
    
    // openNI events
    void onUserEvent( ofxOpenNIUserEvent & event );
};
