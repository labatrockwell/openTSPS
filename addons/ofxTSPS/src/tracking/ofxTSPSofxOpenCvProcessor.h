//
//  ofxTSPSofxOpenCvProcessor.h
//  openTSPS
//
//  Created by Brett Renfer on 7/12/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofxTSPSProcessor.h"

#include "ofxOpenCv.h"
#include "ofxCvHaarTracker.h"
#include "ofxCvOpticalFlowLK.h"
#include "ofxCvBlobTracker.h"

class ofxTSPSofxOpenCvProcessor : public ofxTSPSProcessor, public ofxCvBlobListener {
public:
    
    void setup( ofxTSPSSettings * settings, vector<ofxTSPSPerson*> * peopleVector );
    void captureBackground();
    
    //Call with sequential camera images
    void update( ofxCvColorImage image );
    void update( ofxCvGrayscaleImage image );
    void process();
        
    bool setTrackHaar ( bool trackHaar );
    bool setTrackContours ( bool trackContours );
    bool setTrackSkeleton ( bool trackSkeleton ){ return false; };
    bool setTrackOpticalFlow ( bool trackOpticalFlow );
    
    // ofxCvBlobListener functions    
    void blobOn( int x, int y, int id, int order );
    void blobMoved( int x, int y, int id, int order );
    void blobOff( int x, int y, int id, int order );
    
private:
    
    ofxLABCvHaarFinder	 haarFinder;
    ofxCvHaarTracker    haarTracker;
    ofxCvContourFinder 	contourFinder;
    ofxCvBlobTracker    persistentTracker;
};