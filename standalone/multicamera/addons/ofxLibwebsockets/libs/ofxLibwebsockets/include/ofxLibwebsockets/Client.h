//
//  Client.h
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/11/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include <libwebsockets.h>
#include "ofxLibwebsockets/Reactor.h"

namespace ofxLibwebsockets {

    struct ClientOptions {
        string  host;
        int     port;
        bool    bUseSSL;
        string  channel;
        string  protocol;
    };
    
    static ClientOptions defaultClientOptions(){
        ClientOptions opts;
        opts.host     = "localhost";
        opts.port     = 80;
        opts.bUseSSL  = false;
        opts.channel  = "/";
        opts.protocol = "NULL";
        return opts;
    };
    
    class Client : public Reactor {
        friend class Protocol;
    public:  
        
        Client();
        
        bool connect ( string _address = "localhost", bool bUseSSL=false );
        bool connect ( string _address, int _port, bool bUseSSL=false );
        bool connect ( ClientOptions options );
        
        void onClose( Event& args );
        void close();
        
        void send( string message );
        
        template<class T>
        void addListener(T * app){
            ofAddListener( clientProtocol.onconnectEvent, app, &T::onConnect); 
            ofAddListener( clientProtocol.onopenEvent, app, &T::onOpen);
            ofAddListener( clientProtocol.oncloseEvent, app, &T::onClose);
            ofAddListener( clientProtocol.onidleEvent, app, &T::onIdle);
            ofAddListener( clientProtocol.onmessageEvent, app, &T::onMessage);
            ofAddListener( clientProtocol.onbroadcastEvent, app, &T::onBroadcast);
        }
        
        Connection * getConnection(){
            return connection;
        }
        
    protected:
        ClientOptions defaultOptions;
        
    private:

        Connection * connection;
        
        struct libwebsocket *  lwsconnection;
        void threadedFunction();
        
        //wrap protocol
        Protocol clientProtocol;
        
    };
};