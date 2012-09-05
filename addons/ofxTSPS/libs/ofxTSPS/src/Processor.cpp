//
//  Processor.cpp
//  openTSPS
//
//  Created by rockwell on 9/5/12.
//
//

#include "ofxTSPS/Processor.h"

namespace ofxTSPS {

    Processor::Processor(){
        bCanTrackHaar = bTrackHaar = false;
        bCanTrackContours = bTrackContours = false;
        bCanTrackSkeleton = bTrackSkeleton = false;
        bCanTrackOpticalFlow = bTrackOpticalFlow = false;
        trackedPeople = NULL;
    };

    virtual void Processor::setup( int width, int height, PeopleTracker * _tracker, float trackingScaleFactor, float haarTrackingScaleFactor ){
        // core objects
        tspsWidth   = width;
        tspsHeight  = height;
        tracker     = _tracker;
        scene       = tracker->scene;
        trackedPeople = tracker->peopleVector;
        trackingScale = trackingScaleFactor;
        haarTrackingScale = haarTrackingScaleFactor;
        setupProcessor();
    }

    // settings
    virtual void Processor::setThreshold( float thresh ){
        threshold = thresh;
    };

    virtual void Processor::setBlobSettings( float minimumBlob, float maximumBlob, bool findHoles ){
        minBlobArea = minimumBlob;
        maxBlobArea = maximumBlob;
        bFindHoles  = findHoles;
    };

    virtual void Processor::setHaarPadding( float padding ){
        haarAreaPadding = padding;
    };

    // get capabilities
    // TO-DO: Capabilites turn on/off parts of GUI
    virtual bool Processor::canTrackHaar (){ return bCanTrackHaar; };
    virtual bool Processor::canTrackContours (){ return bCanTrackContours; };
    virtual bool Processor::canTrackSkeleton (){ return bCanTrackSkeleton; };
    virtual bool Processor::canTrackOpticalFlow (){ return bCanTrackOpticalFlow; };

    // methods: settings
    virtual bool Processor::setTrackHaar ( bool trackHaar ){
        if ( bCanTrackHaar ){
            bTrackHaar = trackHaar;
        }
        return bTrackHaar;
    };

    virtual bool Processor::setTrackContours ( bool trackContours ){
        if ( bCanTrackContours ){
            bTrackContours = trackContours;
        }
        return bTrackContours;
    };

    virtual bool Processor::setTrackSkeleton ( bool trackSkeleton ){
        if ( bCanTrackSkeleton ){
            bTrackSkeleton = true;
        }
        return bTrackSkeleton;
    };

    virtual bool Processor::setTrackOpticalFlow ( bool trackOpticalFlow ){
        if ( bCanTrackOpticalFlow ){
            bTrackOpticalFlow = trackOpticalFlow;
        }
        return bTrackOpticalFlow;
    };

    // methods: utils

    virtual Person* Processor::getTrackedPerson(int pid){
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
}