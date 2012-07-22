/*
 *  ofxTUIOSender.h
 *  peopleTracker_000
 *
 *  Created by Brett Renfer on 10/14/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *

 Based on code from memo (memo.tv)
 http://www.openframeworks.cc/forum/viewtopic.php?f=10&t=1192&p=10883&hilit=send+tuio#p10883

 *  11/23/2009 Modified by James George
 */

#pragma once

#include "TuioServer.h"
#include "TuioCursor.h"
#include "ofMain.h"

#ifndef OF_MAX_TOUCHES
#define OF_MAX_TOUCHES 20
#endif

//simple class for cursor management
class TuioPersonCursor {
public:
	int pid;
	float x,y;
	bool isAlive,wasAlive;
	bool moved;

	TuioPersonCursor(){
		isAlive = wasAlive = moved = false;
		x = y = 0;
	}
};

namespace ofxTSPS {
    
    using namespace TUIO;
    class TUIOSender {
    public:
        bool verbose;
        TuioServer      *tuioServer;
        
        TUIOSender() {
            tuioServer   = NULL;
            host      = "";
            port      = 0;
            verbose      = false;
        }
        
        ~TUIOSender() {
            delete tuioServer;
        };
        
        void setup(std::string host, int port);
        void update();
        
        void cursorPressed(float x, float y, int cursorId);
        void cursorReleased(float x, float y, int cursorId);
        void cursorDragged(float x, float y, int cursorId);
        
    protected:
        
        std::string		host;
        int				port;
        TuioCursor      *tuioCursor[OF_MAX_TOUCHES];
        TuioPersonCursor	myCursor[OF_MAX_TOUCHES];
        TuioTime			currentTime;
    };
}
