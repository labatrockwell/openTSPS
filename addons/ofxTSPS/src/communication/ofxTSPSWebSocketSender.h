//
//  ofxTSPSWebSocketSender.h
//  openTSPS
//
//  Created by brenfer on 10/4/11.
//  Copyright 2011 Rockwell Group. All rights reserved.
//

#include "ofxLibwebsockets.h"
#include "ofxTSPSPerson.h"

class ofxTSPSWebSocketMessage {
public:
    ofxTSPSWebSocketMessage( string message){ msg = message; };
    string msg;
};

class ofxTSPSWebSocketSender : public ofxLibwebsockets::Protocol {
    friend class ofxLibwebsockets::Client;
    friend class ofxLibwebsockets::Server;
public:
    ofxTSPSWebSocketSender();
    ~ofxTSPSWebSocketSender();
    
    bool setupClient( string host = "localhost", int port=7777, bool bUseSSL = false, string channel = "" );  
    bool setupServer( int port=7777 );  
    void closeClient();
    void closeServer();
    void send();
    
    // append data to message
    void setAppendData( string append );
    
    string  getHost();
    int     getPort();
    int     getServerPort();
    
    void personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
    void personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
    void personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
    void personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
    
protected:
    vector<ofxTSPSWebSocketMessage> toSend;
    bool bSocketOpened, bSetup;
    
    // data to append to the websocket if you choose
    string appendData;
    
    string host;
    int serverPort, port;
    
    ofxLibwebsockets::Server * server;
    ofxLibwebsockets::Client * client;
    vector<ofxLibwebsockets::Connection *> sockets;
    
    void onConnect(ofxLibwebsockets::Event & args);
    void onOpen(ofxLibwebsockets::Event& args);
    void onClose(ofxLibwebsockets::Event& args);
    void onMessage(ofxLibwebsockets::Event& args);
    void onBroadcast(ofxLibwebsockets::Event& args);
    void onIdle(ofxLibwebsockets::Event& args){};
};
