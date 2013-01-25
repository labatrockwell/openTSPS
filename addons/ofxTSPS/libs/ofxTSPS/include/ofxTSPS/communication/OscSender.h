/***********************************************************************

 ofxTSPS::OSCSender.h
 TSPSPeopleVision

 Copyright (c) 2009, LAB at Rockwell Group
 http://lab.rockwellgroup.com
 Created by Brett Renfer on 1/14/10.

 OSC Message is structured like this:

 address: /TSPS/person/ + ordered id (i.e. TSPS/person/0)

 // LEGACY
 
 argument 0: pid;
 argument 1: oid;
 argument 2: age;
 argument 3: centroid.x;
 argument 4: centroid.y;
 argument 5: velocity.x;
 argument 6: velocity.y;
 argument 7: boundingRect.x;
 argument 9: boundingRect.y;
 argument 9: boundingRect.width;
 argument 10: boundingRect.height;
 argument 11: opticalFlowVectorAccumulation.x;
 argument 12: opticalFlowVectorAccumulation.y;
 argument 13+ : contours (if enabled)

 // NEW
 
 argument 0: pid;
 argument 1: oid;
 argument 2: age;
 argument 3: centroid.x;
 argument 4: centroid.y;
 argument 5: velocity.x;
 argument 6: velocity.y;
 argument 7: depth;
 argument 8: boundingRect.x;
 argument 9: boundingRect.y;
 argument 10: boundingRect.width;
 argument 11: boundingRect.height;
 argument 12: highest.x
 argument 13: highest.y
 argument 14: haarRect.x;           - will be 0 if hasHaar == false
 argument 15: haarRect.y;           - will be 0 if hasHaar == false
 argument 16: haarRect.width;       - will be 0 if hasHaar == false
 argument 17: haarRect.height;      - will be 0 if hasHaar == false
 argument 18: opticalFlowVectorAccumulation.x;
 argument 19: opticalFlowVectorAccumulation.y;
 argument 20+ : contours (if enabled)
 
 // TSPS 1.3.2: Scene + CustomEvent
 
 address: /TSPS/scene
 argument 0: currentTime
 argument 1: percentCovered
 argument 2: numPeople
 argument 3: averageMotion.x
 argument 4: averageMotion.y
 
 address: /TSPS/customEvent
 argument 0: {string} eventName
 argument 1: {string} eventData 
 
 ***********************************************************************/

#pragma once

#include "ofxOsc.h"

namespace ofxTSPS {
    class Person;
    class Scene;
    class OscSender : public ofxOscSender
    {
        public :
        
        //"old" variables are to check for reroute
        string	ip, oldip;
        int		port, oldport;
        bool useLegacy;
        
        OscSender();
        OscSender(string _ip, int _port);
        void setupSender(string _ip, int _port);
        
        void update();
        void personEntered ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        void personMoved ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        void personUpdated ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        void personWillLeave ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        
        // custom data: comes across as /TSPS/CustomEvent/
        void customEvent( string eventName, string eventData );
        void customEvent( string eventName, vector<string>params );
        void customEvent( string eventName, map<string,string>params );
        
        // scene data
        void sceneUpdated( Scene & s );
        
        void send ( ofxOscMessage m );
        void reroute(string _ip, int _port);
    };
};
