//
//  ofxTSPSofxOpenCvProcessor.h
//  openTSPS
//
//  Created by Brett Renfer on 7/12/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofxTSPSEvents.h"
#include "ofxTSPSProcessor.h"

#include "ofxOpenCv.h"
#include "ofxCvHaarTracker.h"
#include "ofxCvOpticalFlowLK.h"
#include "ofxCvBlobTracker.h"

class ofxTSPSofxOpenCvProcessor : public ofxTSPSProcessor, public ofxCvBlobListener {
public:
    
    ofxTSPSofxOpenCvProcessor();
    
    void setup( int width, int height, ofxTSPSScene * scene, vector<ofxTSPSPerson*> * peopleVector, float trackingScaleFactor=.5 );
    void draw();
    
    // step 0: camera
    void setCameraImage( ofBaseImage & image );
    
    // step 1: background
    void captureBackground( ofBaseImage & image );
    void progressiveBackground( ofBaseImage & image, float amount );
    
    void blankBackground();
    
    // step 2: difference
    ofPixelsRef difference( ofBaseImage & image, TSPSTrackingType trackingType );
    
    // step 3: process
    ofPixelsRef process ( ofBaseImage & image );
    void processOpticalFlow( ofBaseImage & image );
    void processHaar( ofBaseImage & image );
    
    // methods: utils
    void resize( int camWidth, int camHeight );
    
    // settings
    void setThreshold( float thresh = 100.0 );
    void setBlobSettings( float minimumBlob = 0.0, float maximumBlob = 1.0, bool bFindHoles = false );
    void setOpticalflowMinMax( float min = 0.0, float max = 10.0 );
    void setHaarXMLFile( string xmlFile );
    void setHaarPadding( float padding = 0.0 );
    
    // ofxCvBlobListener functions    
    ofxTSPSPerson* getTrackedPerson(int pid);
    void blobOn( int x, int y, int id, int order );
    void blobMoved( int x, int y, int id, int order );
    void blobOff( int x, int y, int id, int order );
    
private:
    ofxCvGrayscaleImage dummyImage;
    
    // haar tracking
    ofxLABCvHaarFinder  haarFinder;
    ofxCvHaarTracker    haarTracker;
    float               haarAreaPadding;
    ofxCvGrayscaleImage haarImage;
    
    // contour tracking 
    ofxCvContourFinder 	contourFinder;
    ofxCvBlobTracker    persistentTracker;
    float               threshold;
    float               minBlobArea;
    float               maxBlobArea;
    bool                bFindHoles;
    
    // optical flow
    ofxCvOpticalFlowLK	opticalFlow;
    ofxCvGrayscaleImage opticalFlowImage, opticalFlowLastImage;
    
    // cv images
    ofxCvGrayscaleImage grayImg;            // stores camera image
    ofxCvGrayscaleImage grayBg;             // bg to subtract
    ofxCvShortImage		floatBgImg;         // progressive background image
    
    ofxCvGrayscaleImage	grayDiff;
};