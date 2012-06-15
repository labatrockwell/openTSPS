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
    bServerSetup = bClientSetup = false;
    server = NULL;
    client = NULL;
}

//---------------------------------------------------------------------------
ofxTSPSWebSocketSender::~ofxTSPSWebSocketSender(){
    sockets.clear();
    if ( client != NULL ) client->exit();
    if ( server != NULL ) server->exit();
    bServerSetup = bClientSetup = false;
}

//---------------------------------------------------------------------------
bool ofxTSPSWebSocketSender::setupClient( string _host, int _port, bool bUseSSL, string channel ){
    port = _port;
    host = _host;
    //sockets.clear();
    cout<<"setting up web socket client on port "<<port<<endl;
    
    // setup web socket server
    client = new ofxLibwebsockets::Client();
    client->addListener( this );
    ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
    options.host = _host;
    options.port = _port;
    options.bUseSSL = bUseSSL;
    options.channel = channel;
    
    bClientSetup = client->connect( options );
    return bClientSetup;        
}

//---------------------------------------------------------------------------
bool ofxTSPSWebSocketSender::setupServer( int _port ){
    serverPort = _port;
    //sockets.clear();
    cout<<"setting up web socket server on port "<<port<<endl;
    
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port     = _port;
    options.protocol = "tsps-protocol";
    
    // setup web socket server
    server = new ofxLibwebsockets::Server();
    server->addListener( this ); 
    bServerSetup = server->setup(options);
    return bServerSetup;        
}


//---------------------------------------------------------------------------
string ofxTSPSWebSocketSender::getHost(){
    return host;
}

//---------------------------------------------------------------------------
int ofxTSPSWebSocketSender::getPort(){
    return port;
}

//---------------------------------------------------------------------------
int ofxTSPSWebSocketSender::getServerPort(){
    return serverPort;
}

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::closeClient(){
    if ( client != NULL ){
        client->close();
        client = NULL;
    }
    bClientSetup = false;
}

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::closeServer(){
    if (server != NULL) server->exit();
    bServerSetup = false;
}

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::send(){
    for (int i=0; i<toSend.size(); i++){
        for (int j=0; j<sockets.size(); j++){
            if ( bServerSetup && server != NULL )sockets[j]->send(toSend[i].msg); 
        }
        if ( bClientSetup && client != NULL ){
            client->send( toSend[i].msg );
        }
    }
    
    toSend.clear();
};

//---------------------------------------------------------------------------
// ADVANCED: APPEND DATA TO WS JSON
//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::setAppendData( string data ){
    appendData = data;
    if ( appendData[0] != ',' ){
        ofLog( OF_LOG_WARNING, "You did not add a comma to your appended data. Adding for you, may be invalid JSON!" );
        appendData = "," + appendData;
    }
}

//---------------------------------------------------------------------------
// TSPS EVENTS
//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personEntered ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
    toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("personEntered", centroid,cameraWidth,cameraHeight,bSendContours, appendData )));
}

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personMoved ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("personMoved",centroid,cameraWidth,cameraHeight,bSendContours, appendData )));
}

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personUpdated ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){	
	toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("personUpdated", centroid,cameraWidth,cameraHeight,bSendContours, appendData )));
}

//---------------------------------------------------------------------------
void ofxTSPSWebSocketSender::personWillLeave ( ofxTSPSPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
	toSend.push_back(ofxTSPSWebSocketMessage(p->getJSON("personWillLeave", centroid,cameraWidth,cameraHeight,bSendContours, appendData )));
}


//---------------------------------------------------------------------------
// WEBSOCKET EVENTS
//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onConnect(ofxLibwebsockets::Event& args){
    sockets.push_back(&args.conn);
}

void ofxTSPSWebSocketSender::onOpen(ofxLibwebsockets::Event& args) {
    sockets.push_back(&args.conn);
}

//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onMessage(ofxLibwebsockets::Event& args) {
    // here you can send stuff back from the web if need be
    //args.message
    //cout<<"message? "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onBroadcast(ofxLibwebsockets::Event& args) {
    // here you can send stuff back from the web if need be
    //args.message
    //cout<<"message? "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofxTSPSWebSocketSender::onClose(ofxLibwebsockets::Event& args) {
    std::cout << "Connection closed" << std::endl;
    for (int i=0; i<sockets.size(); i++){
        if (sockets[i] == &args.conn){
			std::cout << "removing connection " << std::endl;
            sockets.erase( sockets.begin() + i);
            break;
        }
    }
}
