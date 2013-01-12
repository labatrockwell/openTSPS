/*
 *  ofxTSPS::Settings.cpp
 *  openTSPS
 */

#include "ofxTSPS/gui/Settings.h"

namespace ofxTSPS {
    Settings::Settings() {
        source = NULL;
        
        // general
        bMinimized  = false;
        
        //camera
        //bUseKinect  = false;
        cameraIndex = 0;
        inputType   = CAMERA_UNDEFINED;
        
        // video files
        videoDirectory = "videos";
        bReloadVideoFiles = true;
        
        // image vars
        bFlipX = bFlipY = bInvert = false;
        
        // background
        bLearnBackground            = false;
        captureSeconds              = 1;
        bLearnBackgroundOnStartup   = false;
        bStartupBgCaptured          = false;
        bLearnBackgroundProgressive = true;
        bAdjustedViewInColor        = false;		
        trackType = TRACK_LIGHT;
        
        threshold = 80;
        highpassBlur = 0;
        highpassNoise = 0;
        highpassAmp = 0;
        smooth = 0; 
        
        bAmplify = bSmooth = bHighpass = false;
        bFindHoles = true;
        
        minBlob = 0.001;
        maxBlob = .05;
        
        haarFile = "HS.xml";
        bTrackSkeleton      = false;
        bTrackOpticalFlow   = false;
        bDetectHaar         = false;
        
        oscHost = "localhost";
        oscPort = 12000;
        bUseLegacyOsc = false;
        
        webSocketUseSSL = false;
        
        spacebrewHost   = "localhost";
        bSendSpacebrew  = false;
        
        tuioHost = "localhost";
        tuioPort = 3333;
        
        tcpPort = 8888;
        
        for (int i=0; i<4; i++){
            quadWarpScaled.push_back(ofPoint(0,0));
            quadWarpOriginal.push_back(ofPoint(0,0));
        }
    }
    
    
    Source* Settings::getSource(){
        return source;
    }
    
    void Settings::setSource( Source* _source ){
        source = _source;
        inputType = source->getType();
    };
    
    SourceType Settings::getInputType(){
        return inputType;
    };
}

