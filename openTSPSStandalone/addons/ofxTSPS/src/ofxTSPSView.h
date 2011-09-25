/*
 *  ofxTSPSView.h
 *  TSPSPeopleVision
 *
 *  Created by Brett Renfer on 2/8/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 */

#pragma once
#include "ofxOpenCv.h"

#define view_smallBorder 2
#define view_largeBorder 10

class ofxTSPSView : public ofRectangle {
public:	
	
	//-------------------------------------------------
	ofxTSPSView(){
		bImage = false;
	}
	
	//-------------------------------------------------
	void setup( int _width, int _height){
		ofAddListener(ofEvents.mouseMoved, this, &ofxTSPSView::mouseMoved);
		bFontLoaded = bActive = bRolled = false;
		image.allocate(_width, _height);
	};
	
	//-------------------------------------------------
	void setImage(ofxCvGrayscaleImage _image){
		bImage = true;
		image = _image;
	}
	
	//-------------------------------------------------
	void loadFont( string fontName, int size ){
		p_font = new ofTrueTypeFont();
		p_font->loadFont(fontName, size);
		bFontLoaded = true;
	}
	
	//-------------------------------------------------
	void setFont( ofTrueTypeFont * font ){
		p_font = font;
		bFontLoaded = true;
	}
	
	//-------------------------------------------------
	void setColor(int r, int g, int b){
		color.r = r;
		color.g = g;
		color.b = b;
	}
	
	//-------------------------------------------------
	void setActive(bool active=true){
		bActive = active;
	}
	
	//-------------------------------------------------
	void setTitle( string _title, string _shortTitle = "" ){
		title = _title;
		if (_shortTitle == "") shortTitle = title;
		else shortTitle = _shortTitle;
	}
	
	//-------------------------------------------------
	void update(ofxCvGrayscaleImage _image){
		bImage = true;
		image = _image;
	}
	
	//-------------------------------------------------
	void draw(){
		ofPushStyle();
		ofDisableAlphaBlending();
		ofPushMatrix();
		ofTranslate((int)x, (int)y);
		
		//draw border
		if (bRolled && !bActive){
			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(color.r, color.g, color.b);
			ofRect(-1,-1,width+2,height+2);			
		//or draw color around all
		} else if (bActive){			
			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(color.r, color.g, color.b);
			ofRect(-1,-1,width+2,height+2);
			ofFill();	
			if (bFontLoaded) ofRect(-2,height+view_smallBorder*3 + 2,width+2,p_font->stringHeight("A") + view_largeBorder );	
			else ofRect(-2,height+view_smallBorder*3 + 1,width+2,view_largeBorder*2);
		};
		
		ofFill(); 
		ofSetHexColor(0xffffff);
		
		if (bFontLoaded){
			p_font->drawString(shortTitle, (int)view_smallBorder,(int)height + view_smallBorder*3 + 2 + p_font->stringHeight("A"));
		} else {
			ofDrawBitmapString(shortTitle, view_largeBorder, height + view_largeBorder*2);
		}
		
		if (bRolled || bActive) ofSetColor(color.r, color.g, color.b);
		if( image.bAllocated && bImage ) image.draw( 0,0, width, height );
		else if (!image.bAllocated) ofLog(OF_LOG_WARNING, title+" image not allocated!");
		
		ofPopMatrix();
		ofPopStyle();
		
	};
	
	//-------------------------------------------------
	void drawLarge( float _x, float _y, float _width, float _height){		
		if( image.bAllocated && bImage ) image.draw( _x, _y, _width, _height);
		else if (!image.bAllocated) ofLog(OF_LOG_WARNING, title+" image not allocated!");
		
		ofPushStyle();
			ofNoFill();
			ofSetColor(color.r, color.g, color.b);
			ofSetLineWidth(3);
			ofRect(_x, _y, _width, _height);
		ofPopStyle();	
				
		ofSetHexColor(0xffffff);
		if (bFontLoaded){
			p_font->drawString(title, _x + view_largeBorder, _y + view_largeBorder + p_font->stringHeight("A"));
		} else {
			ofDrawBitmapString(title, _x + view_largeBorder, _y + view_largeBorder*2);
		}
	};
	
	//-------------------------------------------------
	void mouseMoved( ofMouseEventArgs &e){
		bRolled = isInsideRect(e.x, e.y);
	}
		
	//-------------------------------------------------
	bool isInsideRect(float _x, float _y){
		return ( _x >= x && _x <= x + width && _y >= y && _y <= y + height );
	}
	
protected:
	string title, shortTitle;
	ofColor color;
	ofxCvGrayscaleImage image;
	
	ofTrueTypeFont * p_font;
	
	bool bActive;
	bool bRolled;
	bool bFontLoaded;
	bool bImage;
};
