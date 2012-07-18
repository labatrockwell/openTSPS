//
//  ofxTSPSopenNIPerson.cpp
//  openTSPS
//
//  Created by Brett Renfer on 7/17/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#include "ofxTSPSopenNIPerson.h"

ofxTSPSopenNIPerson::ofxTSPSopenNIPerson( int pid, int oid ) : ofxTSPSPerson(pid, oid){
    //
}

void ofxTSPSopenNIPerson::draw( int cameraWidth, int cameraHeight, bool bSendContours, bool bSendHaar, float haarPadding ){
    ofPushStyle();    
    ofSetLineWidth(5);
    for (int i=0; i<skeleton.size(); i++){
        ofPoint start = ofVec2f(skeleton[i].getStartJoint().getProjectivePosition());
        ofPoint end = ofVec2f(skeleton[i].getEndJoint().getProjectivePosition());
        ofLine(start, end);
    }
    ofPopStyle();
    ofxTSPSPerson::draw( cameraWidth, cameraHeight, false, false, 0 );
}

void ofxTSPSopenNIPerson::update( ofxOpenNIUser & user ){
    updateCentroid(user.getCenter(), true);
    
    if ( user.getUseSkeleton() ){
        skeleton.clear();
        for ( int i=0; i<user.getNumLimbs(); i++){
            skeleton.push_back( user.getLimb((Limb) i) );
        }
    }
    
    ofxTSPSPerson::update();
}


string ofxTSPSopenNIPerson::getJSON( string type, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours, string append ){
    string skeletonString = ",\"skeleton\":";
    
    // for now, just packing these into an array
    skeletonString += "[";
    for (int i=0; i<skeleton.size(); i++){
        ofPoint start = skeleton[i].getStartJoint().getProjectivePosition();
        ofPoint end = skeleton[i].getEndJoint().getProjectivePosition();
        skeletonString += "{\"start\":{\"x\":\"" + ofToString(start.x) +"\",\"y\":\"" +ofToString(start.y) + "\"},\"end\":{\"x\":\"" + ofToString(end.x) + "\", \"y\":\"" + ofToString(end.x) + "\"}}";
        if ( i + 1 < skeleton.size()){
            skeletonString += ",";
        }
    }
    skeletonString += "]";
    
    return ofxTSPSPerson::getJSON(type, centroid, cameraWidth, cameraHeight, false, skeletonString );
}

