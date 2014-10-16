//
//  Kinect.h
//  openTSPS
//
//  Created by Brett Renfer on 8/7/12.
//
//
#pragma once

#include "ofxKinect.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class Kinect : public Source, public ofxKinect {
    public:
                
        Kinect() : Source(){
            type = CAMERA_KINECT;
        }
        
        // core
        bool available(){
            if (!bGrabberInited) init();
            return (numAvailableDevices() >= 1);
        }
        
        int numAvailable(){
            if (!bGrabberInited) init();
            return numAvailableDevices();
        };
        
        void update(){
            ofxKinect::update();
#ifdef TARGET_OSX
            if ( bPublishTexture ){
                publishToSyphon( ofxKinect::getTextureReference() );
            }
#endif
        }
        
        bool doProcessFrame(){
            return isFrameNew();
        }
        
        ofPixels & getPixels(){
            return getDepthPixels();
        }
        
        bool openSource( int width, int height, string etc="" ){
            if (!bGrabberInited) init();
            bIsOpen = open( sourceIndex );
            return bIsOpen;
        }
        
        //void update();
        void closeSource(){
            close();
            clear();
        }
    };
}
