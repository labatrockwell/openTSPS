/***********************************************************************
 
 ofxCvCoordWarpingGui.cpp
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
 
 ***********************************************************************/

#include "ofxCvCoordWarpingGui.h"

//----------------------------------------------------
ofxCvCoordWarpingGui::ofxCvCoordWarpingGui(){
	selected = -1;
	quadName = "QUAD_";
	scale.x = scale.y = 1.0f;
	bAutoSave = false;
	ofAddListener(ofEvents.mousePressed, this, &ofxCvCoordWarpingGui::_mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &ofxCvCoordWarpingGui::_mouseReleased);
	ofAddListener(ofEvents.mouseDragged, this, &ofxCvCoordWarpingGui::_mouseDragged);
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::setup(string _quadName){
	quadName = _quadName;
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::releaseAllPoints(){
	selected = -1;
}

//----------------------------------------------------
//these should be in the range x(0-maxX) y(0-maxH) 
//with 	width  = maxW
//		height = maxH 
void ofxCvCoordWarpingGui::setQuadPoints( ofPoint * inPts ){
	
	for(int i = 0; i < 4; i++){
		srcZeroToOne[i].x	= inPts[i].x;
		srcZeroToOne[i].y	= inPts[i].y;
		
		if(srcZeroToOne[i].x > 1) srcZeroToOne[i].x = 1;
		if(srcZeroToOne[i].y > 1) srcZeroToOne[i].y = 1;
	}
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::readFromFile(string filePath){
	
	xml.loadFile(filePath);
	xmlFile = filePath;
	loadSettings();	
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::loadSettings(){
	
	string str;
	
	
	for(int i = 0; i < 4; i++){
		
		//These two lines give us:
		// "QUAD_SRC_0"
		// "QUAD_SRC_1" etc...
		
		str = quadName;
		str += ofToString(i);									
		
		srcZeroToOne[i].x = xml.getValue(str+"_X", srcZeroToOne[i].x);
		srcZeroToOne[i].y = xml.getValue(str+"_Y", srcZeroToOne[i].y);		
		
		if(srcZeroToOne[i].x > 1) srcZeroToOne[i].x = 1;
		if(srcZeroToOne[i].y > 1) srcZeroToOne[i].y = 1;
		
	}	
	
}

//----------------------------------------------------
bool ofxCvCoordWarpingGui::selectPoint(float passedX, float passedY, float offsetX, float offsetY, float scaleWidth, float scaleHeight, float hitArea){
	
	//make sure selected is -1 unless we really find a point			
	selected = -1;
	
	if(scaleWidth == 0 || scaleHeight == 0 || passedX < offsetX || passedX > offsetX + scaleWidth || passedY < offsetY ||  passedY > offsetY + scaleHeight){
		//then we are out of our possible quad area
		//so we ignore :)
		return false;
	}  
	
	//lets get it in range x(0 - scaleWidth) y(0 - scaleHeight)
	float px = passedX - offsetX;
	float py = passedY - offsetY;
	
	//now get in 0-1 range
	px /= scaleWidth;
	py /= scaleHeight;
	
	hitArea /= scaleWidth;
	
	//we want to store the smallest distance found
	//because in the case when two points are in the 
	//hit area we want to pick the closet
	float storeDist = 9999999.0;
	
	for(int i = 0; i < 4; i++){
		float dx = fabs(px -  srcZeroToOne[i].x);
		float dy = fabs(py -  srcZeroToOne[i].y);
		
		float dist = sqrt(dx*dx + dy*dy);
		
		if(dist > hitArea)continue;
		
		if(dist < storeDist){
			selected = i;
			storeDist = dist;
		}
	}
	
	if(selected != -1){
		srcZeroToOne[selected].x 	= px;
		srcZeroToOne[selected].y 	= py;	
		srcScaled[selected].x		= px;
		srcScaled[selected].y		= py;
		return true;
	}
	
	return false;
}

//----------------------------------------------------
bool ofxCvCoordWarpingGui::updatePoint(float passedX, float passedY, float offsetX, float offsetY, float scaleWidth, float scaleHeight){
	
	//nothing to update
	if(selected == -1) return false;
	
	if(scaleWidth == 0 || scaleHeight == 0){
		//dangerous so we ignore :)
		return false;
	}  
	
	if( passedX < offsetX ) 			passedX = offsetX;
	if( passedX > offsetX + scaleWidth ) 	passedX = offsetX + scaleWidth;
	if( passedY < offsetY ) 			passedY = offsetY;
	if( passedY > offsetY + scaleHeight) 	passedY = offsetY + scaleHeight;
	
	//lets get it in range x(0 - scaleWidth) y(0 - scaleHeight)
	float px = passedX - offsetX;
	float py = passedY - offsetY;
	
	//now get in 0-1 range
	px /= scaleWidth;
	py /= scaleHeight;
	
	srcZeroToOne[selected].x 	= px;
	srcZeroToOne[selected].y 	= py;	
	srcScaled[selected].x		= px;
	srcScaled[selected].y		= py;
	
	return true;
}

//----------------------------------------------------
//returns pts to scaleWidth by scaleHeight range 
ofPoint * ofxCvCoordWarpingGui::getScaledQuadPoints(float scaleWidth, float scaleHeight){
		
	for(int i = 0; i < 4; i++){
		returnScaled[i].x = srcScaled[i].x = srcZeroToOne[i].x * scaleWidth;
		returnScaled[i].y = srcScaled[i].y = srcZeroToOne[i].y * scaleHeight;				
	}
	
	return returnScaled;
}


//----------------------------------------------------
//returns pts in 0-1 scale
ofPoint * ofxCvCoordWarpingGui::getQuadPoints(){
	return srcZeroToOne;
}


//----------------------------------------------------
void ofxCvCoordWarpingGui::saveToFile(string filePath, string newQuadName){
	string str;
	
	//reload XML file...	
	xml.loadFile(filePath);
	
	for(int i = 0; i < 4; i++){
		
		str = newQuadName;
		str += ofToString(i);
		
		xml.setValue(str+"_X", srcZeroToOne[i].x);
		xml.setValue(str+"_Y", srcZeroToOne[i].y);		
	}
	
	xml.saveFile(filePath);
}		

//----------------------------------------------------
void ofxCvCoordWarpingGui::saveToFile(string filePath){
	xmlFile = filePath;
	saveToFile(filePath, quadName);
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::draw(float passedX, float passedY, float scaleWidth, float scaleHeight, int red, int green, int blue, int thickness){
	
	getScaledQuadPoints(scaleWidth, scaleHeight);
	ofPushMatrix();
	ofPushStyle();
		ofTranslate(passedX, passedY);

		ofNoFill();

		ofSetColor(red, green, blue);
		ofSetLineWidth(thickness);
		ofBeginShape();
			for(int i = 0; i < 4; i++){
				ofVertex(srcScaled[i].x, srcScaled[i].y);
			}
		ofEndShape(true);
		ofFill();
		glLineWidth(1);
		ofSetRectMode(OF_RECTMODE_CENTER);
		for(int i = 0; i < 4; i++){
			
			if(i == 0)ofSetColor(255, 0, 0);
			if(i == 1)ofSetColor(0, 255, 0);
			if(i == 2)ofSetColor(255, 255, 0);
			if(i == 3)ofSetColor(0, 255, 255);
			
			ofRect(srcScaled[i].x, srcScaled[i].y, 8, 8);
		}
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofFill();

		ofSetColor(255,255,255,50);
		ofSetPolyMode(OF_POLY_WINDING_ODD);
		ofBeginShape();
			ofVertex(0, 0);
			ofVertex(scaleWidth, 0);
			ofVertex(scaleWidth, scaleHeight);
			ofVertex(0, scaleHeight);
			ofVertex(0, 0);
			for(int i = 0; i < 4; i++){
				ofVertex(srcScaled[i].x, srcScaled[i].y);
			}
			
			ofVertex(srcScaled[0].x+1, srcScaled[0].y+1);
		ofEndShape();	
	
		ofSetColor(0xffffff);
	
	ofPopStyle();
	ofPopMatrix();
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::draw(float passedX, float passedY, float scaleWidth, float scaleHeight){
	
	//default to a think yellow line
	draw(passedX, passedY, scaleWidth, scaleHeight, 255, 255, 255, 1);
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::draw(){
	
	//default to a think yellow line
	draw(x, y, width*scale.x, height*scale.y, 255, 255, 0, 1);
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::_mousePressed(ofMouseEventArgs &e){
	selectPoint(e.x, e.y, x, y, width*scale.y, height*scale.y, 40);
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::setAutoSave( bool autoSave){
	bAutoSave = autoSave;
}

//----------------------------------------------------
//void ofxCvCoordWarpingGui::onDragOutside(int mouseX, int mouseY, int button){
//	updatePoint(mouseX, mouseY, x, y, width*scale.x, height*scale.y);
//}

//----------------------------------------------------
void ofxCvCoordWarpingGui::_mouseDragged(ofMouseEventArgs &e){
	updatePoint(e.x, e.y, x, y, width*scale.x, height*scale.y);
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::_mouseReleased(ofMouseEventArgs &e){
	if (bAutoSave){
		if (xmlFile != "") saveToFile(xmlFile);
		else saveToFile("guiquad-settings.xml");
	}
}
//----------------------------------------------------
void ofxCvCoordWarpingGui::onReleaseOutside(int mouseX, int mouseY, int button){
	if (bAutoSave){
		if (xmlFile != "") saveToFile(xmlFile);
		else saveToFile("guiquad-settings.xml");
	}
}

//----------------------------------------------------
void ofxCvCoordWarpingGui::setScale( float scaleX, float scaleY ){
	scale.x = scaleX;
	scale.y = scaleY;
};

