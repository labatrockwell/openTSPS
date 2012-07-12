//
//  ofxTSPSProcessor.h
//  openTSPS
//
//  Created by Brett Renfer on 7/12/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofMain.h"

class ofxTSPSProcessor {
public:
    
    virtual void setup( ofxTSPSSettings * settings, vector<ofxTSPSPerson*> * peopleVector );
    virtual void captureBackground();
    virtual void update( ofBaseHasPixels & image = NULL );
    virtual void process();
    
    bool canTrackHaar (){ return bCanTrackHaar };
    bool canTrackContours (){ return bCanTrackContours };
    bool canTrackSkeleton (){ return bCanTrackSkeleton };
    bool canTrackOpticalFlow (){ return bCanTrackOpticalFlow };
    
    virtual bool setTrackHaar ( bool trackHaar );
    virtual bool setTrackContours ( bool trackContours );
    virtual bool setTrackSkeleton ( bool trackSkeleton );
    virtual bool setTrackOpticalFlow ( bool trackOpticalFlow );
    
private:
    ofxTSPSSettings *p_Settings;
    vector<ofxTSPSPerson*> * trackedPeople;
    
    bool bCanTrackHaar, bTrackHaar;
    bool bCanTrackContours, bTrackContours;
    bool bCanTrackSkeleton, bTrackSkeleton;
    bool bCanTrackOpticalFlow, bTrackOpticalFlow;
    
};
