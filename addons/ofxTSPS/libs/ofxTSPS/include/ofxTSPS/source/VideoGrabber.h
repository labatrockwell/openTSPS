//
//  VideoGrabber.h
//  openTSPS
//
//  Created by Brett Renfer on 8/7/12.
//
//

#pragma once

#include "ofVideoGrabber.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class VideoGrabber : public Source, public ofVideoGrabber {
    public:
        
        VideoGrabber(){
            bCanTrackSkeleton = false;
            setVerbose(false);
            type = CAMERA_VIDEOGRABBER;
        }
        
        int numAvailable(){
            return 1;
        }
        
        bool openSource( int width, int height, string etc="" ){
            setDeviceID(sourceIndex);
            bIsOpen = initGrabber(width,height);
            return bIsOpen;
        }
        
        void update(){
            ofVideoGrabber::update();
#ifdef TARGET_OSX
            if ( bPublishTexture ){
                publishToSyphon( ofVideoGrabber::getTexture() );
            }
#endif
        }
        
        bool doProcessFrame(){
            return isFrameNew();
        }
        
        void closeSource(){
            close();
        }
        
    private:
        
        ofPixels grayPixels;
    };
}