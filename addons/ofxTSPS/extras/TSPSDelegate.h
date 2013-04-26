//
//  TSPSDelegate.h
//  openTSPS
//
//  Created by BRenfer on 1/14/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

// TSPS core
#include "ofxTSPS.h"

#define TSPS_HOME_PAGE "http://opentsps.com"

namespace ofxTSPS {
    class Delegate
    {
    public:
        
        Delegate( int myId = 0 );
        
        void setup();
        void update();
        void draw();
        
        void setSettingsFile( string filename );
        string getSettingsFile();
        
        virtual void disableEvents();
        virtual void enableEvents();
        
        // accessors
        ofxTSPS::PeopleTracker * getPeopleTracker();
        
        // TSPS events for you to override
        virtual void onPersonEntered( ofxTSPS::Person * person, ofxTSPS::Scene * scene ){};
        virtual void onPersonUpdated( ofxTSPS::Person * person, ofxTSPS::Scene * scene ){};
        virtual void onPersonWillLeave( ofxTSPS::Person * person, ofxTSPS::Scene * scene ){};
        
        //status bar stuff
        ofImage statusBar;
        int		drawStatus[3];
        ofImage personEnteredImage;
        ofImage personUpdatedImage;
        ofImage personLeftImage;
        ofTrueTypeFont timesBoldItalic;
        
        //other gui images
        ofImage background;
        
        // TSPS core: PeopleTracker + events
        void onPersonEntered( const void * peopleTracker, ofxTSPS::EventArgs & tspsEvent );
        void onPersonUpdated( const void * peopleTracker, ofxTSPS::EventArgs & tspsEvent );
        void onPersonWillLeave( const void * peopleTracker, ofxTSPS::EventArgs & tspsEvent );
        
    protected:
        
        ofxTSPS::PeopleTracker peopleTracker;
        
        // settings file
        string settingsFile;
        
        int id;
        
    private:
        
    };
}

