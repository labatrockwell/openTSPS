//
//  Protcol.cpp
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/11/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#include "ofxLibwebsockets/Protocol.h"
#include "ofxLibwebsockets/Reactor.h"

namespace ofxLibwebsockets {

    //--------------------------------------------------------------
    Protocol::Protocol()
    : defaultAllowPolicy(true){
        ofAddListener(onconnectEvent,      this, &Protocol::_onconnect);
        ofAddListener(onopenEvent,         this, &Protocol::_onopen);
        ofAddListener(oncloseEvent,        this, &Protocol::_onclose);
        ofAddListener(onidleEvent,         this, &Protocol::_onidle);
        ofAddListener(onmessageEvent,      this, &Protocol::_onmessage);
        ofAddListener(onbroadcastEvent,    this, &Protocol::_onbroadcast);
        binary = false;
    }

    //--------------------------------------------------------------
    Protocol::~Protocol() {
        ofRemoveListener(onconnectEvent,   this, &Protocol::_onconnect);
        ofRemoveListener(onopenEvent,      this, &Protocol::_onopen);
        ofRemoveListener(oncloseEvent,     this, &Protocol::_onclose);
        ofRemoveListener(onidleEvent,      this, &Protocol::_onidle);
        ofRemoveListener(onmessageEvent,   this, &Protocol::_onmessage);
        ofRemoveListener(onbroadcastEvent, this, &Protocol::_onbroadcast);
        binary = false;
    }

    //--------------------------------------------------------------
    bool Protocol::_allowClient(const std::string name, const std::string ip) const {
        std::map<std::string, bool>::const_iterator allow_iter;
        
        allow_iter = allowRules.find(name);
        if (allow_iter != allowRules.end())
            return allow_iter->second;
        
        allow_iter = allowRules.find(ip);
        if (allow_iter != allowRules.end())
            return allow_iter->second;
        
        return allowClient(name, ip);
    }

    //--------------------------------------------------------------
    bool Protocol::allowClient(const std::string name, const std::string ip) const {
        return defaultAllowPolicy;
    }

    //--------------------------------------------------------------
    void Protocol::_onconnect(Event& args){ onconnect(args); }  

    void Protocol::onconnect(Event& args){}

    //--------------------------------------------------------------
    void Protocol::_onopen(Event& args){ onopen(args); }

    void Protocol::onopen(Event&args){}

    //--------------------------------------------------------------
    void Protocol::_onclose(Event& args){ onclose(args); }

    void Protocol::onclose(Event&args){}

    //--------------------------------------------------------------
    void Protocol::_onidle(Event& args){ onidle(args); }

    void Protocol::onidle(Event&args){}

    //--------------------------------------------------------------
    void Protocol::_onmessage(Event& args){
        onmessage(args);
    }

    void Protocol::onmessage(Event&args){}

    //--------------------------------------------------------------
    void Protocol::_onbroadcast(Event& args){ onbroadcast(args); }

    void Protocol::onbroadcast(Event&args){ args.conn.send(args.message); }

    //--------------------------------------------------------------
    void Protocol::broadcast(const std::string& message){
        std::string buf(LWS_SEND_BUFFER_PRE_PADDING+1024+LWS_SEND_BUFFER_POST_PADDING, 0);
        unsigned char *p = (unsigned char*)&buf[LWS_SEND_BUFFER_PRE_PADDING];
        
        if (reactor != NULL)
        {
            memcpy(p, message.c_str(), message.size());
            int n = libwebsockets_broadcast(&reactor->lws_protocols[idx], p, message.size());
            if (n < 0)
                fprintf(stderr, "ERROR writing to socket");
        }
    }
}
