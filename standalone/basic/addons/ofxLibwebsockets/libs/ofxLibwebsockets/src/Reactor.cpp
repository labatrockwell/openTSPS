//
//  Reactor.cpp
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/12/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#include "ofxLibwebsockets/Reactor.h"

namespace ofxLibwebsockets { 

    //--------------------------------------------------------------
    Reactor::Reactor()
    : context(NULL), waitMillis(50){
        reactors.push_back(this);
    }

    //--------------------------------------------------------------
    Reactor::~Reactor(){
        exit();
    }

    //--------------------------------------------------------------
    void Reactor::registerProtocol(const std::string& name, Protocol& protocol){
        protocol.idx = protocols.size();
        protocol.reactor = this;
        protocols.push_back(make_pair(name, &protocol));
    }

    //--------------------------------------------------------------
    Protocol* const Reactor::protocol(const unsigned int idx){
        return (idx < protocols.size())? protocols[idx].second : NULL;
    }

    //--------------------------------------------------------------
    void Reactor::close(Connection* const conn){
        if (conn != NULL && conn->ws != NULL){
            libwebsocket_close_and_free_session(context, conn->ws, LWS_CLOSE_STATUS_NORMAL);
        }
    }

    //--------------------------------------------------------------
    void Reactor::exit(){
        if (context != NULL)
        {
            stopThread();
            libwebsocket_context_destroy(context);
            context = NULL;
        }
    }


    //--------------------------------------------------------------
    struct libwebsocket_context * Reactor::getContext(){
        return context;
    }
    
    //--------------------------------------------------------------
    vector<Connection *> Reactor::getConnections(){
        return connections;
    }
    
    //--------------------------------------------------------------
    Connection * Reactor::getConnection( int index ){
        if ( index < connections.size() ){
            return connections[ index ];
        }
        return NULL;
    }

    //--------------------------------------------------------------
    unsigned int
    Reactor::_allow(Protocol* const protocol, const long fd){
        std::string client_ip(128, 0);
        std::string client_name(128, 0);
        
        libwebsockets_get_peer_addresses((int)fd,
                                         &client_name[0], client_name.size(),
                                         &client_ip[0], client_ip.size());
        return protocol->_allowClient(client_name, client_ip);
    }

    //--------------------------------------------------------------
    unsigned int Reactor::_notify(Connection* conn,
                                enum libwebsocket_callback_reasons const reason,
                                const char* const _message,
                                const unsigned int len){
        if (conn == NULL || conn->protocol == NULL){
            if (conn == NULL){
                cout<<"connection is null"<<endl;
            } else {
                cout<<"protocol is null"<<endl;            
            }
            return 1;
        }
        
        std::string message;
        
        if (_message != NULL && len > 0){
            message = std::string(_message, len);
        }
        Event args(*conn, message);
        
        if (_message != NULL && len > 0){
            args.json    = NULL;
            
            args.message = args.conn.recv(args.message);
            
            bool parsingSuccessful = reader.parse( args.message, args.json );
            if ( !parsingSuccessful ){
                // report to the user the failure and their locations in the document.
                ofLog( OF_LOG_WARNING, "Failed to parse JSON\n"+ reader.getFormatedErrorMessages() );
                args.json = NULL;
            }
        }
        
        if (reason==LWS_CALLBACK_ESTABLISHED || reason == LWS_CALLBACK_CLIENT_ESTABLISHED){
            connections.push_back( conn );
            ofNotifyEvent(conn->protocol->onopenEvent, args);
        } else if (reason==LWS_CALLBACK_CLOSED){
            
            // erase connection from vector
            for (int i=0; i<connections.size(); i++){
                if ( connections[i] == conn ){
                    connections.erase( connections.begin() + i );
                    break;
                }
            }
            
            ofNotifyEvent(conn->protocol->oncloseEvent, args);
        } else if (reason==LWS_CALLBACK_SERVER_WRITEABLE){
            ofNotifyEvent(conn->protocol->onidleEvent, args);
        } else if (reason==LWS_CALLBACK_BROADCAST){
            ofNotifyEvent(conn->protocol->onbroadcastEvent, args);
        } else if (reason==LWS_CALLBACK_RECEIVE || reason == LWS_CALLBACK_CLIENT_RECEIVE){
            ofNotifyEvent(conn->protocol->onmessageEvent, args);
        }
        
        return 0;
    }

    //--------------------------------------------------------------
    unsigned int Reactor::_http(struct libwebsocket *ws,
                              const char* const _url){
        std::string url(_url);
        if (url == "/")
            url = "/index.html";
        
        // why does this need to be done?
        std::string ext = url.substr(url.find_last_of(".")+1);
        std::string file = document_root+url;
        std::string mimetype = "text/html";
        
        if (ext == "ico")
            mimetype = "image/x-icon";
        if (ext == "manifest")
            mimetype = "text/cache-manifest";
        if (ext == "swf")
            mimetype = "application/x-shockwave-flash";
        if (ext == "js")
            mimetype = "application/javascript";
        if (ext == "css")
            mimetype = "text/css";
        
        if (libwebsockets_serve_http_file(ws, file.c_str(), mimetype.c_str())){
            ofLog( OF_LOG_WARNING, "Failed to send HTTP file "+ file + " for "+ url);
        }
			return 0;
    }
}