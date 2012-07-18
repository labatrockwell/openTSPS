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
    bCanTrackContours = bTrackContours = false;
    bCanTrackSkeleton = bTrackSkeleton = true;
    bCanTrackOpticalFlow = bTrackOpticalFlow = false;
}

//--------------------------------------------------------------
ofxTSPSopenNIProcessor::~ofxTSPSopenNIProcessor(){
    exit();
}


//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::exit(){
    openNIDevice.stop();    
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::setupProcessor(){
    
    grayPixels.allocate( tspsWidth, tspsHeight, 2 );
    blobImage.allocate(tspsWidth, tspsHeight);
    
    openNIDevice.setup();
    //openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(5);
    openNIDevice.setUsePointCloudsAllUsers(false);
    openNIDevice.setUseMaskPixelsAllUsers(true);
    openNIDevice.setUseMaskTextureAllUsers(true);
    openNIDevice.setMaskPixelFormatAllUsers(OF_PIXELS_MONO);
    
    openNIDevice.setUseBackgroundDepthSubtraction(true);
    openNIDevice.setLogLevel(OF_LOG_ERROR);
    
    ofAddListener(openNIDevice.userEvent, this, &ofxTSPSopenNIProcessor::onUserEvent);
    
    openNIDevice.start();
}

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::draw(){
    /*ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,100);
    blobImage.draw(0,0);
    ofPopStyle();*/
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
    grayPixels = openNIDevice.getDepthPixels().getChannel(0);
    return grayPixels;
};

//--------------------------------------------------------------
ofPixelsRef ofxTSPSopenNIProcessor::process ( ofBaseImage & image ){
    openNIDevice.update();
    
    // update TSPS people
    int nUsers = openNIDevice.getNumTrackedUsers();
    for (int i=0; i<nUsers; i++){
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
        ofxTSPSopenNIPerson * person = (ofxTSPSopenNIPerson *) getTrackedPerson( user.getXnID() );
                
        // update person
        if ( person != NULL ){
            // this is weird
            if ( user.getMaskPixels().size() > 0){ // should prevent weird crash
                blobImage.setFromPixels(user.getMaskPixels());
            }
            int numBlobs = contourFinder.findContours( blobImage, minBlobArea, maxBlobArea, 1, bFindHoles );
            if (numBlobs > 0 ){
                person->updateCentroid(contourFinder.blobs[0].centroid, true);
                person->updateBoundingRect(contourFinder.blobs[0].boundingRect);
                person->updateContour(contourFinder.blobs[0].pts );            
            }
            person->update( user );
            
            ofxTSPSEventArgs args;
            args.person = person;
            args.scene  = scene;
            ofNotifyEvent( ofxTSPSEvents().personUpdated, args, this );
        }
    }
    
    grayPixels = openNIDevice.getDepthPixels().getChannel(0);
    return grayPixels;
};

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::setThreshold( float thresh ){
    if ( openNIDevice.getNumDepthThresholds() == 0 ){
        //openNIDevice.addDepthThreshold(255, thresh);
    } else {
        //openNIDevice.getDepthThreshold(0).set(255, thresh);
    }
};

//--------------------------------------------------------------
void ofxTSPSopenNIProcessor::onUserEvent( ofxOpenNIUserEvent & event ){   
    ofxTSPSopenNIPerson * person = NULL;
    
    switch (event.userStatus) {            
        case USER_TRACKING_STARTED:     
            person = new ofxTSPSopenNIPerson( event.id, trackedPeople->size() );
            trackedPeople->push_back(person);
            
            ofxTSPSEventArgs args;
            args.person = person;
            args.scene  = scene;
            ofNotifyEvent( ofxTSPSEvents().personEntered, args, this );            
            break;
            
        case USER_TRACKING_STOPPED:
            person = (ofxTSPSopenNIPerson*) getTrackedPerson( event.id );
            if ( person != NULL){
                //delete the object and remove it from the vector
                std::vector<ofxTSPSPerson*>::iterator it;
                for(it = trackedPeople->begin(); it != trackedPeople->end(); it++){
                    if((*it)->pid == person->pid){
                        trackedPeople->erase(it);
                        break;
                    }
                }
                
                ofxTSPSEventArgs args;
                args.person = person;
                args.scene  = scene;
                ofNotifyEvent( ofxTSPSEvents().personWillLeave, args, this );
                
                // delete pointer
                delete person;
                return;
            }
            openNIDevice.resetUserTracking(event.id);
            break;
            
        case USER_CALIBRATION_STARTED:
            break;
            
        case USER_CALIBRATION_STOPPED:
            break;
            
        case USER_SKELETON_LOST:
            break;    
            
        case USER_SKELETON_FOUND:
            break;
            
        default:
            break;
    };
}
;