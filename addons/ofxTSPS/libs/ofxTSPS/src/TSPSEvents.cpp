/*
 *  Events.cpp
 *  openTSPS
 */

#include "ofxTSPS/TSPSEvents.h"

namespace ofxTSPS {
    CoreEvents & Events(){
        static CoreEvents * events = new CoreEvents;
        return *events;
    }
}