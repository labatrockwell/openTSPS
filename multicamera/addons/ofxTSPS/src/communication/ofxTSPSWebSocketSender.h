//
//  ofxTSPSWebSocketSender.h
//  openTSPS
//
//  Created by brenfer on 10/4/11.
//  Copyright 2011 Rockwell Group. All rights reserved.
//

#include "ofxWebSocket.h"
#include "ofxTSPSPerson.h"

class ofxTSPSWebSocketMessage
{
    public:
        ofxTSPSWebSocketMessage( string message){ msg = message; };
        string msg;
};

class ofxTSPSWebSocketSender : public ofxWebSocketProtocol
{
    public:
        ofxTSPSWebSocketSender();
        
        bool setup( int port=7777 );  
        void close();
        void send();
              
        int getPort();
    
        void personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
    
    protected:
        vector<ofxTSPSWebSocketMessage> toSend;
        bool bSocketOpened, bSetup;
    
        int port;
    
        ofxWebSocketReactor * reactor;
        vector<ofxWebSocketConnection *> sockets;
        
        void onopen(ofxWebSocketEvent& args);
        void onclose(ofxWebSocketEvent& args);
        void onmessage(ofxWebSocketEvent& args);
};
