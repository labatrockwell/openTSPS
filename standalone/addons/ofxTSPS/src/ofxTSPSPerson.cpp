/*
 *  ofxTSPSPerson.cpp
 *  peopleTracker_000
 *
 *  Adapted by James George on 11/21/09.
 *  Copyright 2009 (http://www.jamesgeorge.org) All rights reserved.
 *
 */
#include "ofxTSPSPerson.h"

#define MAX_HAAR_GHOSTFRAMES 15 //how long before we say it's really gone

ofxTSPSPerson::ofxTSPSPerson(int pid, int oid, ofxCvBlob blob)
: pid(pid),
  oid(oid),
  age(0),
  hasHaar(false),
  haarRect(ofRectangle(0,0,0,0)),
  opticalFlowVectorAccumulation(ofPoint(0,0)),
  centroid(blob.centroid),
  framesWithoutHaar(0),
  customAttributes(NULL),
  depth(0)
{
	update(blob, false);
}

ofxTSPSPerson::~ofxTSPSPerson()
{
	if(customAttributes != NULL){
		free(customAttributes);
	}
}

void ofxTSPSPerson::update(ofxCvBlob blob, bool dampen)
{
	if(dampen){
		centroid = (centroid * .7) + (blob.centroid * .3);
	}
	else{
		centroid = blob.centroid;
	}
	
	velocity	 = blob.centroid - centroid;
	area		 = blob.area;
	boundingRect = blob.boundingRect;
	contour		 = blob.pts; 
	age++;
}


void ofxTSPSPerson::setHaarRect(ofRectangle _haarRect)
{
	haarRect = _haarRect;
	hasHaar = true;
	framesWithoutHaar = 0;
}

ofRectangle ofxTSPSPerson::getHaarRect()
{
	if(!hasHaar){
		printf("ofxTSPSPerson: accessing Haar rect without Haar!\n");
		return ofRectangle(0,0,0,0);
	}
	return haarRect;
}

bool ofxTSPSPerson::hasHaarRect()
{
	return hasHaar;
}

void ofxTSPSPerson::noHaarThisFrame()
{
	//temp remove keep haar
	hasHaar = false;
	return;
	
	//if we had a haar but it vanished move it by the movement of the blob
	if(framesWithoutHaar++ > MAX_HAAR_GHOSTFRAMES){
		hasHaar = false;
	}
	if(hasHaar){
		haarRect.x += velocity.x;
		haarRect.y += velocity.y;
	}
}

ofRectangle ofxTSPSPerson::getBoundingRectNormalized(float videoWidth, float videoHeight)
{
		return ofRectangle(boundingRect.x/videoWidth,
						   boundingRect.y/videoHeight,
						   boundingRect.width/videoWidth,
						   boundingRect.height/videoHeight);
}


ofRectangle ofxTSPSPerson::getHaarRectNormalized(float videoWidth, float videoHeight){
	return ofRectangle(haarRect.x/videoWidth,
					   haarRect.y/videoHeight,
					   haarRect.width/videoWidth,
					   haarRect.height/videoHeight);
};

ofPoint ofxTSPSPerson::getCentroidNormalized(float videoWidth, float videoHeight)
{
	return ofPoint(centroid.x / videoWidth, centroid.y / videoHeight);
}

ofPoint ofxTSPSPerson::getHaarCentroidNormalized(float videoWidth, float videoHeight)
{
	ofRectangle haarRect = getHaarRect();
	return ofPoint( (haarRect.x + haarRect.width/2) / videoWidth, (haarRect.y + haarRect.height/2) / videoHeight );
}

#pragma mark get as string

/***************************************************************
 GET PERSON STRING (so we don't have to write this so many times)
 ***************************************************************/

string ofxTSPSPerson::getJSON( string type, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	
	//construct a JSON object
	
	stringstream message;
	message<<"{";
    message<<"\"type\":\""<<type<<"\",";
    message<<"\"id\":"<<pid<<",";
	message<<"\"age\":"<<age<<",";
	message<<"\"depth\":"<<depth<<",";
	message<<"\"centroid\":{"<<"\"x\":"<<centroid.x<<",\"y\":"<<centroid.y<<"},"; //pass in normalized centroid
	message<<"\"velocity\":{"<<"\"x\":"<<ofToString(velocity.x, 3)<<",\"y\":"<<ofToString(velocity.y, 3)<<"},";
	
	ofRectangle scaledRect = getBoundingRectNormalized(cameraWidth,cameraHeight);
	message<<"\"boundingrect\":{"<<"\"x\":"<<scaledRect.x<<",\"y\":"<<scaledRect.y<<",\"width\":"<<scaledRect.width<<",\"height\":"<<scaledRect.height<<"},";
	
	message<<"\"opticalflow\":{"<<"\"x\":"<<opticalFlowVectorAccumulation.x<<",\"y\":"<<opticalFlowVectorAccumulation.y<<"},";
	ofRectangle scaledHaar = getHaarRectNormalized(cameraWidth,cameraHeight);
	message<<"\"haarrect\":{"<<"\"x\":"<<scaledHaar.x<<",\"y\":"<<scaledHaar.y<<",\"width\":"<<scaledHaar.width<<",\"height\":"<<scaledHaar.height<<"},";	
	
	if (bSendContours){
		message<<"\"contours\":[";
		for (int i=0; i<simpleContour.size(); i++){
			message<<"{\"x\":"<<ofToString(simpleContour[i].x, 3)<<",\"y\":"<<ofToString(simpleContour[i].y, 3)<<"}";
			if (i+1 < simpleContour.size()) message<<",";
		};
		message<<"]";
	}	
	message<<"}";
	return message.str();
}

