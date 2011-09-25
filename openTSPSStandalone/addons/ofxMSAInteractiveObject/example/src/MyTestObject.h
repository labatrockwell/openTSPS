/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

/********  Test sample for ofxInteractiveObject									********/
/********  Make sure you open your console to see all the events being output	********/


#pragma once

#include "ofxMSAInteractiveObject.h"

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x00FF00
#define		DOWN_COLOR		0xFF0000


class MyTestObject : public ofxMSAInteractiveObject {
public:
	void setup() {
		printf("MyTestObject::setup() - hello!\n");
		enableMouseEvents();
		enableKeyEvents();
	}
	
	
	void exit() {
		printf("MyTestObject::exit() - goodbye!\n");
	}
	
	
	void update() {
		//		x = ofGetWidth()/2 + cos(ofGetElapsedTimef() * 0.2) * ofGetWidth()/4;
		//		y = ofGetHeight()/2 + sin(ofGetElapsedTimef() * 0.2) * ofGetHeight()/4;
	}
	
	
	void draw() {
		if(isMouseDown()) ofSetColor(DOWN_COLOR);
		else if(isMouseOver()) ofSetColor(OVER_COLOR);
		else ofSetColor(IDLE_COLOR);
		
		ofRect(x, y, width, height);
	}
	
	virtual void onRollOver(int x, int y) {
		printf("MyTestObject::onRollOver(x: %i, y: %i)\n", x, y);
	}
	
	virtual void onRollOut() {
		printf("MyTestObject::onRollOut()\n");
	}
	
	virtual void onMouseMove(int x, int y){
		printf("MyTestObject::onMouseMove(x: %i, y: %i)\n", x, y);
	}
	
	virtual void onDragOver(int x, int y, int button) {
		printf("MyTestObject::onDragOver(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onDragOutside(int x, int y, int button) {
		printf("MyTestObject::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onPress(int x, int y, int button) {
		printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onRelease(int x, int y, int button) {
		printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onReleaseOutside(int x, int y, int button) {
		printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void keyPressed(int key) {
		printf("MyTestObject::keyPressed(key: %i)\n", key);
	}
	
	virtual void keyReleased(int key) {
		printf("MyTestObject::keyReleased(key: %i)\n", key);
	}
	
};