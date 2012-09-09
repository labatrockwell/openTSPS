//
//  ofxTSPS::Processor.h
//  openTSPS
//
//  Created by Brett Renfer on 7/12/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "ofxTSPS/Constants.h"
#include "ofxTSPS/TSPSEvents.h"
#include "ofxTSPS/Scene.h"
#include "ofxTSPS/gui/Settings.h"
#include "ofxTSPS/Person.h"

namespace ofxTSPS {
    class PeopleTracker;
    class Processor {
        friend class PeopleTracker;
    public:
        
        Processor();
        
        virtual void exit(){};
        
        virtual void setup( int width, int height, PeopleTracker * _tracker, float trackingScaleFactor=.5, float haarTrackingScaleFactor=.125 );
        virtual void draw(){};
        
        // step 0: set camera image
        virtual void setCameraImage( ofBaseImage & image ) = 0;
        
        // step 1: background
        virtual void captureBackground( ofBaseImage & image ) = 0;
        virtual ofPixelsRef progressiveBackground( ofBaseImage & image, float amount ) = 0;
        virtual void blankBackground() = 0;
        
        // step 2: difference
        virtual ofPixelsRef difference( ofBaseImage & image, TrackingType trackingType ) = 0;
        
        // step 3: process
        virtual ofPixelsRef process ( ofBaseImage & image ) = 0;
        virtual void processOpticalFlow( ofBaseImage & image ){};
        virtual void processHaar( ofBaseImage & image ){};
        
        // settings
        virtual void setThreshold( float thresh = 100.0 );
        
        virtual void setBlobSettings( float minimumBlob = 0.0, float maximumBlob = 1.0, bool findHoles = false );
        
        virtual void setOpticalflowMinMax( float min = 0.0, float max = 10.0 ){};
        virtual void setHaarXMLFile( string xmlFile ){};
        virtual void setHaarPadding( float padding = 0.0 );
        
        // get capabilities
        virtual bool canTrackHaar (){ return bCanTrackHaar; };
        virtual bool canTrackContours (){ return bCanTrackContours; };
        virtual bool canTrackSkeleton (){ return bCanTrackSkeleton; };
        virtual bool canTrackOpticalFlow (){ return bCanTrackOpticalFlow; };
        
        // methods: settings
        virtual bool setTrackHaar ( bool trackHaar );        
        virtual bool setTrackContours ( bool trackContours );        
        virtual bool setTrackSkeleton ( bool trackSkeleton );        
        virtual bool setTrackOpticalFlow ( bool trackOpticalFlow );
        
        // methods: utils
        virtual void resize( int camWidth, int camHeight ){};
        virtual Person* getTrackedPerson(int pid);
        
        // methods: views
        //virtual ofBaseImage & getCameraView() = 0;
        //virtual ofBaseImage & getBackgroundView() = 0;
        //virtual ofBaseImage & getProcessedView() = 0;
        
    protected:
        // called automatically by setup();
        // use for custom setup of your processor
        virtual void setupProcessor(){};
        
        int                     tspsWidth, tspsHeight;
        SourceType              cameraType;
        
        PeopleTracker *      tracker;
        Scene *              scene;
        vector<Person*> *    trackedPeople;
        
        // call events on tracker
        virtual void personEntered( Person * p, Scene * s );
        virtual void personUpdated( Person * p, Scene * s );
        virtual void personWillLeave( Person * p, Scene * s );
        
        // capabilities + settings
        bool bCanTrackHaar, bTrackHaar;
        bool bCanTrackContours, bTrackContours;
        bool bCanTrackSkeleton, bTrackSkeleton;
        bool bCanTrackOpticalFlow, bTrackOpticalFlow;
        
        float   haarAreaPadding;    // padding around bound rect to look for haar
        float   trackingScale, haarTrackingScale;  // amount to scale
        
        float   threshold;
        float   minBlobArea;
        float   maxBlobArea;
        bool    bFindHoles;
    };

}