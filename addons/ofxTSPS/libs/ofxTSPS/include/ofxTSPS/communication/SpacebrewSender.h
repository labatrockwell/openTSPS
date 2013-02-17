//
//  Spacebrew.h
//  openTSPS
//
//  Created by BRenfer on 12/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofxSpacebrew.h"

namespace Spacebrew {
    class TSPSMessage : public Message {
    public:
        TSPSMessage( string _name="", string _type="", string _val="") :
        Message( _name, _type, _val)
        {}
        
        string getJSON( string configName ){
            // don't quote escape value
            return "{\"message\":{\"clientName\":\"" + configName +"\",\"name\":\"" + name + "\",\"type\":\"" + type + "\",\"value\":" + value + "}}";
        }
    };
}

namespace ofxTSPS {
    class Person;
    class Scene;
    
    class SpacebrewSender
    {
    public:
        
        SpacebrewSender( string host="localhost" );
        
        void setHost( string host );
        void enable();
        void disable();
        bool isEnabled();
        
        void update();
        void personEntered ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        void personUpdated ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        void personWillLeave ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        
        // custom data: comes across as /TSPS/CustomEvent/
        void customEvent( string eventName, string eventData );
        void customEvent( string eventName, vector<string>params );
        void customEvent( string eventName, map<string,string>params );
        
        // scene data
        void sceneUpdated( Scene & s );
        
    private:
        bool bEnabled;
        Spacebrew::Connection * connection;
        void setupConnection();
        
        bool bSeesPeople;
        
        Spacebrew::TSPSMessage personEnteredMessage, personUpdatedMessage, personWillLeaveMessage, sceneMessage;
        
        ofMutex spacebrewMutex;
    };
};
