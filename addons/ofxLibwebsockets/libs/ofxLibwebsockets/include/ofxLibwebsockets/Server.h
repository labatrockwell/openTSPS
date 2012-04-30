//
//  Server.h
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/11/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//
//  SETUP PROCESS:
//  1) initialize (automatic if not a pointer)
//  2) (optional) add protocols (e.g. server.addProtocol( "hey", myProtocol ); )
//      - see Reactor.h
//  3) (optional) add testApp as a listener (defaults to "" protocol, can be set in options)
//  4) call setup( port ) or setup ( ServerOptions options )

#pragma once

#include "ofThread.h"
#include "ofEvents.h"

#include <libwebsockets.h>

#include "ofxLibwebsockets/Reactor.h"

namespace ofxLibwebsockets {

    class Connection;
    class Protocol;
    
    struct ServerOptions {
        int     port;
        bool    bUseSSL;
        string  protocol;
        string  sslCertPath;
        string  sslKeyPath;
        
        string  documentRoot;
    };
    
    static ServerOptions defaultServerOptions(){
        ServerOptions opts;
        opts.port     = 80;
        opts.protocol = "NULL";
        opts.bUseSSL  = false;
        opts.sslCertPath = ofToDataPath("ssl/libwebsockets-test-server.pem", true);
        opts.sslKeyPath = ofToDataPath("ssl/libwebsockets-test-server.key.pem", true);
        opts.documentRoot = ofToDataPath("web", true);
        return opts;
    }

    class Server : public Reactor {
        friend class Protocol;
        
    public:
        Server();
        
        bool setup( int _port = 80, bool bUseSSL = false );
        bool setup( ServerOptions options );
        
        // broadcast a message to all connections
        void broadcast( string message );
        
        // send to all connections
        void send( string message );
        
        // send anything that has pixels
        template <class T> 
        void sendBinary( T& image ){
            lock();
            for (int i=0; i<connections.size(); i++){
                if ( connections[i] != NULL ){
                    connections[i]->sendBinary( image );
                }
            }
            unlock();
        }
        
        void sendBinary( unsigned char * data, int size );
        
        // send to a specific connection
        void send( string message, string ip );
        
        template<class T>
        void addListener(T * app){
            ofAddListener( serverProtocol.onconnectEvent, app, &T::onConnect); 
            ofAddListener( serverProtocol.onopenEvent, app, &T::onOpen);
            ofAddListener( serverProtocol.oncloseEvent, app, &T::onClose);
            ofAddListener( serverProtocol.onidleEvent, app, &T::onIdle);
            ofAddListener( serverProtocol.onmessageEvent, app, &T::onMessage);
            ofAddListener( serverProtocol.onbroadcastEvent, app, &T::onBroadcast);
        }
        
        //getters
        int     getPort();
        string  getProtocol();
        bool    usingSSL();
        
    protected:
        std::string interface;
        ServerOptions defaultOptions;
        
    private:
        Protocol serverProtocol;
        void threadedFunction();  
    };
};