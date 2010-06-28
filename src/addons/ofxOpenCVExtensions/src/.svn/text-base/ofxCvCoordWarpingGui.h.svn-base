/***********************************************************************
 
 ofxCvCoordWarpingGui.h
 CYAPeopleVision

 Copyright (c) 2009, LAB at Rockwell Group
 http://lab.rockwellgroup.com
 Created by Brett Renfer on 1/14/10.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 
 This is a super simple draggable GUI for the ofxCvCoordWarping functionality
 
 ***********************************************************************/

#ifndef _GUI_QUAD_
#define _GUI_QUAD_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "ofxMSAInteractiveObject.h"

class ofxCvCoordWarpingGui : public ofxMSAInteractiveObject {
	
public:
	ofxCvCoordWarpingGui();
		
	void setup(string _quadName);
	void readFromFile(string filePath);
	void loadSettings();
	
	void releaseAllPoints();
	//these should be in the range x(0-maxX) y(0-maxH) 
	//with 	width  = maxW
	//		height = maxH 
	void setQuadPoints( ofPoint * inPts );		
	
	bool selectPoint(float x, float y, float offsetX, float offsetY, float scaleWidth, float scaleHeight, float hitArea);
	bool updatePoint(float x, float y, float offsetX, float offsetY, float scaleWidth, float scaleHeight);
	
	//returns pts to width by height range 
	ofPoint * getScaledQuadPoints(float width, float height);
	//returns pts in 0-1 scale
	ofPoint * getQuadPoints();		
	void saveToFile(string filePath, string newQuadName);
	void saveToFile(string filePath);
	void draw(float x, float y, float width, float height, int red, int green, int blue, int thickness);
	void draw(float x, float y, float width, float height);
	void draw();
	
	void setAutoSave( bool autoSave=true);
	
	//Added MSAInteractiveObject stuff to simplify
	void _mousePressed(ofMouseEventArgs &e);
	//void onDragOver(int x, int y, int button);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);
	void onReleaseOutside(int x, int y, int button);
	
	//added scaling
	void setScale( float scaleX, float scaleY );
	ofPoint scale;
	
protected:
	ofxXmlSettings	xml;
	string xmlFile;
	ofPoint srcZeroToOne[4];
	ofPoint srcScaled[4];
	ofPoint returnScaled[4];
	string quadName;
	bool bAutoSave;
	int selected;
};

#endif