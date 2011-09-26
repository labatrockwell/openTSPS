/*
 *  TuioTcpXmlServer.h
 *  openFrameworks
 *
 *  Created by Dave Vondle on 10/8/09.
 *  Copyright 2009  All rights reserved.
 *
 */
#ifndef _TUIO_TCP_XMLSERVER
#define _TUIO_TCP_XMLSERVER

#include "ofMain.h"
#include "TuioCursor.h"
#include "ofxNetwork.h"

using namespace TUIO;

class TuioTcpXmlServer { 
	
public:
	//TCP Network 
	ofxTCPServer 	m_tcpServer;
	
	TuioTcpXmlServer();
	void initialize();
	void run();
	void updateCursor(TuioCursor *tcur, float x, float y);
	
private:
	TuioTime currentFrameTime;
};

#endif