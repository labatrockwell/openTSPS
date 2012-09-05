//
//  OpenNI.h
//  openTSPS
//
//  Created by Brett Renfer on 9/4/12.
//
//

#pragma once


#include "ofxOpenNI.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class OpenNI : public Source, public ofxOpenNI {
    public:
        
        OpenNI() : Source(){
            // type defaults to CAMERA_CUSTOM
            bCanTrackHaar = false;
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

