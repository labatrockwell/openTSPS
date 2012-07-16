//
//  ofxTSPSEvents.cpp
//  openTSPS
//
//  Created by Brett Renfer on 7/15/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#include <iostream>

#include "ofxTSPSEvents.h"

ofxTSPSCoreEvents & ofxTSPSEvents(){
    static ofxTSPSCoreEvents * events = new ofxTSPSCoreEvents;
    return *events;
}