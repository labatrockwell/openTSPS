//
//  Protocol.h
//  ofxLibwebsockets
//
//  Created by Brett Renfer on 4/11/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include <string>
#include <map>

#include "ofMain.h"
#include "ofxLibwebsockets/Events.h"

namespace ofxLibwebsockets {
    
    class Reactor;

    #include "json.h"

    class Protocol
    {
        friend class Reactor;
        friend class Server;
        friend class Client;
        
    public:
        Protocol();
        ~Protocol();
        
        virtual bool allowClient(const std::string name,
                                 const std::string ip) const;
        
        void broadcast(const std::string& message);
        
        unsigned int idx;
        bool binary;
        
    protected:  
        virtual void execute() {}
        
        virtual void onconnect  (Event& args);
        virtual void onopen     (Event& args);
        virtual void onclose    (Event& args);
        virtual void onidle     (Event& args);
        virtual void onmessage  (Event& args);
        virtual void onbroadcast(Event& args);
        
        ofEvent<Event> onconnectEvent;
        ofEvent<Event> onopenEvent;
        ofEvent<Event> oncloseEvent;
        ofEvent<Event> onidleEvent;
        ofEvent<Event> onmessageEvent;
        ofEvent<Event> onbroadcastEvent;
        
        bool defaultAllowPolicy;
        std::map<std::string, bool> allowRules;
        
        Reactor* reactor;
        
    private:
        void _onconnect   (Event& args);
        void _onopen      (Event& args);
        void _onclose     (Event& args);
        void _onidle      (Event& args);
        void _onmessage   (Event& args);
        void _onbroadcast (Event& args);
        
        bool _allowClient(const std::string name,
                          const std::string ip) const;
    };
};