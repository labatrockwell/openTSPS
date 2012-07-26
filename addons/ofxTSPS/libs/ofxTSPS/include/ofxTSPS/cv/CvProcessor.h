//
//  CvProcessor.h
//  openTSPS
//
//  Created by Brett Renfer on 7/22/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include "ofxTSPS/Processor.h"
#include "ofxCv.h"

namespace ofxTSPS {
    class CvProcessor : public Processor {
    public:
        
        CvProcessor();
        
        void exit();
        void draw();
        
        // step 0: set camera image
        void setCameraImage( ofBaseImage & image );
        
        // step 1: background
        void captureBackground( ofBaseImage & image );
        ofPixelsRef progressiveBackground( ofBaseImage & image, float amount );
        void blankBackground();
        
        // step 2: difference
        ofPixelsRef difference( ofBaseImage & image, TrackingType trackingType );
        
        // step 3: process
        ofPixelsRef process ( ofBaseImage & image );
        void processOpticalFlow( ofBaseImage & image );
        void processHaar( ofBaseImage & image );
        
        // settings
        void setOpticalflowMinMax( float min = 0.0, float max = 10.0 );
        void setHaarXMLFile( string xmlFile );
        
        // methods: utils
        void resize( int camWidth, int camHeight );
        
    protected:
        // called automatically by setup();
        // use for custom setup of your processor
        void setupProcessor();
        
        // images
        ofImage cameraImage, cameraSmallImage, backgroundImage, progressiveBackgroundImage, differencedImage;
        
        // ofxCv stuff
        ofxCv::ContourFinder        contourFinder;
        ofxCv::CascadeClassifier    haarFinder;
        vector<cv::Rect>            haarObjects;
        
        // opticalflow     
        ofxCv::FlowFarneback    flow;
        bool bFlowTrackedOnce;
    };
}