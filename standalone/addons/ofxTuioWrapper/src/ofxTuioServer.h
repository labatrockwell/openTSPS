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

#ifndef _TUIO_SERVER
#define _TUIO_SERVER

#include "ofMain.h"
#include "TuioServer.h"
#include "TuioCursor.h"

#include <list>
#include <math.h>

using namespace TUIO;

class ofxTuioServer { 
	
public:
	ofxTuioServer();
	~ofxTuioServer() {
		delete tuioServer;
	};
	
	void start(char * _host, int _port);
	TuioCursor * addCursor(float _x, float _y);
	TuioObject * addObject(int _sid, float _x, float _y, float _a);
	
	void updateCursor(TuioCursor * _tcur, float _nx, float _ny);
	void updateObject(TuioObject * _tobj, float _nx, float _ny, float _ang);
	
	void removeCursor(TuioCursor * _tcur);
	void removeObject(TuioObject * _tobj);
	
	void drawCursors();
	void drawObjects();
	
	void setVerbose(bool _b);
	TuioTime getCurrentTime();

	void run();
	
	//This is public so you can also access all the functionality of the official TUIO c++ client that does not get wrapped!!
	//I only wrapped the functions where screencoordinates need to be put in a range from 0 to 1 ect.
	TuioServer * tuioServer;
	
private:
	bool verbose;
	TuioTime currentTime;
};

#endif /* INCLUDED_ofxTuioServer_H */
