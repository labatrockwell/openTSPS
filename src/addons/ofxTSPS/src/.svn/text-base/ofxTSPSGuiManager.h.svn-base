/*
 *  ofxTSPSGuiManager.h
 *  TSPSPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *

 Portions of this software are distributed under the GPL and the MIT license.

 The MIT License

 Copyright (c) 2010 Rockwell Group LLC

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 */

//#include "ofxSimpleGuiToo.h"
#include "ofxLabGui.h"

// maybe there's a simpler way to integrate this?
#include "ofxCvCoordWarpingGui.h"

typedef struct  {
	int type;
	string key;

	float* f;
	int* i;
	bool* b;

} ofxTSPSGUICustomParam;

class ofxTSPSGuiManager {

  public:
	ofxTSPSGuiManager();
	//ofxSimpleGuiToo	gui;

	bool enableGui;
	void setupQuadGui ( int cameraWidth, int cameraHeight );
	void drawQuadGui();
	void drawQuadGui( int x, int y, int width, int height );

	void update(ofEventArgs &e);
	void draw(ofEventArgs &e);

	//forwarded events
	void mousePressed(ofMouseEventArgs &e);
	void mouseDragged(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);

	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);

	//get + add parameters
	void addSlider(string name, int* value, int min, int max);
	void addSlider(string name, float* value, float min, float max);
	void addToggle(string name, bool* value);

	//get + set panel info
	int getSelectedPanel();
	void setSelectedPanel( int index );
	void setSelectedPanel( string name );

	void loadSettings ( string xml );
	
  protected:
	//a little goofy way to keep track of custom params. works but i'm open to suggestions
	ofxLabGui panel;
	vector<ofxTSPSGUICustomParam> params;

	//an even goofier way to easily add the quad qui
	bool quadGuiSetup;
	ofxCvCoordWarpingGui quadGui;
	int cameraWidth, cameraHeight;
	int currentPanel, lastCurrentPanel;

	void saveEventCatcher( string & buttonName);
	void reloadEventCatcher( string & buttonName);
	void loadEventCatcher( string & buttonName);
	void saveAsEventCatcher( string & buttonName);
	
	simpleFileLister* haarFiles;
};
