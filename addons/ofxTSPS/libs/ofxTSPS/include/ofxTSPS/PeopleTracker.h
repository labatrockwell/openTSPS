/***************************************************************************
 *
 *  ofxTSPS::PeopleTracker.h
 *  Rockwell LAB + IDEO LAB peopleVision project
 *
 *  Based in part on code created by NUI Group Dev Team A on 3/1/09.
 *  Copyright 2009 NUI Group/Inc. All rights reserved.       
 *  Version : 1.20
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are 
 * met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer as
 * the first lines of this file unmodified.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NUI GROUP ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BEN WOODHOUSE BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 * Web: http://github.com/labatrockwell/openTSPS
 *
 * (...)
 *
 ***************************************************************************/

#ifndef _OFXPEOPLETRACKER_H
#define _OFXPEOPLETRACKER_H

// TSPS Core
#include "ofxTSPS/Constants.h"
#include "ofxTSPS/Events.h"
#include "ofxTSPS/Scene.h"
#include "ofxTSPS/Person.h"
#include "ofxTSPS/utils/View.h"
#include "ofxTSPS/utils/Utils.h"

// TSPS Processors
#include "ofxTSPS/Processor.h"
//#include "ofxTSPS/opencv/OpenCvProcessor.h" // R.I.P.
#include "ofxTSPS/cv/CvProcessor.h"

//processing
#include "CPUImageFilter.h"

// gui + settings
#include "ofxTSPS/gui/Settings.h"
#include "ofxTSPS/gui/GuiManager.h"

// communication
#include "ofxTSPS/communication/TUIOSender.h"
#include "ofxTSPS/communication/OscSender.h"
#include "ofxTSPS/communication/TCPSender.h"
#include "ofxTSPS/communication/WebSocketSender.h"

namespace ofxTSPS {

    // TSPS Manager class
    class PeopleTracker {
        public:
        
            PeopleTracker();
            ~PeopleTracker();
            
            //set up and update
        
            void setup(int w, int h, string settings="settings/settings.xml");				//Call during setup with camera width & height
        
            template <class Image>
            void update(Image & image){
                cameraImage = image;
                updateSettings();
                trackPeople();
            }
        
            void resize( int w, int h );            //If you need to resize the camera width / height
            void mousePressed(ofMouseEventArgs &e);	
        
            // processor
            void setProcessor ( Processor * _processor );
            Processor * getProcessor(){ return tspsProcessor; };
        
            // events called from processor
            void onPersonEntered( EventArgs & tspsEvent );
            void onPersonWillLeave( EventArgs & tspsEvent );
            void onPersonUpdated( EventArgs & tspsEvent );
            
            //communication
            
            void setupTuio(string ip, int port);
            void setupOsc(string ip, int port);
            void setupTcp(int port);
            void setupWebSocketServer(int port);
            void setupWebSocketClient( string host, int port, bool bUseSSL = false, string channel="");
        
            //tracking metrics
            
            void enableHaarFeatures(bool doHaar);
            void enableOpticalFlow(bool doOpticalFlow);
        
            // background learning and management
        
            void relearnBackground(); //one shot relearn trigger
            void enableBackgroundRelearnSmart(bool doSmartLearn);//auto-relearns if there are too many blobs in the scene
            void enableBackgroundReleaernProgressive(bool doProgressive); //relearns over time using progessive frame averagering
            void setRelearnRate(float relearnRate);
            
            //image control
        
            void setThreshold(float thresholdAmount);
            void setMinBlobSize(float minBlobSize); // % of screen
            void setMaxBlobSize(float maxBlobSize); // % of screen
        
            void enableSmooth(bool doSmooth);
            void setSmoothAmount(int smoothAmount);
        
            void enableHighpass(bool doHighpass);
            void setHighpassBlurAmount(int highpassBlurAmount);
            void setHighpassNoiseAmount(int highpassNoiseAmount);

            void enableAmplify(bool doAmp);
            void setAmplifyAmount(int amplifyAmount);

            //haar
            void setHaarXMLFile(string haarFile);
            void setHaarExpandArea(float haarExpandAmount); //makes the haar rect +area bigger
            void useHaarAsCentroid(bool useHaarCenter);
        
            //blobs
            void enableFindHoles(bool findHoles);
            void trackDarkBlobs();
            void trackLightBlobs();
            int  getDrawMode();
        
            //GUI extension
            void enableGuiEvents();
            void disableGuiEvents();
            void addSlider(string name, int* value, int min, int max);
            void addSlider(string name, float* value, float min, float max);
            void addToggle(string name, bool* value);
        
            // add buttons managed by the gui but external to the main interface
            guiTypeButton * addExternalButton( string name, ofRectangle dimensions );
        
            // video file stuff
            bool    useVideoFile();
            string  getVideoFile();
            void    setUseVideoFile( bool bUseVideoFile = true);
            void    setVideoFile( string file );
        
            //drawing methods
            void setDrawMode(int mode);
            void draw();
            void draw(int x, int y);
            void draw(int x, int y, int mode);
        
            //callbacks for blob tracking
        
            Person* personAtIndex(int i);
            int totalPeople();
            
            // layout functions
            void setActiveView( int activeIndex );
            void setActiveDimensions ( int actWidth, int actHeight);	
            bool loadFont(string fontName, int fontSize);
        
            //JG this is so we can access video grabber settings through the default interface
            void setVideoGrabber(ofBaseVideo* grabber, CameraType inputType);
            
            // for accessing the OSC sender whose parameters are adjusted in the GUI
            OscSender* getOSCsender(); 
        
            // Websockets 
            WebSocketSender * getWebSocketServer();
            
            // for accessing which view is the current view
            bool inCameraView();
            bool inAdjustedView();
            bool inBackgroundView();
            bool inDifferencingView();
            bool inDataView();
            
            // GUI getters + setters
            bool useKinect();	
            void setUseKinect( bool bUseKinect=true );
        
        protected:
            
            Processor * tspsProcessor;
        
            void trackPeople();
            void updateSettings();
            bool isTrackingPerson(int pid);
                
            vector<Person*> trackedPeople;
            Scene scene;
        
            // core images
            ofImage cameraImage, warpedImage, backgroundImage, differencedImage;
            
            // filter image
            CPUImageFilter		grayDiff;
        
            // blob tracking things
            int drawMode;
            float width, height;
            
            // haar
            string lastHaarFile;
            
            // switches for filters
            Settings *p_Settings;
        
            // communication
            
            TUIOSender tuioClient;
            bool bTuioEnabled;
            OscSender oscClient;
            bool bOscEnabled;
            TCPSender tcpClient;
            bool bTcpEnabled;
            WebSocketSender webSocketServer;
            bool bWebSocketServerEnabled, bWebSocketClientEnabled;
        
            //gui

            GuiManager gui;
            bool guiLoaded;
            float activeHeight, activeWidth;
        
            // layout functions
        
            ofTrueTypeFont font;
            bool bFontLoaded;
            
            //drawing + activating active video
        
            void updateViewRectangles();
            void drawBlobs( float width, float height );
            bool isInsideRect(float x, float y, ofRectangle rect);
            int activeViewIndex;
            
            ofRectangle activeView;
            ofxTSPS::View cameraView;
            ofxTSPS::View adjustedView;
            ofxTSPS::View bgView;
            ofxTSPS::View processedView;
            ofxTSPS::View dataView;
        
        private:
            bool hasMouseEvents;
    };
}
#endif
