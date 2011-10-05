//
//  ofxTSPSWebSocketSender.h
//  openTSPS
//
//  Created by brenfer on 10/4/11.
//  Copyright 2011 Rockwell Group. All rights reserved.
//

#include "ofxWebSocket.h"
#include "ofxTSPSPerson.h"

class ofxTSPSWebSocketSender : public ofxWebSocketProtocol
{
    public:
        ofxTSPSWebSocketSender();
        
        void setup( int port );  
        void send();
        
        string currentString;
        
        void personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
        void personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
    
    protected:
        bool bSocketOpened;
    
        ofxWebSocketReactor * reactor;
        vector<ofxWebSocketConnection *> sockets;
    
    
        string getPersonString( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours );
    
        void onopen(ofxWebSocketEvent& args);
        void onclose(ofxWebSocketEvent& args);
        void onmessage(ofxWebSocketEvent& args);
};
