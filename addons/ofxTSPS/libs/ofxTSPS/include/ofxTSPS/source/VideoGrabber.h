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
        
        bool openSource( int width, int height, string etc="" ){
            setDeviceID(sourceIndex);
            bool setup = false;//initGrabber(width,height);
            return setup;
        }
        
        void update(){
            ofVideoGrabber::update();
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