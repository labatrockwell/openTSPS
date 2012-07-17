//
//  ofxTSPSopenNIProcessor.cpp
//  openTSPS
//
//  Created by Brett Renfer on 7/16/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#include "ofxTSPSopenNIProcessor.h"

//--------------------------------------------------------------
ofxTSPSopenNIProcessor::ofxTSPSopenNIProcessor(){
    bCanTrackHaar = bTrackHaar = false;
    bCanTrackContours = bTrackContours = true;
    bCanTrackSkeleton = bTrackSkeleton = true;
    bCanTrackOpticalFlow = bTrackOpticalFlow = false;
}

//--------------------------------------------------------------
ofxTSPSopenNIProcessor::~ofxTSPSopenNIProcessor(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::setup( int width, int height, ofxTSPSScene * scene, vector<ofxTSPSPerson*> * peopleVector, float trackingScaleFactor ){
    
    grayPixels.allocate( width, height, 2 );
    
    openNIDevice.setup();
    //openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(5);
    
    openNIDevice.setUseBackgroundDepthSubtraction(true);
    
    openNIDevice.start();
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::draw(){
};

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::setCameraImage( ofBaseImage & image ){
    
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::captureBackground( ofBaseImage & image ){
    openNIDevice.setCaptureBackgroundDepthPixels(true);
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::progressiveBackground( ofBaseImage & image, float amount ){
    //not implemented yet...
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::blankBackground(){
    openNIDevice.setUseBackgroundDepthSubtraction(false);
}

//--------------------------------------------------------------
ofPixelsRef ofxTSPSopenNIProcessor::difference( ofBaseImage & image, TSPSTrackingType trackingType ){
    ofPixelsRef pix = openNIDevice.getDepthPixels();
    // for some reason depth pixels are rgba?
    for ( int i=0; i<pix.getWidth() * pix.getHeight() * pix.getNumChannels(); i+= pix.getNumChannels()){
        grayPixels[i / 4] = pix[i];
    }
    return grayPixels;
};

//--------------------------------------------------------------
ofPixelsRef ofxTSPSopenNIProcessor::process ( ofBaseImage & image ){
    openNIDevice.update();
    ofPixelsRef pix = openNIDevice.getDepthPixels();
    for ( int i=0; i<pix.getWidth() * pix.getHeight() * pix.getNumChannels(); i+= pix.getNumChannels()){
        grayPixels[i / 4] = pix[i];
    }
    return grayPixels;
};

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::setThreshold( float thresh ){
    if ( openNIDevice.getNumDepthThresholds() == 0 ){
        openNIDevice.addDepthThreshold(255, thresh);
    } else {
        openNIDevice.getDepthThreshold(0).set(255, thresh);
    }
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::setBlobSettings( float minimumBlob, float maximumBlob, bool bFindHoles ){
    
}
