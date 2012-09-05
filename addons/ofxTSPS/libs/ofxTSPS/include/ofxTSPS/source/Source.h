//
//  Source.h
//  openTSPS
//
//  Created by Brett Renfer on 7/26/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxTSPS/Constants.h"

namespace ofxTSPS {
    class Source :  virtual public ofBaseHasPixels {
    public:
        Source(){
            type = CAMERA_CUSTOM;
            // init default settings
            bCanTrackContours       = true;
            bCanTrackHaar           = true;
            bCanTrackOpticalFlow    = true;
            bCanTrackSkeleton       = true;
        }
        
        // get capabilities
        virtual bool canTrackHaar (){ return bCanTrackHaar; };
        virtual bool canTrackContours (){ return bCanTrackContours; };
        virtual bool canTrackSkeleton (){ return bCanTrackSkeleton; };
        virtual bool canTrackOpticalFlow (){ return bCanTrackOpticalFlow; };
        virtual SourceType getType(){ return type; };
        
        // core
        virtual bool available(){ return true; };
        virtual bool openSource( int width, int height, string etc="" ){ return true; };
        virtual void update(){};
        virtual bool doProcessFrame(){ return true; };
        virtual void closeSource(){};;
        
        // methods: settings
        virtual bool setTrackHaar ( bool trackHaar ){
            if ( bCanTrackHaar ){
                bTrackHaar = trackHaar;
            }
            return bTrackHaar;
        };
        
        virtual bool setTrackContours ( bool trackContours ){
            if ( bCanTrackContours ){
                bTrackContours = trackContours;
            }
            return bTrackContours;
        };
        
        virtual bool setTrackSkeleton ( bool trackSkeleton ){
            if ( bCanTrackSkeleton ){
                bTrackSkeleton = trackSkeleton;
            }
            return bTrackSkeleton;
        };
        
        virtual bool setTrackOpticalFlow ( bool trackOpticalFlow ){
            if ( bCanTrackOpticalFlow ){
                bTrackOpticalFlow = trackOpticalFlow;
            }
            return bTrackOpticalFlow;
        };
        
    protected:
        SourceType type;
        
        // mirrored by ofxTSPS::Processor
        // capabilities + settings
        bool bCanTrackHaar, bTrackHaar;
        bool bCanTrackContours, bTrackContours;
        bool bCanTrackSkeleton, bTrackSkeleton;
        bool bCanTrackOpticalFlow, bTrackOpticalFlow;
    };
}
