#pragma once 

#include <string>

class ofxWebSocketConnection;

class ofxWebSocketEvent {
public:
  ofxWebSocketEvent(ofxWebSocketConnection& _conn,
                    std::string& _message);

  ofxWebSocketConnection& conn;
  std::string& message;
};

