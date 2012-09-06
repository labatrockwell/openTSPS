//
//  Processor.cpp
//  openTSPS
//
//  Created by rockwell on 9/5/12.
//
//

#include "ofxTSPS/PeopleTracker.h"
#include "ofxTSPS/Processor.h"

namespace ofxTSPS {
    
    Processor::Processor(){
        bCanTrackHaar = bTrackHaar = false;
        bCanTrackContours = bTrackContours = false;
        bCanTrackSkeleton = bTrackSkeleton = false;
        bCanTrackOpticalFlow = bTrackOpticalFlow = false;
        trackedPeople = NULL;
    };

    void Processor::setup( int width, int height, PeopleTracker * _tracker, float trackingScaleFactor, float haarTrackingScaleFactor ){
        // core objects
        tspsWidth   = width;
        tspsHeight  = height;
        tracker     = _tracker;
        scene       = &tracker->scene;
        trackedPeople = &tracker->trackedPeople;
        trackingScale = trackingScaleFactor;
        haarTrackingScale = haarTrackingScaleFactor;
        setupProcessor();
    }

    // settings
    void Processor::setThreshold( float thresh ){
        threshold = thresh;
    };

    void Processor::setBlobSettings( float minimumBlob, float maximumBlob, bool findHoles ){
        minBlobArea = minimumBlob;
        maxBlobArea = maximumBlob;
        bFindHoles  = findHoles;
    };

    void Processor::setHaarPadding( float padding ){
        haarAreaPadding = padding;
    };

    // methods: settings
    bool Processor::setTrackHaar ( bool trackHaar ){
        if ( bCanTrackHaar ){
            bTrackHaar = trackHaar;
        }
        return bTrackHaar;
    };

    bool Processor::setTrackContours ( bool trackContours ){
        if ( bCanTrackContours ){
            bTrackContours = trackContours;
        }
        return bTrackContours;
    };

    bool Processor::setTrackSkeleton ( bool trackSkeleton ){
        if ( bCanTrackSkeleton ){
            bTrackSkeleton = true;
        }
        return bTrackSkeleton;
    };

    bool Processor::setTrackOpticalFlow ( bool trackOpticalFlow ){
        if ( bCanTrackOpticalFlow ){
            bTrackOpticalFlow = trackOpticalFlow;
        }
        return bTrackOpticalFlow;
    };

    // methods: utils

    Person* Processor::getTrackedPerson(int pid){
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
    
    // call to update Tracker
    void Processor::personEntered( Person * p, Scene * s ){
        tracker->personEntered(p, s);
    }
    
    void Processor::personUpdated( Person * p, Scene * s ){
        tracker->personUpdated(p, s);
    }
    
    void Processor::personWillLeave( Person * p, Scene * s ){
        tracker->personWillLeave(p, s);
    }
}