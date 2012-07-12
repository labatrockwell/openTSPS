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

class ofxTSPSofxOpenCvProcessor : public ofxTSPSProcessor, public ofxCvBlobListener {
public:
    
    void setup( ofxTSPSSettings * settings, vector<ofxTSPSPerson*> * peopleVector );
    void captureBackground();
    void update( ofBaseHasPixels & image = NULL );
    void process();
    
    bool canTrackHaar (){ return bCanTrackHaar };
    bool canTrackContours (){ return bCanTrackContours };
    bool canTrackSkeleton (){ return bCanTrackSkeleton };
    bool canTrackOpticalFlow (){ return bCanTrackOpticalFlow };
    
    bool setTrackHaar ( bool trackHaar );
    bool setTrackContours ( bool trackContours );
    bool setTrackSkeleton ( bool trackSkeleton ){ return false };
    bool setTrackOpticalFlow ( bool trackOpticalFlow );
    
}