/*
 *  ofxTSPS::TCPSender.h
 *  TSPSPeopleVision
 *
 *  Created by Brett Renfer on 2/8/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *	
 *	lots of help from http://www.openframeworks.cc/forum/viewtopic.php?p=17764&#p17764
 */

#pragma once

#include "ofxNetwork.h"
#include "ofxXmlSettings.h"

namespace ofxTSPS {
    class Person;
    class TCPSender {
    public:
        
        ofxTCPServer TCP;
        int port, oldport;
        bool bSendContours;
        ~TCPSender();
        void setup( int port );	
        void update();
        void send();
        
        string currentString;
        
        void personEntered ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personMoved ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personUpdated ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personWillLeave ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        
        // custom data
        void customEvent( string eventName, string eventData );
        void customEvent( string eventName, vector<string>params );
        void customEvent( string eventName, map<string,string>params );
        
        string getPersonString( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours );
    };
};