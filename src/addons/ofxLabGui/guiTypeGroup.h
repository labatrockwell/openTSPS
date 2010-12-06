/*
 *  guiTypeGroup.h
 *  CYAPeopleVision
 *
 *  Created by Brett Renfer on 1/28/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 */

#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeGroup : public guiBaseObject{
	
public:
	
	//------------------------------------------------
	guiTypeGroup(){
		currentXPos     = 0;
		currentYPos     = 0;
		spacingAmntX    = 10;
		spacingAmntY    = 10;
		
		columns.clear();
		columns.push_back(ofRectangle(20, 20, 30, 20));
		col = 0;
		active			= true;
		alpha			= 255;
	}
	
	//------------------------------------------------
	void setup(string groupName, float defaultX = 10, float defaultY = 10){
		name = groupName;
		
		columns[0] = ofRectangle(defaultX, defaultY, 10, 10);
		
		//we don't want our panel flashing when we click :)
		bgColor.selected = bgColor.color;
		outlineColor.selected = outlineColor.color;
	}
	
	//-----------------------------------------------
	void setElementSpacing(float spacingX, float spacingY){
		spacingAmntX = spacingX;
		spacingAmntY = spacingY;
	}
			
	void setActive( bool _active=true ){
		active = _active;
		if (active){
			bgColor.setColor(bgColor.getColor().r*255.0f, bgColor.getColor().g*255.0f, bgColor.getColor().b*255.0f, 255);			
		} else {
			bgColor.setColor(bgColor.getColor().r*255.0f, bgColor.getColor().g*255.0f, bgColor.getColor().b*255.0f, 150);
		}
	}
	
	//-----------------------------------------------.
	virtual bool checkHit(float x, float y, bool isRelative){
		if(readOnly)return false;
		if( x >= hitArea.x && x <= hitArea.x + hitArea.width && y >= hitArea.y && y <= hitArea.y + hitArea.height){
			state = SG_STATE_SELECTED;
			
			float xx = x - boundingBox.x;
			float yy = y - boundingBox.y;
			
			setSelected();
			updateGui(x, y, true, isRelative);
							
			float offsetX = x - hitArea.x;
			float offsetY = y - hitArea.y;
			
			for(int i = 0; i < children.size(); i++){
				children[i]->bTextEnterMode = false;
				children[i]->release();
			}
			
			for(int i = 0; i < children.size(); i++){
				if (!bTextEnterMode){
					children[i]->checkHit(offsetX, offsetY, isRelative);
					if (children[i]->bTextEnterMode){
						bTextEnterMode = true;
					}
				} else {
					children[i]->bTextEnterMode = false;
					children[i]->setNormal();
				}
			}
			return true;
		} else {
			resetChildren();
		}
		return false;
	}
	
	
	//-----------------------------------------------.
	void keyPressed(int key){
		if( !locked ){
			for(int i = 0; i < children.size(); i++){
				if (children[i]->state == SG_STATE_SELECTED) children[i]->keyPressed(key);
			}
		}
	}
	
	//-----------------------------------------------.
	void updateGui(float x, float y, bool firstHit, bool isRelative){
		//if( state == SG_STATE_SELECTED){
			
			float offsetX = 0;
			float offsetY = 0;
			
			if( isRelative ){
				offsetX = x;
				offsetY = y;
			}else{
				offsetX = x - hitArea.x;
				offsetY = y - hitArea.y;
			}
			
			if( !locked ){
				
				for(int i = 0; i < children.size(); i++){
					children[i]->updateGui(offsetX, offsetY, firstHit, isRelative);
				}				
			}
		//}
	}
	
	//we should actually be checking our child heights
	//every frame to see if the panel needs to adjust layout
	//for now we only check heights when elements are first added
	//----------------------------------------------
	virtual void update(){
		updateText();
				
		for(int i = 0; i < children.size(); i++){
			children[i]->update();
		}
		
		for(int i = 0; i < whichColumn.size(); i++){
			if( children[i]->boundingBox.x != columns[whichColumn[i]].x ){
				float amntToShiftX = (int)columns[whichColumn[i]].x - children[i]->boundingBox.x;
				
				children[i]->hitArea.x += amntToShiftX;
				children[i]->boundingBox.x += amntToShiftX;
			}
		}
	}
	
	//-----------------------------------------------
	void addElement( guiBaseObject * element ){
		element->updateText();
		element->setPosition(columns[col].x, columns[col].y);
		whichColumn.push_back(col);
		
		//add the element to the panel list
		children.push_back( element );
		
		//update the current position for the next element
		columns[col].y += element->getHeight() + spacingAmntY;
		
		float checkWidth = element->getWidth();
		if(checkWidth >= columns[col].width ){
			float amnt = checkWidth - columns[col].width;
			columns[col].width += amnt;
			
			for(int i = col+1; i < columns.size(); i++){
				columns[i].x += amnt;
			}
		}
				
		float checkHeight = element->boundingBox.y + element->getHeight();
		if(checkHeight >= boundingBox.height ){
			float amnt = checkHeight - boundingBox.height;
			boundingBox.height = columns[col].y;
			hitArea.height = columns[col].y;
			
			for(int i = col+1; i < columns.size(); i++){
				columns[i].y += amnt;
			}
		}
		
		//see if we need to resize!
		//checkResize(element);
		
	}
	
	//-----------------------------------------------.
	void render(){
		ofPushStyle();
		ofSetHexColor(0xffffff);
		glPushMatrix();
			glTranslatef(boundingBox.x, boundingBox.y, 0);
			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(0, 0, boundingBox.width, boundingBox.height);
			
			//draw the outline
			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(0, 0, boundingBox.width, boundingBox.height);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(boundingBox.x, boundingBox.y, 0);
			renderText(spacingAmntX,spacingAmntY / 2);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(hitArea.x, hitArea.y, 0);
		for(int i = 0; i < children.size(); i++){
			children[i]->render();
		}
		glPopMatrix();
		
		
		ofPopStyle();
	}
	
	int alpha;
		
	float currentXPos;
	float currentYPos;
	float spacingAmntX;
	float spacingAmntY;
	
	vector <ofRectangle> columns;
	vector <int> whichColumn;
	int col;
	int row;
	bool active;
};
