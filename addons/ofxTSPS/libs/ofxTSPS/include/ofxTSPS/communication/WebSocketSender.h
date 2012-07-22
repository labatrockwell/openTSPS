//
//  ofxTSPS::WebSocketSender.h
//  openTSPS
//
//  Created by brenfer on 10/4/11.
//  Copyright 2011 Rockwell Group. All rights reserved.
//

#include "ofxLibwebsockets.h"

namespace ofxTSPS {
    class Person;
    
    class WebSocketMessage {
    public:
        WebSocketMessage( string message){ msg = message; };
        string msg;
    };
    
    class WebSocketSender : public ofxLibwebsockets::Protocol {
        friend class ofxLibwebsockets::Client;
        friend class ofxLibwebsockets::Server;
    public:
        WebSocketSender();
        ~WebSocketSender();
        
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
        
        void personEntered ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personMoved ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personUpdated ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personWillLeave ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
        
    protected:
        vector<WebSocketMessage> toSend;
        bool bClientSetup, bServerSetup;
        
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

};