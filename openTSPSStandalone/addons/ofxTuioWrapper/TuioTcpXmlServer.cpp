/*
 *  TuioTcpXmlServer.cpp
 *  openFrameworks
 *
 *  Created by Dave Vondle on 10/8/09.
 *  Copyright 2009 All rights reserved.
 */

#include "TuioTcpXmlServer.h"
#include "TuioTcpXmlServer.h"

TuioTcpXmlServer::TuioTcpXmlServer() {
	
}

void TuioTcpXmlServer::initialize() {
	m_tcpServer.setup(3000);
	//currentFrameTime = TuioTime::getSessionTime().getSeconds();
}

void TuioTcpXmlServer::run() {
	
}

void TuioTcpXmlServer::updateCursor(TuioCursor *tcur, float x, float y){
	if (tcur==NULL) return;
	if (tcur->getTuioTime()==currentFrameTime) return;
	tcur->update(currentFrameTime,(x/ofGetWidth()), (y/ofGetHeight()));
	
	if (tcur->isMoving()){	
		
		m_tcpServer.sendToAll("<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\""+ofToString(3333)+"\" TIME=\""+ofToString(ofGetElapsedTimef())+"\">" +
						  "<MESSAGE NAME=\"/tuio/2Dcur\"><ARGUMENT TYPE=\"s\" VALUE=\"set\"/><ARGUMENT TYPE=\"i\" VALUE=\""+ofToString(tcur->getCursorID())+"\"/>"+
						  "<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(tcur->getX())+"\"/>"+
						  "<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(tcur->getY())+"\"/>"+
						  "<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(tcur->getXSpeed())+"\"/>"+
						  "<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(tcur->getYSpeed())+"\"/>"+
						  "<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(tcur->getMotionAccel())+"\"/>"+
						  "</MESSAGE>"+
						  "</OSCPACKET>");
	}
}
