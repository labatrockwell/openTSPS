#include "ofxWebSocketEvents.h"

//--------------------------------------------------------------
ofxWebSocketEvent::ofxWebSocketEvent(ofxWebSocketConnection& _conn,
                                     std::string& _message)
: conn(_conn)
, message(_message)
{}

