/*
 *  ofxTSPSGuiManager.cpp
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

#include "ofxTSPSGuiManager.h"
#include "ofxTSPSSettings.h"

enum{
	PARAM_INT, PARAM_FLOAT, PARAM_BOOL
};

ofxTSPSGuiManager::ofxTSPSGuiManager() {
	//JG TODO add drawing event
	ofAddListener(ofEvents.update, this, &ofxTSPSGuiManager::update);
	ofAddListener(ofEvents.draw, this, &ofxTSPSGuiManager::draw);
	
	ofAddListener(ofEvents.mousePressed, this, &ofxTSPSGuiManager::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &ofxTSPSGuiManager::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxTSPSGuiManager::mouseReleased);
	ofAddListener(ofEvents.keyPressed, this, &ofxTSPSGuiManager::keyPressed);
	
	//ofxTSPSSettings *p_Settings;
	//p_Settings = ofxTSPSSettings::getInstance();
	
	enableGui = true;
	
	//create + setup layout of panel
	panel.setup("settings", 10, 10, 330, 680);
	panel.loadFont("fonts/times.ttf", 10);
	
	//panel layout
	
		panel.setPosition(10, 10);
		panel.setShowText(false);
		panel.setDimensions(330, 680); //yikes... autospacing is not working so well
		panel.setCollapsible(false);
		panel.setDraggable(false);
		panel.setBackgroundColor(255,255,255);
		panel.setOutlineColor(255,255,255,0);
		panel.setTextColor(148,129,85);
		panel.setSaveColor(58, 187, 147);
		panel.setLoadColor(180, 87, 128);
		panel.setSaveAsColor(238, 53, 35);
		panel.setSaveSelectedColor(80, 253, 190);
		panel.setRestoreColor(34, 151, 210);
		panel.setRestoreSelectedColor(116, 191, 228);
		panel.setDrawBarOnTop(false);
				
		guiTypePanel * videoPanel = panel.addPanel("video", 1, false);
		videoPanel->setDrawLock( false );	
		videoPanel->setBackgroundColor(174,139,138);
		videoPanel->setBackgroundSelectColor(174,139,138);
	
		guiTypePanel * backgroundPanel = panel.addPanel("background", 1, false);
		backgroundPanel->setDrawLock( false );	
		backgroundPanel->setBackgroundColor(213,105,68);
		backgroundPanel->setBackgroundSelectColor(213,105,68);
	
		guiTypePanel * differencingPanel = panel.addPanel("differencing", 1, false);
		differencingPanel->setDrawLock( false );	
		differencingPanel->setBackgroundColor(113,171,154);
		differencingPanel->setBackgroundSelectColor(113,171,154);
	
		guiTypePanel * sensingPanel = panel.addPanel("sensing", 1, false);
		sensingPanel->setDrawLock( false );
		sensingPanel->setBackgroundColor(191,120,0);
		sensingPanel->setBackgroundSelectColor(191,120,0);
		
		guiTypePanel * communicationPanel = panel.addPanel("communication", 1, false);
		communicationPanel->setDrawLock( false );
		communicationPanel->setBackgroundColor(180,87,128);
		communicationPanel->setBackgroundSelectColor(180,87,128);
		
	#ifdef USE_CUSTOM_GUI
		guiTypePanel * customPanel = panel.addPanel("custom", 1, false);
		customPanel->setDrawLock( false );
		customPanel->setBackgroundColor(218,173,90);
		customPanel->setBackgroundSelectColor(191,120,0);
	#endif		
	
	// video settings		
		panel.setWhichPanel("video");	
		guiTypeGroup * amplificationGroup = panel.addGroup("amplification");
		amplificationGroup->setBackgroundColor(148,129,85);
		amplificationGroup->setBackgroundSelectColor(148,129,85);
		amplificationGroup->seBaseColor(244,136,136);
		amplificationGroup->setShowText(false);
		panel.addToggle("use amplification (video gain)", "USE_AMPLIFICATION", false);
		panel.addSlider("amplification amount:", "AMPLIFICATION_AMOUNT", 1, 1, 100, true);
	
	//background settings
			
		panel.setWhichPanel("background");
		guiTypeGroup * backgroundGroup = panel.addGroup("background");
		backgroundGroup->setBackgroundColor(148,129,85);
		backgroundGroup->setBackgroundSelectColor(148,129,85);
		backgroundGroup->seBaseColor(58,187,147);
		backgroundGroup->setShowText(false);
		backgroundGroup->setActive(true);
		//TODO: use the button class for this maybe?
		panel.addToggle("capture background", "LEARN_BACKGROUND", false);
	
		guiTypeGroup * relearnGroup = panel.addGroup("background relearn");
		relearnGroup->setBackgroundColor(148,129,85);
		relearnGroup->setBackgroundSelectColor(148,129,85);
		relearnGroup->seBaseColor(180,87,128);
		relearnGroup->setShowText(false);
		panel.addToggle("progressive background recapture", "RELEARN", false);
		panel.addSlider("recapture rate :", "RELEARN_BACKGROUND", .1f, 0.0f, 1000.0f, false);
	
	//differencing settings
	
		panel.setWhichPanel("differencing");
		guiTypeGroup * thresholdGroup = panel.addGroup("threshold group");	
		thresholdGroup->setBackgroundColor(148,129,85);
		thresholdGroup->setBackgroundSelectColor(148,129,85);
		thresholdGroup->seBaseColor(255,136,37);
		thresholdGroup->setShowText(false);
		thresholdGroup->setActive(true);
		panel.addSlider("threshold:", "THRESHOLD", 40, 0, 255, false);
		vector<string> multi;
		multi.push_back("light on dark");
		multi.push_back("dark on light");
		multi.push_back("absolute difference");
		panel.addMultiToggle("type of differencing:", "BLOB_TYPE", 0, multi);
			
		guiTypeGroup * highpassGroup = panel.addGroup("highpass");
		highpassGroup->setBackgroundColor(148,129,85);
		highpassGroup->setBackgroundSelectColor(148,129,85);
		highpassGroup->seBaseColor(58,187,147);
		highpassGroup->setShowText(false);
		panel.addToggle("use highpass", "USE_HIGHPASS", true);
		panel.addSlider("highpass blur:", "HIGHPASS_BLUR", 1, 1, 200, true);
		panel.addSlider("highpass noise:", "HIGHPASS_NOISE", 1, 1, 30, true);
	
		guiTypeGroup * smoothingGroup = panel.addGroup("smoothing");		
		smoothingGroup->setBackgroundColor(148,129,85);
		smoothingGroup->setBackgroundSelectColor(148,129,85);
		smoothingGroup->seBaseColor(34,151,210);
		smoothingGroup->setShowText(false);
		panel.addToggle("use shape smoothing", "USE_SMOOTHING", false);
		panel.addSlider("smooth amount:", "SMOOTH_AMOUNT", 0, 0, 15, false);		
	
	//sensing
		
		panel.setWhichPanel("sensing");
		//TODO optionally disable people?
				
		guiTypeGroup * blobGroup = panel.addGroup("blobs");
		blobGroup->setBackgroundColor(148,129,85);
		blobGroup->setBackgroundSelectColor(148,129,85);
		blobGroup->seBaseColor(238,218,0);
		blobGroup->setShowText(false);
		blobGroup->setActive(true);

		//JG 12/8/09 GUI-REDUX Removing this feature
		panel.addSlider("minimum blob size (% of view):", "MIN_BLOB", 1.f, 0.01f, 2.0f, false);
		panel.addSlider("maximum blob size (% of view):", "MAX_BLOB", .50f, 0.1f, 100.f, false);
		panel.addToggle("ignore nested blobs", "FIND_HOLES", false);
					
		guiTypeGroup * optionsGroup = panel.addGroup("options");
		optionsGroup->setBackgroundColor(148,129,85);
		optionsGroup->setBackgroundSelectColor(148,129,85);
		optionsGroup->seBaseColor(58,187,147);
		optionsGroup->setShowText(false);
		panel.addToggle("track and send contours", "SEND_OSC_CONTOURS", false);
	
		guiTypeGroup * opticalGroup = panel.addGroup("optical flow");
		opticalGroup->setBackgroundColor(148,129,85);
		opticalGroup->setBackgroundSelectColor(148,129,85);
		opticalGroup->seBaseColor(34,151,210);
		opticalGroup->setShowText(false);
		//optical flow
		panel.addToggle("track and send optical flow in blobs", "SENSE_OPTICAL_FLOW", true);
		panel.addSlider("filter vectors smaller than:", "MIN_OPTICAL_FLOW", 0, 0, 5.0, false);
		panel.addSlider("clamp vectors: larger than", "MAX_OPTICAL_FLOW", 10, 5.0, 200, false);
		
		guiTypeGroup * haarGroup = panel.addGroup("haar tracking");
		haarGroup->setBackgroundColor(148,129,85);
		haarGroup->setBackgroundSelectColor(148,129,85);
		haarGroup->seBaseColor(238,53,35);
		haarGroup->setShowText(false);
		
		// haar
		panel.addToggle("detect and send features in blobs", "SENSE_HAAR", false);
		//	ofEnableDataPath();
		//	ofSetDataPathRoot("data/");
		haarFiles = new simpleFileLister();	
		int numHaar = haarFiles->listDir(ofToDataPath("haar", true));
		ofLog(OF_LOG_VERBOSE, "haar files found " + numHaar);
		panel.addFileLister("types of features:", haarFiles, 240, 100);
		panel.addSlider("expand detection area:", "HAAR_PADDING", 0.0f, 0.0f, 200.0f, false);
		
		//JG GUI-REDUX: removing this feature
		//gui.addToggle("send haar center as blob center", &p_Settings->bUseHaarAsCenter);
		//JG 1/21/10 disabled this feature to simplify the interface
		//	panel.addSlider("min. checkable haar size (%)", "MIN_HAAR", .1f, 0.0001f, 1.0f, false);
		//	panel.addSlider("max. checkable haar size (%)", "MAX_HAAR", .5f, 0.0001f, 1.0f, false);
		
	//communication
		
		panel.setWhichPanel("communication");
		panel.setWhichColumn(0);
		
		guiTypeGroup * oscGroup = panel.addGroup("OSC");
		oscGroup->setBackgroundColor(148,129,85);
		oscGroup->setBackgroundSelectColor(148,129,85);
		oscGroup->seBaseColor(238,53,35);
		oscGroup->setShowText(false);
		panel.addToggle("send OSC", "SEND_OSC", false);
		panel.addTextField("receiver IP address (OSC host) :", "OSC_HOST", "127.0.0.1", 200, 20);
		panel.addTextField("receiver port (OSC port) :", "OSC_PORT", "12000", 200, 20);
		
		guiTypeGroup * tuioGroup = panel.addGroup("TUIO");
		tuioGroup->setBackgroundColor(148,129,85);
		tuioGroup->setBackgroundSelectColor(148,129,85);
		tuioGroup->seBaseColor(207,202,65);
		tuioGroup->setShowText(false);
		panel.addToggle("send TUIO", "SEND_TUIO", false);
		panel.addTextField("receiver IP address (TUIO host):", "TUIO_HOST", "127.0.0.1", 200, 20);
		panel.addTextField("receiver port (TUIO port):", "TUIO_PORT", "3333", 200, 20);
		
		guiTypeGroup * tcpGroup = panel.addGroup("TCP");
		tcpGroup->setBackgroundColor(148,129,85);
		tcpGroup->setBackgroundSelectColor(148,129,85);
		tcpGroup->seBaseColor(238,53,35);
		tcpGroup->setShowText(false);
		panel.addToggle("send TCP", "SEND_TCP", false);
		panel.addTextField("broadcast port (TCP port):", "TCP_PORT", "8888", 200, 20);
		
	//JG TODO: Optionally change config file through the UI
	//this would be a big help for setting up multiple install sites and having those setting
	//included in repositories
	//ofSetDataPathRoot("data");
	loadSettings("settings/settings.xml");
	
	//listen for save / load / reload / saveas events from GUI + pass to quadgui
	
	ofAddListener(panel.getSaveButton()->buttonPressed, this, &ofxTSPSGuiManager::saveEventCatcher);
	ofAddListener(panel.getRestoreButton()->buttonPressed, this, &ofxTSPSGuiManager::reloadEventCatcher);
	ofAddListener(panel.getLoadButton()->buttonPressed, this, &ofxTSPSGuiManager::loadEventCatcher);
	ofAddListener(panel.getSaveAsButton()->buttonPressed, this, &ofxTSPSGuiManager::saveAsEventCatcher);
		
	//set active panel to be differencing
	panel.setSelectedPanel("differencing");
}

void ofxTSPSGuiManager::addSlider(string name, int* value, int min, int max)
{
	ofxTSPSGUICustomParam p;
	char key[1024];
	sprintf(key, "CUSTOM%d",params.size());
	
	panel.setWhichPanel("custom");
	panel.setWhichColumn(0);	
	panel.addSlider(name, key, *value, min, max, true);
	
	p.key = key;
	p.type = PARAM_INT;
	p.i = value;
	params.push_back(p);
}

void ofxTSPSGuiManager::addSlider(string name, float* value, float min, float max)
{
	ofxTSPSGUICustomParam p;
	char key[1024];
	sprintf(key, "CUSTOM%d",params.size());
	
	panel.setWhichPanel("custom");
	panel.setWhichColumn(0);
	panel.addSlider(name, key, *value, min, max, false);
	
	p.key = key;
	p.type = PARAM_FLOAT;
	p.f = value;
	params.push_back(p);
}

void ofxTSPSGuiManager::addToggle(string name, bool* value)
{
	ofxTSPSGUICustomParam p;	
	char key[1024];
	sprintf(key, "CUSTOM%d",params.size());
	
	panel.setWhichPanel("custom");
	panel.setWhichColumn(0);
	panel.addToggle(name, key, *value);
	
	p.key = key;
	p.type = PARAM_BOOL;
	p.b = value;
	params.push_back(p);
}

/***************************************************************
	GET + SET PANEL INFO / SELECTED PANEL
***************************************************************/

int ofxTSPSGuiManager::getSelectedPanel(){
	return panel.getSelectedPanel();
}

void ofxTSPSGuiManager::setSelectedPanel( int index ){
	panel.setWhichPanel(index);
}

void ofxTSPSGuiManager::setSelectedPanel( string name ){
	panel.setWhichPanel(name);
}

void ofxTSPSGuiManager::update(ofEventArgs &e)
{
	if(!enableGui){
		//if the gui is not shown no need to propagate values
		return;
	}
	
	ofxTSPSSettings *p_Settings;
	p_Settings = ofxTSPSSettings::getInstance();
	
	panel.update();
	
	p_Settings->bLearnBackground = panel.getValueB("LEARN_BACKGROUND");
	
	p_Settings->threshold = panel.getValueF("THRESHOLD");
	p_Settings->bSmooth = panel.getValueB("USE_SMOOTHING");
	p_Settings->smooth = panel.getValueF("SMOOTH_AMOUNT");
	panel.setGroupActive("differencing", "smoothing", p_Settings->bSmooth);
	
	p_Settings->bHighpass = panel.getValueI("USE_HIGHPASS");
	p_Settings->highpassBlur =  panel.getValueI("HIGHPASS_BLUR");
	p_Settings->highpassNoise = panel.getValueI("HIGHPASS_NOISE");
	panel.setGroupActive("differencing", "highpass", p_Settings->bHighpass);
	
	p_Settings->bAmplify = panel.getValueB("USE_AMPLIFICATION");
	p_Settings->highpassAmp = panel.getValueI("AMPLIFICATION_AMOUNT");
	panel.setGroupActive("video", "amplification", p_Settings->bAmplify);
	
	p_Settings->bLearnBackground = panel.getValueB("LEARN_BACKGROUND");
	if(p_Settings->bLearnBackground){ 
		panel.setValueB("LEARN_BACKGROUND", false);
	}
	
	//panel.setValueB("LEARN_BACKGROUND", p_Settings->bLearnBackground);
	//JG 12/8/09 GUI-REDUX Removing this feature
	//gui.addToggle("smart learn background", &p_Settings->bSmartLearnBackground);
	
	p_Settings->minBlob = panel.getValueF("MIN_BLOB")/100.0f; //scale var to be right for tracker
	p_Settings->maxBlob = panel.getValueF("MAX_BLOB")/100.0f;	//scale var to be right for tracker
	p_Settings->bLearnBackgroundProgressive = panel.getValueB("RELEARN");
	p_Settings->fLearnRate = panel.getValueF("RELEARN_BACKGROUND");
	panel.setGroupActive("background", "background relearn", p_Settings->bLearnBackgroundProgressive);
	
	p_Settings->bFindHoles = !(panel.getValueB("FIND_HOLES"));
	p_Settings->bTrackOpticalFlow = panel.getValueB("SENSE_OPTICAL_FLOW");
	panel.setGroupActive("sensing", "optical flow", p_Settings->bTrackOpticalFlow);
	
	//JG 12/8/09 GUI-REDUX:
	p_Settings->minOpticalFlow = panel.getValueF("MIN_OPTICAL_FLOW");
	p_Settings->maxOpticalFlow = panel.getValueF("MAX_OPTICAL_FLOW");
	p_Settings->trackType = panel.getValueI("BLOB_TYPE");
	p_Settings->bDetectHaar = panel.getValueB("SENSE_HAAR");
	panel.setGroupActive("sensing", "haar tracking", p_Settings->bDetectHaar);

	if(haarFiles->getSelectedName() != ""){
		p_Settings->haarFile = haarFiles->getSelectedName();
	}
	p_Settings->haarAreaPadding = panel.getValueF("HAAR_PADDING");
	//JG GUI-REDUX: removing this feature
	//gui.addToggle("send haar center as blob center", &p_Settings->bUseHaarAsCenter);
	//JG 1/21/10 disabled this feature to simplify the interface
//	p_Settings->minHaarArea = panel.getValueF("MIN_HAAR");
//	p_Settings->maxHaarArea = panel.getValueF("MAX_HAAR");
	
	//update osc stuff
	p_Settings->bSendOsc = panel.getValueB("SEND_OSC");
	p_Settings->bSendTuio = panel.getValueB("SEND_TUIO");
	p_Settings->bSendTcp = panel.getValueB("SEND_TCP");
	p_Settings->oscHost = panel.getValueS("OSC_HOST", 0, "localhost");
	p_Settings->oscPort = (int) atoi(panel.getValueS("OSC_PORT", 0, "12000").c_str());
	p_Settings->tuioHost = panel.getValueS("TUIO_HOST", 0, "localhost");
	p_Settings->tuioPort = (int) atoi(panel.getValueS("TUIO_PORT", 0, "3333").c_str());
	p_Settings->tcpPort = (int) atoi(panel.getValueS("TCP_PORT", 0, "8888").c_str());
	p_Settings->bSendOscContours = panel.getValueB("SEND_OSC_CONTOURS");
	panel.setGroupActive("sensing", "options", p_Settings->bSendOscContours);
	
	panel.setGroupActive("communication", "OSC", p_Settings->bSendOsc);
	panel.setGroupActive("communication", "TUIO", p_Settings->bSendTuio);
	panel.setGroupActive("communication", "TCP", p_Settings->bSendTcp);
	
	// UPDATE GUI QUADS HERE
	// because this returns a pointer to the actual points that get updated,
	// you store it in an array so it doesn't get updated when it draws
	ofPoint * scaledPoints = quadGui.getScaledQuadPoints(cameraWidth,cameraHeight);
	for (int i=0; i<4; i++){
		p_Settings->quadWarpScaled[i] = scaledPoints[i];
	}
	
	//modify custom parameters
	vector<ofxTSPSGUICustomParam>::iterator it;
	for(it = params.begin(); it != params.end(); it++){
		ofxTSPSGUICustomParam p = *it;
		switch (p.type) {
			case PARAM_INT:
				*p.i = panel.getValueI(p.key);
				break;
			case PARAM_FLOAT:
				*p.f = panel.getValueF(p.key);
				break;
			case PARAM_BOOL:
				*p.b = panel.getValueB(p.key);
				break;
			default:
				printf("ofxTSPSGUIManager: WARNING undefined parameter type encountered\n");
				break;
		}
	}
	
	//get xml
	p_Settings->currentXmlFile = panel.getCurrentXMLFile();
	
	//get current panel
	p_Settings->lastCurrentPanel = p_Settings->currentPanel;
	p_Settings->currentPanel = panel.getSelectedPanel();
}

//BR: Added some messiness here to setup, draw, and update the gui quad...

void ofxTSPSGuiManager::setupQuadGui ( int _cameraWidth, int _cameraHeight )
{
	cameraWidth = _cameraWidth;
	cameraHeight = _cameraHeight;
	ofxTSPSSettings *p_Settings;
	p_Settings = ofxTSPSSettings::getInstance();
	
	// give the gui quad a starting setting
	
	ofPoint quadSrc[4];
	quadSrc[0].set(0, 0);
	quadSrc[1].set(cameraWidth, 0);
	quadSrc[2].set(cameraWidth, cameraHeight);
	quadSrc[3].set(0, cameraHeight);
	quadGui.setQuadPoints(quadSrc);
	
	// give the gui quad a default setting
	p_Settings->quadWarpOriginal[0].set(0, 0);
	p_Settings->quadWarpOriginal[1].set(cameraWidth, 0);
	p_Settings->quadWarpOriginal[2].set(cameraWidth, cameraHeight);
	p_Settings->quadWarpOriginal[3].set(0, cameraHeight);
	
	//BR TO DO: add this into the normal settings file
	quadGui.width = cameraWidth;
	quadGui.height = cameraHeight;
	quadGui.disableAppEvents();
	
	quadGuiSetup = true;
};

void ofxTSPSGuiManager::drawQuadGui(){
	quadGui.draw();
};

void ofxTSPSGuiManager::drawQuadGui( int x, int y, int width, int height ){
	quadGui.x = x;
	quadGui.y = y;
	quadGui.setScale((float) width/quadGui.width, (float) height/quadGui.height );
	drawQuadGui();
};

/***************************************************************
 GET EVENTS FROM GUI BUTTONS
 ***************************************************************/

void ofxTSPSGuiManager::saveEventCatcher( string & buttonName){
	ofxTSPSSettings *p_Settings;
	p_Settings = ofxTSPSSettings::getInstance();
	p_Settings->currentXmlFile = panel.getCurrentXMLFile();
	quadGui.saveToFile(p_Settings->currentXmlFile);
};

void ofxTSPSGuiManager::reloadEventCatcher( string & buttonName){
	ofxTSPSSettings *p_Settings;
	p_Settings = ofxTSPSSettings::getInstance();
	quadGui.readFromFile(p_Settings->currentXmlFile);
};

void ofxTSPSGuiManager::loadEventCatcher( string & buttonName){
	ofxTSPSSettings *p_Settings;
	p_Settings = ofxTSPSSettings::getInstance();
	p_Settings->currentXmlFile = panel.getCurrentXMLFile();
	quadGui.readFromFile(p_Settings->currentXmlFile);
};

void ofxTSPSGuiManager::saveAsEventCatcher( string & buttonName){
	ofxTSPSSettings *p_Settings;
	p_Settings = ofxTSPSSettings::getInstance();
	p_Settings->currentXmlFile = panel.getCurrentXMLFile();
	quadGui.readFromFile(p_Settings->currentXmlFile);
};

/***************************************************************
 GET + SET PANEL INFO / SELECTED PANEL
 ***************************************************************/

//forward to gui
void ofxTSPSGuiManager::mousePressed(ofMouseEventArgs &e)
{
	if(enableGui) panel.mousePressed(e.x, e.y, e.button);
	if(quadGuiSetup){
		ofxTSPSSettings *p_Settings;
		p_Settings = ofxTSPSSettings::getInstance();
	}
}

void ofxTSPSGuiManager::mouseDragged(ofMouseEventArgs &e)
{
	if(enableGui) panel.mouseDragged(e.x, e.y, e.button);
}

void ofxTSPSGuiManager::mouseReleased(ofMouseEventArgs &e)
{
	if(enableGui) panel.mouseReleased();
}

void ofxTSPSGuiManager::loadSettings( string xmlFile ){
	panel.loadSettings(xmlFile);	
	if (quadGuiSetup) quadGui.readFromFile(xmlFile);	
};

void ofxTSPSGuiManager::keyPressed(ofKeyEventArgs &e)
{
	if(enableGui) panel.keyPressed(e.key);
};

void ofxTSPSGuiManager::keyReleased(ofKeyEventArgs &e)
{
	if(enableGui) panel.keyReleased(e.key);
};

void ofxTSPSGuiManager::draw(ofEventArgs &e) {
	if(enableGui){
		panel.draw();
	}
}

//JG TODO ADD EVENT UNREGISTER FO CLEAN UP

