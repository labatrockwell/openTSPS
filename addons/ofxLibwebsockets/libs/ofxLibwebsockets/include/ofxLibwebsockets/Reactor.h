//
//  Reactor.h
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/11/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include "ofThread.h"
#include "ofEvents.h"
#include <libwebsockets.h>
#include "ofxLibwebsockets/Protocol.h"
#include "ofxLibwebsockets/Connection.h"

namespace ofxLibwebsockets {
        
    class Reactor : public ofThread {
        friend class Protocol;
        
    public:
        Reactor();
        ~Reactor();
        void exit();
        
        // methods
        void close(Connection* const conn);
        void registerProtocol(const std::string& name, Protocol& protocol);
        
        // getters
        struct libwebsocket_context *   getContext();
        vector<Connection *>            getConnections();
        Connection *                    getConnection( int index );
        
        Protocol* const protocol(const unsigned int idx);
        std::vector<std::pair<std::string, Protocol*> > protocols;
        
        //private:
        unsigned int _allow(Protocol* const protocol, const long fd);
        
        unsigned int _notify(Connection* conn, enum libwebsocket_callback_reasons const reason,
                             const char* const _message, const unsigned int len);
        
        unsigned int _http(struct libwebsocket *ws, const char* const url);
        
    protected:
        std::string     document_root;
        unsigned int    waitMillis;
        std::string     interface;
        
        virtual void threadedFunction(){};  
        
        string address;
        string channel;
        int port;
        
        struct libwebsocket_context *context;
        
        std::vector<struct libwebsocket_protocols> lws_protocols;
        
        // maybe this should be a map?
        vector<Connection *> connections;
        
    private:
        Json::Reader reader;
        
    };
    
    static vector<Reactor *> reactors;
};