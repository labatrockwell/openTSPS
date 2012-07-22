//
//  ofxTSPSopenNIPerson.h
//  openTSPS
//
//  Created by Brett Renfer on 7/17/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#pragma once

#include "ofxTSPS/Person.h"
#include "ofxOpenNI.h"

namespace ofxTSPS {
    class OpenNIPerson : public Person {
    public:
        OpenNIPerson( int pid, int oid );
        vector<ofxOpenNILimb> skeleton;
        
        void draw( int cameraWidth, int cameraHeight, bool bSendContours=false, bool bSendHaar=false, float haarPadding=0 );
        void update( ofxOpenNIUser & user );
        string getJSON( string type, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours=false, string append="" );
        vector<ofxOscMessage> getOSCMessages( string type, bool bUseLegacy, int cameraWidth, int cameraHeight, bool sendContours );
    };
}