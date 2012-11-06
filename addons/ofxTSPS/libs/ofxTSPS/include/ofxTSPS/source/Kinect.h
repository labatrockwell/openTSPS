//
//  Kinect.h
//  openTSPS
//
//  Created by Brett Renfer on 8/7/12.
//
//

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
        
        void update(){
            ofxKinect::update();
        }
        
        bool doProcessFrame(){
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
