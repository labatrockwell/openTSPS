/*
 *  guiTypeButton.h
 *  CYAPeopleVision
 *
 *  Created by Brett Renfer on 2/10/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 
 This works differently from a toggle, in that it broadcasts
 an event with its name on press. 
 
 */

#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeButton: public guiBaseObject {
	
public:
	
	ofEvent<string>buttonPressed;
	
	//------------------------------------------------
	void updateBoundingBox(){
		hitArea.y = boundingBox.y;
		boundingBox.height = hitArea.height;
	}
	
	//------------------------------------------------
	void setup(string buttonName, int width, int height){
		name = buttonName;
		displayText.setText(name);
		setDimensions(width, height);
	}
	
	//-----------------------------------------------.
	void updateGui(float x, float y, bool firstHit, bool isRelative){
		if(!firstHit)return;
		
		if( state == SG_STATE_SELECTED){
			ofNotifyEvent(buttonPressed, name, this);
			
			if( value.getValueI() == 0 ){
				value.setValue(1, 0);
			} else {
				value.setValue(0, 0);
			}
		}
	}
	
	//-----------------------------------------------.
	void render(){
		ofPushStyle();
		glPushMatrix();
		
		//draw the background
		ofFill();
		glColor4fv(bgColor.getColorF());
		ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
		
		//draw the outline
		ofNoFill();
		glColor4fv(outlineColor.getColorF());
		ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
		ofRect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
		
		ofFill();
		//glColor4fv(fgColor.getColorF());
		ofSetColor(0xffffff);
		guiBaseObject::renderText((int)(hitArea.x + hitArea.width/2 - (displayText.getTextWidth()/2)), hitArea.y);
		
		glPopMatrix();
		ofPopStyle();
	}
	
	
};
