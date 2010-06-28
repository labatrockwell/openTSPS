/***********************************************************************
 
 ofxTSPSTCPSender.cpp
 TSPSPeopleVision

 Copyright (c) 2009, LAB at Rockwell Group
 http://lab.rockwellgroup.com
 Created by Brett Renfer on 2/9/10.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ***********************************************************************/

#include "ofxTSPSTCPSender.h"


/***************************************************************
	SETUP + CONNECT
***************************************************************/	

void ofxTSPSTCPSender::setup( int _port ){
	port = _port;
	if (TCP.getNumClients() > 0) TCP.disconnectClient(0);
	bool bSetup = TCP.setup( port );
	oldport = port;
};

/***************************************************************
	UPDATE
***************************************************************/	

ofxTSPSTCPSender::~ofxTSPSTCPSender(){
	TCP.close();
};

void ofxTSPSTCPSender::update(){ 
	if (oldport != port){
		oldport = port;
		setup(port);				
	}
}
/***************************************************************
 SEND
 ***************************************************************/	

void ofxTSPSTCPSender::send(){
	for(int i = 0; i < TCP.getNumClients(); i++){
		TCP.send(i,currentString);
	}
	currentString = "";
};

/***************************************************************
 SEND
 ***************************************************************/


void ofxTSPSTCPSender::personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	stringstream message;
	message<<"TSPS/personEntered/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
};

void ofxTSPSTCPSender::personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	stringstream message;
	message<<"TSPS/personMoved/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
};

void ofxTSPSTCPSender::personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	stringstream message;
	message<<"TSPS/personUpdated/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
};

void ofxTSPSTCPSender::personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours )
{
	stringstream message;
	message<<"TSPS/personWillLeave/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
};

/***************************************************************
 GET PERSON STRING (so we don't have to write this so many times)
 ***************************************************************/

string ofxTSPSTCPSender::getPersonString( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	
	//construct a message with each value set separated by ;
	// within each set, items are separated by ,
	// and objects are separated by :
	// e.g. velcoity is:
	// "velocity:velocity.x:velocity.y"
	// while contours are "contours:contour0.x:contour0.y,contour1.x:contour1.y..."
	
	stringstream message;
	message<<"id/"<<p->pid<<";";
	message<<"age/"<<p->age<<";";
	message<<"centroid/"<<"x>"<<p->centroid.x<<":"<<"y>"<<p->centroid.y<<";";
	message<<"velocity/"<<"x>"<<p->velocity.x<<":"<<"y>"<<p->velocity.y<<";";
	
	ofRectangle boundingRect = p->getBoundingRectNormalized(cameraWidth,cameraHeight);
	message<<"boundingrect/"<<"x>"<<boundingRect.x<<":"<<"y>"<<boundingRect.y<<":"<<"width>"<<boundingRect.width<<":"<<"height>"<<boundingRect.height<<";";
	
	message<<"opticalflow/"<<"x>"<<p->opticalFlowVectorAccumulation.x<<":"<<"y>"<<p->opticalFlowVectorAccumulation.y<<";";
	
	if (bSendContours){
		message<<"contours/";
		for (int i=0; i<p->simpleContour.size(); i++){
			message<<"x>"<<p->simpleContour[i].x<<":"<<"y>"<<p->simpleContour[i].y<<",";
		};
		message<<";";
	}	
	return message.str();
}