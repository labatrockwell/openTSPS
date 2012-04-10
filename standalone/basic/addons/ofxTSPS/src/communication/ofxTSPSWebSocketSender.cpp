//
//  ofxTSPSWebSocketSender.cpp
//  openTSPS
//
//  Created by brenfer on 10/4/11.
//  Copyright 2011 Rockwell Group. All rights reserved.
//

#include "ofxTSPSWebSocketSender.h"

//---------------------------------------------------------------------------
    ofxTSPSWebSocketSender::ofxTSPSWebSocketSender(){
        binary = false;
        port = 8887;
        bSetup = false;
        reactor = NULL;
    }

//---------------------------------------------------------------------------
    bool ofxTSPSWebSocketSender::setup(int _port){
        port = _port;
        sockets.clear();
        cout<<"setting up web socket server on port "<<port<<endl;
        
        // setup web socket server
        reactor = new ofxWebSocketReactor();
        //reactor = &ofxWebSocketReactor::instance();
        reactor->registerProtocol("tsps-protocol", *this);              
        bSetup = reactor->setup(port, "", "");
        
        return bSetup;
    }

//---------------------------------------------------------------------------
    int ofxTSPSWebSocketSender::getPort(){
        return port;
    };

//---------------------------------------------------------------------------
    void ofxTSPSWebSocketSender::close(){
        if (reactor != NULL) reactor->exit();
    }

//---------------------------------------------------------------------------
    void ofxTSPSWebSocketSender::send(){
        for (int i=0; i<toSend.size(); i++){
            for (int j=0; j<sockets.size(); j++){
               if (bSetup) sockets[j]->send(toSend[i].msg); 
            }
        }
        toSend.clear();
    };


//---------------------------------------------------------------------------
// TSPS EVENTS
//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
    toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("TSPS/personEntered", centroid,cameraWidth,cameraHeight,bSendContours )));
};

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("TSPS/personMoved",centroid,cameraWidth,cameraHeight,bSendContours )));
};

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){	
	toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("TSPS/personUpdated", centroid,cameraWidth,cameraHeight,bSendContours )));
};

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours )
{
	toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("TSPS/personWillLeave", centroid,cameraWidth,cameraHeight,bSendContours )));
};


//---------------------------------------------------------------------------
// WEBSOCKET EVENTS
//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onopen(ofxWebSocketEvent& args)
{
    sockets.push_back(&args.conn);
    bSocketOpened = true;
}

//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onmessage(ofxWebSocketEvent& args)
{
    // here you can send stuff back from the web if need be
    //args.message
    //cout<<"message? "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onclose(ofxWebSocketEvent& args)
{
    std::cout << "Connection closed" << std::endl;
    for (int i=0; i<sockets.size(); i++){
        if (sockets[i] == &args.conn){
			std::cout << "removing connection " << std::endl;
            sockets.erase( sockets.begin() + i);
            break;
        }
    }
}
