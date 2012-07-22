/*
 *  ofxTSPS::Settings.cpp
 *  openTSPS
 */

#include "ofxTSPS/gui/Settings.h"

namespace ofxTSPS {
    Settings::Settings() {
        videoGrabber = NULL;
        
        // general
        bMinimized  = false;
        
        //camera
        bUseKinect  = false;
        cameraIndex = 0;
        inputType   = CAMERA_UNDEFINED;
        
        // video files
        bUseVideoFile = "";
        videoDirectory = "videos";
        bReloadVideoFiles = true;
        
        // background
        bLearnBackground = false;
        bLearnBackgroundProgressive = true;
        bAdjustedViewInColor = false;		
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
        
        bLearnBackground = true;
        
        oscHost = "localhost";
        oscPort = 12000;
        bUseLegacyOsc = false;
        
        webSocketUseSSL = false;
        
        tuioHost = "localhost";
        tuioPort = 3333;
        
        tcpPort = 8888;
        
        quadWarpScaled.reserve(4);
        quadWarpOriginal.reserve(4);
        
        for (int i=0; i<quadWarpScaled.size(); i++){
            quadWarpScaled[i] = ofPoint(0,0);
            quadWarpOriginal[i] = ofPoint(0,0);
        }
    }
    
    void Settings::setVideoGrabber( ofBaseVideo* videoGrabber, CameraType type){
        videoGrabber = videoGrabber;
        inputType    = type;
    };
    
    ofBaseVideo* Settings::getVideoGrabber(){
        return videoGrabber;
    };
    
    CameraType Settings::getInputType(){
        return inputType;
    };
}

