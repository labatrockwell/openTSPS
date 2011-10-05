//
//  ofxTSPSWebSocketSender.cpp
//  openTSPS
//
//  Created by brenfer on 10/4/11.
//  Copyright 2011 Rockwell Group. All rights reserved.
//

#include "ofxTSPSWebSocketSender.h"

/*********************************************************
    CONSTRUCTOR
*********************************************************/

    ofxTSPSWebSocketSender::ofxTSPSWebSocketSender(){
    }

/*********************************************************
    SETUP
*********************************************************/

    void ofxTSPSWebSocketSender::setup(int port){
        sockets.clear();
        cout<<"setting up web socket server on port "<<port<<endl;
        
        // setup web socket server
        reactor = &ofxWebSocketReactor::instance();
        reactor->registerProtocol("tsps-protocol", *this);              
        reactor->setup(port, "", "");
    }

/***************************************************************
 SEND
 ***************************************************************/	

    void ofxTSPSWebSocketSender::send(){
        if (currentString != ""){
            for (int i=0; i<sockets.size(); i++){
               sockets[i]->send(currentString); 
            }
        }
        currentString = "";
    };

/***************************************************************
 SEND
 ***************************************************************/


void ofxTSPSWebSocketSender::personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	stringstream message;
	message<<"TSPS/personEntered/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
    send();
};

void ofxTSPSWebSocketSender::personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	stringstream message;
	message<<"TSPS/personMoved/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
    send();
};

void ofxTSPSWebSocketSender::personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	stringstream message;
	message<<"TSPS/personUpdated/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
    send();
};

void ofxTSPSWebSocketSender::personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours )
{
	stringstream message;
	message<<"TSPS/personWillLeave/"<<";";
	message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
	message<<"\\";
	
	currentString += message.str();
    send();
};

/***************************************************************
 GET PERSON STRING (so we don't have to write this so many times)
 ***************************************************************/

string ofxTSPSWebSocketSender::getPersonString( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	
	//construct a message with each value set separated by ;
	// within each set, items are separated by ,
	// and objects are separated by :
	// e.g. velcoity is:
	// "velocity:velocity.x:velocity.y"
	// while contours are "contours:contour0.x:contour0.y,contour1.x:contour1.y..."
	
	stringstream message;
	message<<"id/"<<p->pid<<";";
	message<<"age/"<<p->age<<";";
	message<<"centroid/"<<"x>"<<centroid.x<<":"<<"y>"<<centroid.y<<";"; //pass in normalized centroid
	message<<"velocity/"<<"x>"<<p->velocity.x<<":"<<"y>"<<p->velocity.y<<";";
	
	ofRectangle boundingRect = p->getBoundingRectNormalized(cameraWidth,cameraHeight);
	message<<"boundingrect/"<<"x>"<<boundingRect.x<<":"<<"y>"<<boundingRect.y<<":"<<"width>"<<boundingRect.width<<":"<<"height>"<<boundingRect.height<<";";
	
	message<<"opticalflow/"<<"x>"<<p->opticalFlowVectorAccumulation.x<<":"<<"y>"<<p->opticalFlowVectorAccumulation.y<<";";
	ofRectangle haarRect = p->getHaarRectNormalized(cameraWidth,cameraHeight);
	message<<"haarrect/"<<"x>"<<haarRect.x<<":"<<"y>"<<haarRect.y<<":"<<"width>"<<haarRect.width<<":"<<"height>"<<haarRect.height<<";";	
	
	if (bSendContours){
		message<<"contours/";
		for (int i=0; i<p->simpleContour.size(); i++){
			message<<"x>"<<p->simpleContour[i].x<<":"<<"y>"<<p->simpleContour[i].y;
			if (i+1 < p->simpleContour.size()) message<<",";
		};
		message<<";";
	}	
	return message.str();
}

/*********************************************************
    WEBSOCKET EVENTS
*********************************************************/
//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onopen(ofxWebSocketEvent& args)
{
    cout<<"added socket"<<endl;
    sockets.push_back(&args.conn);
    bSocketOpened = true;
}

//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onmessage(ofxWebSocketEvent& args)
{
    //args.message
    //cout<<"message? "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onclose(ofxWebSocketEvent& args)
{
    //std::cout << "Connection closed" << std::endl;
    for (int i=0; i<sockets.size(); i++){
        if (sockets[i] == &args.conn){
            sockets.erase( sockets.begin() + i);
            break;
        }
    }
}
