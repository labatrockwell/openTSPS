//
//  ofxTSPSofxOpenCvProcessor.cpp
//  openTSPS
//
//  Created by Brett Renfer on 7/12/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#include <iostream>
#include "ofxTSPSofxOpenCvProcessor.h"

void ofxTSPSofxOpenCvProcessor::setup( ofxTSPSSettings * settings, vector<ofxTSPSPerson*> * peopleVector ){
    // capabilities
    bCanTrackHaar       = true;
    bCanTrackContours   = true;
    bCanTrackSkeleton   = false;
    bCanTrackOpticalFlow = true;
    
    bTrackHaar = false;
    bTrackContours = true;
    bTrackSkeleton = false;
    bTrackOpticalFlow = false;
}

//--------------------------------------------------------------
void ofxTSPSofxOpenCvProcessor::captureBackground(){
    
}

//--------------------------------------------------------------
void ofxTSPSofxOpenCvProcessor::update( ofBaseHasPixels & image = NULL ){
    
}

//--------------------------------------------------------------
void ofxTSPSofxOpenCvProcessor::process(){
    
}

//--------------------------------------------------------------
bool ofxTSPSofxOpenCvProcessor::setTrackHaar ( bool trackHaar ){
    
}

//--------------------------------------------------------------
bool ofxTSPSofxOpenCvProcessor::setTrackContours ( bool trackContours ){
    
}

//--------------------------------------------------------------
bool ofxTSPSofxOpenCvProcessor::setTrackOpticalFlow ( bool trackOpticalFlow ){
    
}

//---------------------------------------------------------------------------
void ofxTSPSofxOpenCvProcessor::blobOn( int x, int y, int id, int order )
{
	ofxCvTrackedBlob blob = persistentTracker.getById( id );
	ofxTSPSPerson* newPerson = new ofxTSPSPerson(id, order, blob);
	if(eventListener != NULL){
		eventListener->personEntered(newPerson, &scene);
	}
    
    ofPoint centroid = newPerson->getCentroidNormalized(width, height);
    
	if(bTuioEnabled){
		tuioClient.cursorPressed(1.0*x/width, 1.0*y/height, order);
	}
	if(bOscEnabled){
		oscClient.personEntered(newPerson, centroid, width, height, p_Settings->bSendOscContours);
	}
	if(bTcpEnabled){
		tcpClient.personEntered(newPerson, centroid, width, height, p_Settings->bSendOscContours);
	}
	if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
		webSocketServer.personEntered(newPerson, centroid, width, height, p_Settings->bSendOscContours);
	}
	trackedPeople.push_back( newPerson );	
}

//---------------------------------------------------------------------------
void ofxTSPSofxOpenCvProcessor::blobMoved( int x, int y, int id, int order ){/*not used*/}

//---------------------------------------------------------------------------
void ofxTSPSofxOpenCvProcessor::blobOff( int x, int y, int id, int order )
{
	ofxTSPSPerson* p = getTrackedPerson(id);
	//ensure we are tracking
	if(NULL == p){
		ofLog(OF_LOG_WARNING, "ofxPerson::warning. encountered persistent blob " + ofToString( id ) +" without a person behind them\n");		
		return;
	}
    
	//delete the object and remove it from the vector
	std::vector<ofxTSPSPerson*>::iterator it;
	for(it = trackedPeople.begin(); it != trackedPeople.end(); it++){
		if((*it)->pid == p->pid){
			trackedPeople.erase(it);
			break;
		}
	}
	
	//alert the delegate
	if(eventListener != NULL){
		eventListener->personWillLeave(p, &scene);
	}
    
    ofPoint centroid = p->getCentroidNormalized(width, height);
	if (bTuioEnabled) {
		tuioClient.cursorReleased(centroid.x, centroid.y, order);	
	}
	//send osc kill message if enabled
	if (bOscEnabled){
		oscClient.personWillLeave(p, centroid, width, height, p_Settings->bSendOscContours);
	};
	
	//send tcp kill message if enabled
	if(bTcpEnabled){
		tcpClient.personWillLeave(p, centroid, width, height, p_Settings->bSendOscContours);
	}
    
	if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
		webSocketServer.personWillLeave(p, centroid, width, height, p_Settings->bSendOscContours);
	}
    
    // delete pointer
    delete p;
}
