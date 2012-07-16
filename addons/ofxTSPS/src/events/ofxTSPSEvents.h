//
//  ofxTSPSEvents.h
//  openTSPS
//
//  Created by Brett Renfer on 7/15/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include "ofEvents.h"

class ofxTSPSPerson;
class ofxTSPSScene;

class ofxTSPSEventArgs {
public:
    
    ofxTSPSPerson   * person;
    ofxTSPSScene    * scene;
};

class ofxTSPSCoreEvents {
public:
    
	//called when the person enters the system
    ofEvent<ofxTSPSEventArgs> personEntered;
    
	//called one frame before the person is removed from the list to let you clean up
    ofEvent<ofxTSPSEventArgs>  personWillLeave;
    
	//called every frame no matter what.
	ofEvent<ofxTSPSEventArgs>  personUpdated;
    
};

ofxTSPSCoreEvents & ofxTSPSEvents();

template<class ListenerClass>
void ofxAddTSPSListeners( ListenerClass * listener ){
    ofAddListener(ofxTSPSEvents().personEntered, listener, &ListenerClass::onPersonEntered);
    ofAddListener(ofxTSPSEvents().personUpdated, listener, &ListenerClass::onPersonUpdated);
    ofAddListener(ofxTSPSEvents().personWillLeave, listener, &ListenerClass::onPersonWillLeave);
}

template<class ListenerClass>
void ofxRemoveTSPSListeners( ListenerClass * listener ){
    ofRemoveListener(ofxTSPSEvents().personEntered, listener, &ListenerClass::onPersonEntered);
    ofRemoveListener(ofxTSPSEvents().personUpdated, listener, &ListenerClass::onPersonUpdated);
    ofRemoveListener(ofxTSPSEvents().personWillLeave, listener, &ListenerClass::onPersonWillLeave);
}