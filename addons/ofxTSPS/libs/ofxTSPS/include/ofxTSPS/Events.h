//
//  ofxTSPS::Events.h
//  openTSPS
//
//  Created by Brett Renfer on 7/15/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include "ofEvents.h"

namespace ofxTSPS {
    class Person;
    class Scene;
    
    class EventArgs {
    public:
        
		EventArgs(){};

        Person   * person;
        Scene    * scene;
    };
    
    class CoreEvents {
    public:
        
		CoreEvents(){};

        //called when the person enters the system
        ofEvent<EventArgs> personEntered;
        
        //called one frame before the person is removed from the list to let you clean up
        ofEvent<EventArgs>  personWillLeave;
        
        //called every frame no matter what.
        ofEvent<EventArgs>  personUpdated;
        
    };
    
    CoreEvents & Events();
}

template<class ListenerClass>
void ofxAddTSPSListeners( ListenerClass * listener ){
    ofAddListener(ofxTSPS::Events().personEntered, listener, &ListenerClass::onPersonEntered);
    ofAddListener(ofxTSPS::Events().personUpdated, listener, &ListenerClass::onPersonUpdated);
    ofAddListener(ofxTSPS::Events().personWillLeave, listener, &ListenerClass::onPersonWillLeave);
}

template<class ListenerClass>
void ofxRemoveTSPSListeners( ListenerClass * listener ){
    ofRemoveListener(ofxTSPS::Events().personEntered, listener, &ListenerClass::onPersonEntered);
    ofRemoveListener(ofxTSPS::Events().personUpdated, listener, &ListenerClass::onPersonUpdated);
    ofRemoveListener(ofxTSPS::Events().personWillLeave, listener, &ListenerClass::onPersonWillLeave);
}