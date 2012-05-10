//
//  Util.h
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/11/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include <libwebsockets.h>
#include "ofxLibwebsockets/Connection.h"
#include "ofxLibwebsockets/Reactor.h"
#include "ofxLibwebsockets/Client.h"
#include "ofxLibwebsockets/Server.h"

namespace ofxLibwebsockets {
        
    class Client;
    class Server;
    
    static string getCallbackReason( int reason );
    static void dump_handshake_info(struct lws_tokens *lwst);

    // CLIENT CALLBACK

    static int lws_client_callback(struct libwebsocket_context* context, struct libwebsocket *ws, enum libwebsocket_callback_reasons reason, void *user, void *data, size_t len){
        const struct libwebsocket_protocols* lws_protocol = (ws == NULL ? NULL : libwebsockets_get_protocol(ws));
        int idx = lws_protocol? lws_protocol->protocol_index : 0;            
        
        Connection* conn;
        
        Connection** conn_ptr = (Connection**)user;
        Reactor* reactor = NULL;
        Protocol* protocol;
            
        for (int i=0; i<reactors.size(); i++){
            if (reactors[i]->getContext() == context){
                reactor =  reactors[i];
                protocol = reactor->protocol(idx);
                conn = ((Client*) reactor)->getConnection();
                break;
            } else {
            }
        }
        
        ofLog( OF_LOG_VERBOSE, getCallbackReason(reason) );
        
        if (reason == LWS_CALLBACK_CLIENT_ESTABLISHED){
        } else if (reason == LWS_CALLBACK_CLOSED){
        }
        
        switch (reason)
        {            
            case LWS_CALLBACK_CONFIRM_EXTENSION_OKAY:
                return 0;
                
            case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
                if (protocol != NULL){
                    return reactor->_allow(protocol, (int)(long)user)? 0 : 1;
                } else {
                    return 0;
                }
            case LWS_CALLBACK_HTTP:
                if ( reactor != NULL){
                    return reactor->_http(ws, (char*)data);
                } else {
                    return 0;
                }
            case LWS_CALLBACK_CLIENT_ESTABLISHED:
            case LWS_CALLBACK_CLIENT_WRITEABLE:
            case LWS_CALLBACK_CLOSED:
            case LWS_CALLBACK_CLIENT_RECEIVE:
                if (reactor != NULL){
                    //conn = *(Connection**)user;
                    if (conn && conn->ws != ws) conn->ws = ws;
                    return reactor->_notify(conn, reason, (char*)data, len);
                } else {
                    return 0;
                }
            default:
                return 0;
        }

        return 1; // FAIL (e.g. unhandled case/break in switch)
    }
    
    // SERVER CALLBACK

    static int lws_callback(struct libwebsocket_context* context, struct libwebsocket *ws, enum libwebsocket_callback_reasons reason, void *user, void *data, size_t len){
        const struct libwebsocket_protocols* lws_protocol = (ws == NULL ? NULL : libwebsockets_get_protocol(ws));
        int idx = lws_protocol? lws_protocol->protocol_index : 0;   
        
        // valid connection w/o a protocol
        if ( ws != NULL && lws_protocol == NULL ){
            // OK for now, returning 0 above
        }
        
        //bool bAllowAllProtocls = (ws != NULL ? lws_protocol == NULL : false);
        
        Connection* conn;    
        Connection** conn_ptr = (Connection**)user;
        Server* reactor = NULL;
        Protocol* protocol = NULL;
        
        for (int i=0; i<reactors.size(); i++){
            if (reactors[i]->getContext() == context){
                reactor = (Server*) reactors[i];
                protocol = reactor->protocol( (idx > 0 ? idx : 0) );
                break;
            } else {
            }
        }
        
        ofLog( OF_LOG_VERBOSE, getCallbackReason(reason) );
        
        if (reason == LWS_CALLBACK_ESTABLISHED){
            if ( reactor != NULL ){
                *conn_ptr = new Connection(reactor, protocol);
            }
        } else if (reason == LWS_CALLBACK_CLOSED){
            //if (*conn_ptr != NULL)
            //delete *conn_ptr;
        }
        
        switch (reason)
        {
            case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
                return 0;
                
            case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
                if (protocol != NULL ){
                    return reactor->_allow(protocol, (int)(long)user)? 0 : 1;
                } else {
                    return 1;
                }
                
            case LWS_CALLBACK_HTTP:
                return reactor->_http(ws, (char*)data);
                
            case LWS_CALLBACK_ESTABLISHED:
            case LWS_CALLBACK_CLOSED:
            case LWS_CALLBACK_SERVER_WRITEABLE:
            case LWS_CALLBACK_RECEIVE:
            case LWS_CALLBACK_BROADCAST:
                conn = *(Connection**)user;
                
                if (conn && conn->ws != ws){
                    conn->ws = ws;
                    conn->setupAddress();
                }
                if (reactor){
                    return reactor->_notify(conn, reason, (char*)data, len);                
                } else {
                    return 0;
                }
                
            default:
                return 0;
        }
        
        return 1; // FAIL (e.g. unhandled case/break in switch)
    }

    static void dump_handshake_info(struct lws_tokens *lwst)
    {
        int n;
        static const char *token_names[WSI_TOKEN_COUNT] = {
            "GET URI",
            "Host",
            "Connection",
            "key 1",
            "key 2",
            "Protocol",
            "Upgrade",
            "Origin",
            "Draft",
            "Challenge",
            
            /* new for 04 */
            "Key",
            "Version",
            "Sworigin",
            
            /* new for 05 */
            "Extensions",
            
            /* client receives these */
            "Accept",
            "Nonce",
            "Http",
            "MuxURL",
        };
        
        for (n = 0; n < WSI_TOKEN_COUNT; n++) {
            if (lwst[n].token == NULL)
                continue;
            
            fprintf(stderr, "    %s = %s\n", token_names[n], lwst[n].token);
        }
    }
    
    static string getCallbackReason( int reason ){
        switch (reason){
            case 0 : return "LWS_CALLBACK_ESTABLISHED";
            case 1 : return "LWS_CALLBACK_CLIENT_ESTABLISHED";
            case 2 : return "LWS_CALLBACK_CLOSED";
            case 3 : return "LWS_CALLBACK_RECEIVE";
            case 4 : return "LWS_CALLBACK_CLIENT_RECEIVE";
            case 5 : return "LWS_CALLBACK_CLIENT_RECEIVE_PONG";
            case 6 : return "LWS_CALLBACK_CLIENT_WRITEABLE";
            case 7 : return "LWS_CALLBACK_SERVER_WRITEABLE";
            case 8 : return "LWS_CALLBACK_HTTP";
            case 9 : return "LWS_CALLBACK_BROADCAST";
            case 10 : return "LWS_CALLBACK_FILTER_NETWORK_CONNECTION";
            case 11 : return "LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION";
            case 12 : return "LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS";
            case 13 : return "LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS";
            case 14 : return "LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION";
            case 15 : return "LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER";
            case 16 : return "LWS_CALLBACK_CONFIRM_EXTENSION_OKAY";
            case 17 : return "LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED";
            /* external poll() management support */
            case 18 : return "LWS_CALLBACK_ADD_POLL_FD";
            case 19 : return "LWS_CALLBACK_DEL_POLL_FD";
            case 20 : return "LWS_CALLBACK_SET_MODE_POLL_FD";
            case 21 : return "LWS_CALLBACK_CLEAR_MODE_POLL_FD";
        };
    }
}