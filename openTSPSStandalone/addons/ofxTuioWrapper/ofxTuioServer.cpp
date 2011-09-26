/*
 TUIO Server Wrapper for OpenFrameworks 
 Copyright (c) 2009 Matthias Dörfelt <info@mokafolio.de>
 Based on the TUIO SimpleSimulator by Martin Kaltenbrunner:
 
 Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ofxTuioServer.h"

ofxTuioServer::ofxTuioServer() {
	
}

//start Server (host, port);
void  ofxTuioServer::start(char * host, int port){
	verbose = false;
	if ((strcmp(host,"default")==0) && (port==0)) tuioServer = new TuioServer();
	else tuioServer = new TuioServer(host, port);
	currentTime = TuioTime::getSessionTime();	
}
//add cursor, object
TuioCursor * ofxTuioServer::addCursor(float _x, float _y){
	return tuioServer->addTuioCursor(_x/ofGetWidth(), _y/ofGetHeight());
	if(verbose) cout<<"ofxTuioServer: TUIO Cursor added at x: "+ofToString(_x)+" y: "+ofToString(_y)<<endl; 
}

TuioObject * ofxTuioServer::addObject(int _sid, float _x, float _y, float _a){
	return tuioServer->addTuioObject(_sid, _x/ofGetWidth(), _y/ofGetHeight(), _a);	
}

//update them
void ofxTuioServer::updateCursor(TuioCursor * _tcur, float _nx, float _ny){
	tuioServer->updateTuioCursor(_tcur, _nx/ofGetWidth(), _ny/ofGetHeight());
}

void ofxTuioServer::updateObject(TuioObject * _tobj, float _nx, float _ny, float _ang){
	tuioServer->updateTuioObject(_tobj, _nx/ofGetWidth(), _ny/ofGetHeight(), _ang);	
}

//remove them
void ofxTuioServer::removeCursor(TuioCursor * _tcur){
	tuioServer->removeTuioCursor(_tcur);
}

void ofxTuioServer::removeObject(TuioObject * _tobj){
	tuioServer->removeTuioObject(_tobj);
}

//Getters
TuioTime ofxTuioServer::getCurrentTime(){
	return tuioServer->getFrameTime();
}

void ofxTuioServer::setVerbose(bool _b){
	verbose = _b;
}

//send the OSC messages
void ofxTuioServer::run() {
	//this is weird, it should actually be something like....
	tuioServer->stopUntouchedMovingCursors();
	tuioServer->commitFrame();
	currentTime = TuioTime::getSessionTime();
	tuioServer->initFrame(currentTime);
	//...this:
	/*
	currentTime = TuioTime::getSessionTime();
	tuioServer->initFrame(currentTime);
	tuioServer->stopUntouchedMovingCursors();
	tuioServer->commitFrame();
	*/
	
	//propably due to the order of which OF handles mouse and keyboard events
}

//draw them for debug purposes
void ofxTuioServer::drawCursors() {
	char id[3];
	// draw the cursors
	std::list<TuioCursor*> cursorList = tuioServer->getTuioCursors();
	for (std::list<TuioCursor*>::iterator tuioCursor = cursorList.begin(); tuioCursor!=cursorList.end(); tuioCursor++) {
		TuioCursor * tcur = (*tuioCursor); 
		std::list<TuioPoint> path = tcur->getPath();
		if (path.size()>0) {
			
			TuioPoint last_point = path.front();
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 1.0);
			
			for (std::list<TuioPoint>::iterator point = path.begin(); point!=path.end(); point++) {
				glVertex3f(last_point.getX()*ofGetWidth(), last_point.getY()*ofGetHeight(), 0.0f);
				glVertex3f(point->getX()*ofGetWidth(), point->getY()*ofGetHeight(), 0.0f);
				last_point.update(point->getX(),point->getY());
			}
			glEnd();
			
			// draw the finger tip
			glColor3f(0.0, 0.75, 0.75);
			ofCircle(tcur->getX()*ofGetWidth(), tcur->getY()*ofGetHeight(), 10);
		}
	}
}

void ofxTuioServer::drawObjects(){
    std::list<TuioObject*> objectList = tuioServer->getTuioObjects();
	list<TuioObject*>::iterator tobj;

	for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
		TuioObject *obj = (*tobj);
		glColor3f(1.0,0.0,0.0);
		glPushMatrix();
		glTranslatef(obj->getX()*ofGetWidth(), obj->getY()*ofGetHeight(), 0.0);
		glRotatef(obj->getAngleDegrees(), 0.0, 0.0, 1.0);
		ofRect(-10.0, -10.0, 20.0, 20.0);
		glColor3f(1.0,1.0,1.0);
		ofLine(0, 0, 0, -10);
		glPopMatrix();
		string str = "SymbolId: "+ofToString((int)(obj->getSymbolID()));
		ofDrawBitmapString(str, obj->getX()*ofGetWidth()-10.0, obj->getY()*ofGetHeight()+25.0);
		str = "SessionId: "+ofToString((int)(obj->getSessionID()));
		ofDrawBitmapString(str, obj->getX()*ofGetWidth()-10.0, obj->getY()*ofGetHeight()+40.0);
	}

}