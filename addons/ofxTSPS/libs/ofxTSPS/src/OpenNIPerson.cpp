/*
 *  OpenNIPerson.cpp
 *  openTSPS
 */

#include "ofxTSPS/person/OpenNIPerson.h"

namespace ofxTSPS {
    //--------------------------------------------------------------
    OpenNIPerson::OpenNIPerson( int pid, int oid ) : Person(pid, oid){
        //
    }
    
    //--------------------------------------------------------------
    void OpenNIPerson::draw( int cameraWidth, int cameraHeight, bool bSendContours, bool bSendHaar, float haarPadding ){
        ofPushStyle();    
        ofSetLineWidth(5);
        for (int i=0; i<skeleton.size(); i++){
            ofPoint start = ofVec2f(skeleton[i].getStartJoint().getProjectivePosition());
            ofPoint end = ofVec2f(skeleton[i].getEndJoint().getProjectivePosition());
            ofLine(start, end);
        }
        ofPopStyle();
        Person::draw( cameraWidth, cameraHeight, false, false, 0 );
    }
    
    //--------------------------------------------------------------
    void OpenNIPerson::update( ofxOpenNIUser & user ){
        updateCentroid(user.getCenter(), true);
        
        if ( user.getUseSkeleton() ){
            skeleton.clear();
            for ( int i=0; i<user.getNumLimbs(); i++){
                skeleton.push_back( user.getLimb((Limb) i) );
            }
        }
        
        Person::update();
    }
    
    
    //--------------------------------------------------------------
    string OpenNIPerson::getJSON( string type, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours, string append ){
        string skeletonString = ",\"skeleton\":";
        
        // for now, just packing these into an array
        skeletonString += "[";
        for (int i=0; i<skeleton.size(); i++){
            ofPoint start = skeleton[i].getStartJoint().getProjectivePosition();
            ofPoint end = skeleton[i].getEndJoint().getProjectivePosition();
            skeletonString += "{\"start\":{\"x\":\"" + ofToString(start.x / cameraWidth, 4) +"\",\"y\":\"" +ofToString(start.y/cameraHeight, 4) + "\"},\"end\":{\"x\":\"" + ofToString(end.x  / cameraWidth, 4) + "\", \"y\":\"" + ofToString(end.y  / cameraHeight, 4) + "\"}}";
            if ( i + 1 < skeleton.size()){
                skeletonString += ",";
            }
        }
        skeletonString += "]";
        
        return Person::getJSON(type, centroid, cameraWidth, cameraHeight, bSendContours, skeletonString );
    }
    
    //--------------------------------------------------------------
    vector<ofxOscMessage> OpenNIPerson::getOSCMessages( string type, bool bUseLegacy, int cameraWidth, int cameraHeight, bool sendContours ){
        vector<ofxOscMessage> messages = Person::getOSCMessages( type, bUseLegacy, cameraWidth, cameraHeight, sendContours );
        
        // assuming message ends with '/'
        ofxOscMessage m; 
        m.setAddress( type +"skeleton/");
        
        for (int i=0; i<skeleton.size(); i++){
            ofPoint start = skeleton[i].getStartJoint().getProjectivePosition();
            ofPoint end = skeleton[i].getEndJoint().getProjectivePosition();
            m.addFloatArg(start.x / cameraWidth);
            m.addFloatArg(start.y / cameraWidth);
            m.addFloatArg(end.x / cameraHeight);
            m.addFloatArg(end.y / cameraHeight);
        }
        
        messages.push_back(m);
        
        return messages;   
    }

}

