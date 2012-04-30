//
//  Events.cpp
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/11/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#include "ofxLibwebsockets/Events.h"

namespace ofxLibwebsockets {
        
    //--------------------------------------------------------------
    Event::Event(Connection& _conn, std::string& _message)
    : conn(_conn)
    , message(_message)
    {}

    //_Events ofxWebSocketEvents;
}