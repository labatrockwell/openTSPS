//
//  OpenNI.h
//  openTSPS
//
//  Created by rockwell on 9/4/12.
//
//

#pragma once
//
//  Kinect.h
//  openTSPS
//
//  Created by Brett Renfer on 8/7/12.
//
//

#include "ofxOpenNI.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class OpenNI : public Source, public ofxOpenNI {
    public:
        
        OpenNI() : Source(){
            type = CAMERA_OPENNI;
        }
        
        // core
        bool available(){
            return (getNumDevices() >= 1);
        }
        
        void update(){
            ofxOpenNI::update();
        }
        
        bool doProcessFrame(){
            return true;
        }
        
        // required bc ofxOpenNI doesn't declare it
        unsigned char*  getPixels(){
            return getDepthPixels().getPixels();
        }
        
        ofPixelsRef getPixelsRef(){
            return getDepthPixels();
        }
        
        bool openSource( int width, int height, string etc="" ){
            bool bSetup = isContextReady();
            if(!bSetup) bSetup = setup();
            if ( bSetup ){
                addDepthGenerator();
                start();                
            }
            return bSetup;
        }
        
        void closeSource(){
            stop();
        }
    };
}

