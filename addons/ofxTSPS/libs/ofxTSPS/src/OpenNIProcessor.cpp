/*
 *  OpenNIProcessor.cpp
 *  openTSPS
 */

#include "ofxTSPS/openni/OpenNIProcessor.h"

namespace ofxTSPS {
    //--------------------------------------------------------------
    OpenNIProcessor::OpenNIProcessor(){
        bCanTrackHaar = bTrackHaar = false;
        bCanTrackContours = bTrackContours = false;
        bCanTrackSkeleton = bTrackSkeleton = true;
        bCanTrackOpticalFlow = bTrackOpticalFlow = false;
    }
    
    //--------------------------------------------------------------
    OpenNIProcessor::~OpenNIProcessor(){
        exit();
    }
    
    
    //--------------------------------------------------------------
    void OpenNIProcessor::exit(){
        openNIDevice.stop();    
    }
    
    //--------------------------------------------------------------
    void OpenNIProcessor::setupProcessor(){
        
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
        
        ofAddListener(openNIDevice.userEvent, this, &OpenNIProcessor::onUserEvent);
        
        openNIDevice.start();
    }
    
    //--------------------------------------------------------------
    void OpenNIProcessor::draw(){
        /*ofPushStyle();
         ofEnableAlphaBlending();
         ofSetColor(255,100);
         blobImage.draw(0,0);
         ofPopStyle();*/
    };
    
    //--------------------------------------------------------------
    void OpenNIProcessor::setCameraImage( ofBaseImage & image ){
        
    }
    
    //--------------------------------------------------------------
    void OpenNIProcessor::captureBackground( ofBaseImage & image ){
        openNIDevice.setCaptureBackgroundDepthPixels(true);
    }
    
    //--------------------------------------------------------------
    void OpenNIProcessor::progressiveBackground( ofBaseImage & image, float amount ){
        //not implemented yet...
    }
    
    //--------------------------------------------------------------
    void OpenNIProcessor::blankBackground(){
        openNIDevice.setUseBackgroundDepthSubtraction(false);
    }
    
    //--------------------------------------------------------------
    ofPixelsRef OpenNIProcessor::difference( ofBaseImage & image, TrackingType trackingType ){
        grayPixels = openNIDevice.getDepthPixels().getChannel(0);
        return grayPixels;
    };
    
    //--------------------------------------------------------------
    ofPixelsRef OpenNIProcessor::process ( ofBaseImage & image ){
        openNIDevice.update();
        
        // update TSPS people
        int nUsers = openNIDevice.getNumTrackedUsers();
        for (int i=0; i<nUsers; i++){
            ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
            OpenNIPerson * person = (OpenNIPerson *) getTrackedPerson( user.getXnID() );
            
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
                
                EventArgs args;
                args.person = person;
                args.scene  = scene;
                ofNotifyEvent( Events().personUpdated, args, this );
            }
        }
        
        grayPixels = openNIDevice.getDepthPixels().getChannel(0);
        return grayPixels;
    };
    
    //--------------------------------------------------------------
    void OpenNIProcessor::setThreshold( float thresh ){
        if ( openNIDevice.getNumDepthThresholds() == 0 ){
            //openNIDevice.addDepthThreshold(255, thresh);
        } else {
            //openNIDevice.getDepthThreshold(0).set(255, thresh);
        }
    };
    
    //--------------------------------------------------------------
    void OpenNIProcessor::onUserEvent( ofxOpenNIUserEvent & event ){   
        OpenNIPerson * person = NULL;
        
        switch (event.userStatus) {            
            case USER_TRACKING_STARTED:     
                person = new OpenNIPerson( event.id, trackedPeople->size() );
                trackedPeople->push_back(person);
                
                EventArgs args;
                args.person = person;
                args.scene  = scene;
                ofNotifyEvent( Events().personEntered, args, this );            
                break;
                
            case USER_TRACKING_STOPPED:
                person = (OpenNIPerson*) getTrackedPerson( event.id );
                if ( person != NULL){
                    //delete the object and remove it from the vector
                    std::vector<Person*>::iterator it;
                    for(it = trackedPeople->begin(); it != trackedPeople->end(); it++){
                        if((*it)->pid == person->pid){
                            trackedPeople->erase(it);
                            break;
                        }
                    }
                    
                    EventArgs args;
                    args.person = person;
                    args.scene  = scene;
                    ofNotifyEvent( Events().personWillLeave, args, this );
                    
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
}