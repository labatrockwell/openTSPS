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
#include "ofxTSPS/Events.h"
#include "ofxTSPS/Scene.h"
#include "ofxTSPS/gui/Settings.h"
#include "ofxTSPS/Person.h"

namespace ofxTSPS {
    class Processor {
    public:
        
        Processor(){
            bCanTrackHaar = bTrackHaar = false;
            bCanTrackContours = bTrackContours = false;
            bCanTrackSkeleton = bTrackSkeleton = false;
            bCanTrackOpticalFlow = bTrackOpticalFlow = false;
            trackedPeople = NULL;
        };
        
        virtual void exit(){};
        
        virtual void setup( int width, int height, Scene * _scene, vector<Person*> * peopleVector, float trackingScaleFactor=.125 ){
            // core objects
            tspsWidth   = width;
            tspsHeight  = height;
            scene       = _scene;
            trackedPeople = peopleVector;
            trackingScale = trackingScaleFactor;
            setupProcessor();
        }
        virtual void draw(){};
        
        // step 0: set camera image
        virtual void setCameraImage( ofBaseImage & image ) = 0;
        
        // step 1: background
        virtual void captureBackground( ofBaseImage & image ) = 0;
        virtual ofPixelsRef progressiveBackground( ofBaseImage & image, float amount ) = 0;
        virtual void blankBackground() = 0;
        
        // step 2: difference
        virtual ofPixelsRef difference( ofBaseImage & image, TrackingType trackingType ){};
        
        // step 3: process
        virtual ofPixelsRef process ( ofBaseImage & image ){};
        virtual void processOpticalFlow( ofBaseImage & image ){};
        virtual void processHaar( ofBaseImage & image ){};
        
        // settings
        virtual void setThreshold( float thresh = 100.0 ){
            threshold = thresh;
        };
        
        virtual void setBlobSettings( float minimumBlob = 0.0, float maximumBlob = 1.0, bool findHoles = false ){
            minBlobArea = minimumBlob;
            maxBlobArea = maximumBlob;
            bFindHoles  = findHoles;
        };
        
        virtual void setOpticalflowMinMax( float min = 0.0, float max = 10.0 ){};
        virtual void setHaarXMLFile( string xmlFile ){};
        virtual void setHaarPadding( float padding = 0.0 ){
            haarAreaPadding = padding;
        };
        
        // get capabilities
        // TO-DO: Capabilites turn on/off parts of GUI
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
        
        virtual Person* getTrackedPerson(int pid){
            if ( trackedPeople == NULL ){
                ofLog( OF_LOG_ERROR, "No people vector?");
                return NULL;
            }
            for( int i = 0; i < trackedPeople->size(); i++ ) {
                if( (*trackedPeople)[i]->pid == pid ) {
                    return (*trackedPeople)[i];
                }
            }
            return NULL;
        }
        
        // methods: views
        //virtual ofBaseImage & getCameraView() = 0;
        //virtual ofBaseImage & getBackgroundView() = 0;
        //virtual ofBaseImage & getProcessedView() = 0;
        
    protected:
        // called automatically by setup();
        // use for custom setup of your processor
        virtual void setupProcessor(){};
        
        int                     tspsWidth, tspsHeight;
        CameraType              cameraType;
        
        Scene *              scene;
        vector<Person*> *    trackedPeople;
        
        // capabilities + settings
        bool bCanTrackHaar, bTrackHaar;
        bool bCanTrackContours, bTrackContours;
        bool bCanTrackSkeleton, bTrackSkeleton;
        bool bCanTrackOpticalFlow, bTrackOpticalFlow;
        
        float   haarAreaPadding;    // padding around bound rect to look for haar
        float   trackingScale;  // amount to scale
        
        float   threshold;
        float   minBlobArea;
        float   maxBlobArea;
        bool    bFindHoles;
    };

}