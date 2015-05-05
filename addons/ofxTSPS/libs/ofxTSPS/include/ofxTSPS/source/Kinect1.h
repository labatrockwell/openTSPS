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
			cout << numAvailableDevices() <<" Kinect 1s"<<endl;
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
			cout << isFrameNew() <<" Kinect 1 new"<<endl;
            return isFrameNew();
        }
        
        ofPixelsRef getPixelsRef(){
            return getDepthPixelsRef();
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
