/*
 *  ofxTSPSTCPSender.h
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
#include "ofxTSPSPerson.h"

class ofxTSPSTCPSender 
{
public:
	
	ofxTCPServer TCP;
	int port, oldport;
	bool bSendContours;
	~ofxTSPSTCPSender();
	void setup( int port );	
	void update();
	void send();
	
	string currentString;
	
	void personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
	void personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
	void personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
	void personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
	
	string getPersonString( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours );
};