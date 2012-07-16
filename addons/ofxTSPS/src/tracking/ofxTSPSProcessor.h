//
//  ofxTSPSProcessor.h
//  openTSPS
//
//  Created by Brett Renfer on 7/12/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "ofxTSPSConstants.h"
#include "ofxTSPSPerson.h"
#include "ofxTSPSScene.h"
#include "ofxTSPSSettings.h"

class ofxTSPSProcessor {
public:
    
    ofxTSPSProcessor(){
        bCanTrackHaar = bTrackHaar = false;
        bCanTrackContours = bTrackContours = false;
        bCanTrackSkeleton = bTrackSkeleton = false;
        bCanTrackOpticalFlow = bTrackOpticalFlow = false;
    };
    
    virtual void setup( int width, int height, ofxTSPSScene * scene, vector<ofxTSPSPerson*> * peopleVector, float trackingScaleFactor=.5 ) = 0;
    virtual void draw(){};
    
    // step 0: set camera image
    virtual void setCameraImage( ofBaseImage & image ) = 0;
    
    // step 1: background
    virtual void captureBackground( ofBaseImage & image ) = 0;
    virtual void progressiveBackground( ofBaseImage & image, float amount ) = 0;
    virtual void blankBackground() = 0;
    
    // step 2: difference
    virtual ofPixelsRef difference( ofBaseImage & image, TSPSTrackingType trackingType ){};
    
    // step 3: process
    virtual ofPixelsRef process ( ofBaseImage & image ){};
    virtual void processOpticalFlow( ofBaseImage & image ){};
    virtual void processHaar( ofBaseImage & image ){};
    
    // settings
    virtual void setThreshold( float thresh = 100.0 ){};
    virtual void setBlobSettings( float minimumBlob = 0.0, float maximumBlob = 1.0, bool bFindHoles = false ){};
    virtual void setOpticalflowMinMax( float min = 0.0, float max = 10.0 ){};
    virtual void setHaarXMLFile( string xmlFile ){};
    virtual void setHaarPadding( float padding = 0.0 ){};
    
    // get capabilities
    virtual bool canTrackHaar (){ return bCanTrackHaar; };
    virtual bool canTrackContours (){ return bCanTrackContours; };
    virtual bool canTrackSkeleton (){ return bCanTrackSkeleton; };
    virtual bool canTrackOpticalFlow (){ return bCanTrackOpticalFlow; };
    
    // methods: settings
    virtual bool setTrackHaar ( bool trackHaar ){
        if ( bCanTrackHaar ){
            bTrackHaar = trackHaar;
        }
        return bTrackHaar;
    };
    
    virtual bool setTrackContours ( bool trackContours ){
        if ( bCanTrackContours ){
            bTrackContours = trackContours;
        }
        return bTrackContours;
    };
    
    virtual bool setTrackSkeleton ( bool trackSkeleton ){
        if ( bCanTrackSkeleton ){
            bTrackSkeleton = true;
        }
        return bTrackSkeleton;
    };
    
    virtual bool setTrackOpticalFlow ( bool trackOpticalFlow ){
        if ( bCanTrackOpticalFlow ){
            bTrackOpticalFlow = trackOpticalFlow;
        }
        return bTrackOpticalFlow;
    };
    
    // methods: utils
    virtual void resize( int camWidth, int camHeight ){};
    
    // methods: views
    //virtual ofBaseImage & getCameraView() = 0;
    //virtual ofBaseImage & getBackgroundView() = 0;
    //virtual ofBaseImage & getProcessedView() = 0;
    
protected:
    int                         tspsWidth, tspsHeight;
    TSPSCameraType              cameraType;
    
    ofxTSPSScene *              scene;
    vector<ofxTSPSPerson*> *    trackedPeople;
    
    // capabilities + settings
    bool bCanTrackHaar, bTrackHaar;
    bool bCanTrackContours, bTrackContours;
    bool bCanTrackSkeleton, bTrackSkeleton;
    bool bCanTrackOpticalFlow, bTrackOpticalFlow;
    
    // amount to scale
    float trackingScale;
};
