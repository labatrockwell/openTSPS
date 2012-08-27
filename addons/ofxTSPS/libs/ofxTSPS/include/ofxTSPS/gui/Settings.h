/*
 *  ofxTSPS::Settings.h
 *  TSPSPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 
 Portions of this software are distributed under the GPL and the MIT license.
 
 The MIT License
 
 Copyright (c) 2010 Rockwell Group LLC
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

#pragma  once
#include "ofMain.h"
#include "ofxTSPS/Constants.h"

namespace ofxTSPS {
    class Settings {
    public:
        Settings();
        
        string haarFile;
        
        // TSPS general vars
        bool    bMinimized;
        
        // camera vars
        //bool    bUseKinect;
        int     cameraIndex;
        
        // video file vars
        string  videoDirectory;
        string  videoFile;
        bool    bReloadVideoFiles;
        
        // image vars
        int     smooth;
        int     highpassBlur;
        int     highpassNoise;
        int     highpassAmp;
        int     threshold;	
        bool    bHighpass;
        bool    bAmplify;
        bool    bSmooth;
        bool    bLearnBackground;
        bool    bBlankBackground;
        bool    bAdjustedViewInColor;	
        
        // tracking vars
        bool    bLearnBackgroundProgressive;
        float   fLearnRate;
        int     trackType;
        bool    bFindHoles;
        float   minBlob;
        float   maxBlob;
        
        //sensing vars
        bool    bTrackOpticalFlow;
        float   minOpticalFlow;
        float   maxOpticalFlow;
        bool    bDetectHaar;
        float   haarAreaPadding;
        bool    bTrackSkeleton;
        //	float   minHaarArea;
        //	float   maxHaarArea;
        
        // communication vars
        string  oscPrefix;
        bool    bSendOsc;
        bool	bUseLegacyOsc;
        bool    bSendTuio;
        bool    bSendTcp;
        
        bool    bSendWebSocketServer;
        bool    bSendWebSocketClient;
        string  webSocketHost;
        int     webSocketPort;
        bool    webSocketUseSSL;
        string  webSocketChannel;
        int     webSocketServerPort;
        
        bool    bSendOscContours;
        string  oscHost;
        int     oscPort;
        string  tuioHost;
        int     tuioPort;
        int     tcpPort;
        
        // video grabber
        void setVideoGrabber( ofBaseVideo* videoGrabber, SourceType type);
        ofBaseVideo*    getVideoGrabber();
        SourceType      getInputType();
        
        //layout vars
        int lastCurrentPanel;
        int currentPanel;
        
        //quad warping
        vector <ofPoint> quadWarpScaled;
        vector <ofPoint> quadWarpOriginal;
        
        //xml file
        vector <string> xmlFiles;
        string currentXmlFile;
        SourceType      inputType;
        
    private:
        
        //JG added the video grabber so we can access settings
        //may be NULL so please do check
        ofBaseVideo*    videoGrabber;
        
        static bool     instanceFlag;
        static Settings *single;
    };
}