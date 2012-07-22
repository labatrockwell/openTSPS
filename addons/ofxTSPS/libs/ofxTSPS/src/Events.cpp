/*
 *  Events.cpp
 *  openTSPS
 */

#include "ofxTSPS/Events.h"

namespace ofxTSPS {
    CoreEvents & Events(){
        static CoreEvents * events = new CoreEvents;
        return *events;
    }
}