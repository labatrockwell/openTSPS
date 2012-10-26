/*
 *  ofxTSPS::Scene.cpp
 *  TSPSPeopleVision
 *
 */

#include "ofxTSPS/Scene.h"
#include "ofxOscMessage.h"
#include "ofUtils.h"

namespace ofxTSPS {
    Scene::Scene(){
        
    }
    
    
    ofxOscMessage Scene::getOscMessage( string address ){
        ofxOscMessage m;
        m.setAddress(address);
        m.addIntArg(ofGetElapsedTimeMillis());
        m.addFloatArg(percentCovered);
        m.addFloatArg(averageMotion.x);
        m.addFloatArg(averageMotion.y);
        return m;
    }
}