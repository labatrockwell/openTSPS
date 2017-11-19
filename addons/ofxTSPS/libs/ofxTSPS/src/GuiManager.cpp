/*
 *  GuiManager.cpp
 *  openTSPS
 */

#include "ofxTSPS/gui/GuiManager.h"
#include "ofxTSPS/gui/Settings.h"

namespace ofxTSPS {
    enum{
        PARAM_INT, PARAM_FLOAT, PARAM_BOOL
    };
    
    GuiManager::GuiManager() {
        //JG TODO add drawing event
        //ofAddListener(ofEvents().draw, this, &GuiManager::draw);
        bLastBgState   = false;
        bEventsEnabled = false;
    }
    
    
    GuiManager::~GuiManager(){
        disableEvents();
    }
    
    void GuiManager::enableEvents(){
        if (bEventsEnabled) return;
        bEventsEnabled = true;
        ofAddListener(ofEvents().update, this, &GuiManager::update);
        ofAddListener(ofEvents().mousePressed, this, &GuiManager::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &GuiManager::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &GuiManager::mouseReleased);
        ofAddListener(ofEvents().keyPressed, this, &GuiManager::keyPressed);
        quadGui.enableEvents();
    }
    
    void GuiManager::disableEvents(){
        if (!bEventsEnabled) return;
        bEventsEnabled = false;
        ofRemoveListener(ofEvents().update, this, &GuiManager::update);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiManager::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiManager::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiManager::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &GuiManager::keyPressed);
        quadGui.disableEvents();
    }
    
    void GuiManager::setup(){
        enableGui = true;
        
        // setup each panel group
        panelGroups["source"];
        panelGroups["sensing"];
        panelGroups["communication"];
        panelGroups["data"];
        panelGroups["custom"];
        
        panel.setup("custom", 10, 10, 330, 640);
        
        buttonPosition.set(10,70);
        
        //panel layout
        panel.loadFont("fonts/times.ttf", 10);
        panel.setPosition(10, 100);
        panel.setShowText(false);
        panel.setDimensions(330, 590); //yikes... autospacing is not working so well
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
        
        // setup panel switch buttons
        
        guiTypeButton * sourceButton = new guiTypeButton();
        sourceButton->setup("source", buttonWidth*1.5, buttonHeight);
        sourceButton->setPosition(buttonPosition.x, buttonPosition.y);
        sourceButton->setDimensions(50, 20);
        sourceButton->setFont(&panel.guiTTFFont);
        sourceButton->setBackgroundColor(174,139,138,75);
        sourceButton->setBackgroundSelectColor(174,139,138);
        buttonPosition.x += 60;
        
        guiTypeButton * sensingButton = new guiTypeButton();
        sensingButton->setup("sensing", buttonWidth*1.5, buttonHeight);
        sensingButton->setPosition(buttonPosition.x, buttonPosition.y);
        sensingButton->setDimensions(60, 20);
        sensingButton->setFont(&panel.guiTTFFont);
        sensingButton->setBackgroundColor(113,171,154,75);
        sensingButton->setBackgroundSelectColor(113,171,154);
        buttonPosition.x += 70;
        
        guiTypeButton * communicationButton = new guiTypeButton();
        communicationButton->setup("communication", buttonWidth*1.5, buttonHeight);
        communicationButton->setDimensions(90, 20);
        communicationButton->setPosition(buttonPosition.x, buttonPosition.y);
        communicationButton->setFont(&panel.guiTTFFont);
        communicationButton->setBackgroundColor(180,87,128,75);
        communicationButton->setBackgroundSelectColor(180,87,128);
        buttonPosition.x += 100;
        
        guiTypeButton * dataButton = new guiTypeButton();
        dataButton->setup("data", buttonWidth*1.5, buttonHeight);
        dataButton->setDimensions(40, 20);
        dataButton->setPosition(buttonPosition.x, buttonPosition.y);
        dataButton->setFont(&panel.guiTTFFont);
        dataButton->setBackgroundColor(178,101,0,75);
        dataButton->setBackgroundSelectColor(178,101,0);
        buttonPosition.x += 50;
        
        buttons["source"] = sourceButton;
        buttons["sensing"] = sensingButton;
        buttons["communication"] = communicationButton;
        buttons["data"] = dataButton;
        
        ofAddListener(sourceButton->buttonPressed, this, &GuiManager::enableGroup);
        ofAddListener(sensingButton->buttonPressed, this, &GuiManager::enableGroup);
        ofAddListener(communicationButton->buttonPressed, this, &GuiManager::enableGroup);
        ofAddListener(dataButton->buttonPressed, this, &GuiManager::enableGroup);
        
        // setup min + max button
        
        minimizeButton = new guiTypeButton();
        minimizeButton->setup("minimize", buttonWidth*1.5, buttonHeight);
        minimizeButton->setPosition(10,10);
        minimizeButton->setFont(&panel.guiTTFFont);
        minimizeButton->setBackgroundColor(205,198,63);
        minimizeButton->setBackgroundSelectColor(205,198,63);
        
        maximizeButton = new guiTypeButton();
        maximizeButton->setup("maximize", buttonWidth*1.5, buttonHeight);
        maximizeButton->setPosition(10,10);
        maximizeButton->disable();
        maximizeButton->setFont(&panel.guiTTFFont);
        maximizeButton->setBackgroundSelectColor(0,168,156);
        maximizeButton->setBackgroundColor(0,168,156);
        
        buttons["minimize"] = (minimizeButton);
        buttons["maximize"] = (maximizeButton);
        
        ofAddListener(minimizeButton->buttonPressed, this, &GuiManager::minimize);
        ofAddListener(maximizeButton->buttonPressed, this, &GuiManager::maximize);
        
        // setup source panel
        
        guiTypePanel * videoPanel = panel.addPanel("video", 1, false);
        videoPanel->setDrawLock( false );	
        videoPanel->setBackgroundColor(174,139,138);
        videoPanel->setBackgroundSelectColor(174,139,138);
        
        panelGroups["source"].push_back( videoPanel );
        
        // video settings
        panel.setWhichPanel("video");
        guiTypeGroup * videoSettingsGroup = panel.addGroup("camera settings");
        videoSettingsGroup->setBackgroundColor(148,129,85);
        videoSettingsGroup->setBackgroundSelectColor(148,129,85);
        videoSettingsGroup->seBaseColor(244,136,136);
        videoSettingsGroup->setShowText(false);
#ifndef OF_VIDEO_CAPTURE_QTKIT
        panel.addButton("open video settings");
#endif
        vector<string>source_types;
        ofVideoGrabber dummyGrabber;
        vector<ofVideoDevice>	devices = dummyGrabber.listDevices();
        for ( int i=0; i<devices.size(); i++){
            source_types.push_back(devices[i].deviceName);
            SourceSelection sel;
            sel.type = CAMERA_VIDEOGRABBER;
            sel.index = i;
            currentSources[currentSources.size()] = sel;
        }
        
#ifndef TSPS_ONLY_OPENNI
#ifdef TSPS_KINECT2
        Kinect2 dummyKinect;
#else
        Kinect dummyKinect;
#endif
        int numKinects = dummyKinect.numAvailable();
        
        for ( int i=0; i<numKinects; i++){
            source_types.push_back("Kinect "+ofToString(i));
            
            SourceSelection sel;
            sel.type = CAMERA_KINECT;
            sel.index = i;
            currentSources[currentSources.size()] = sel;
        }
        
        numKinects = 1;
        
        for ( int i=0; i<numKinects; i++){
            source_types.push_back("Kinect 2 "+ofToString(i));
            
            SourceSelection sel;
            sel.type = CAMERA_KINECT2;
            sel.index = i;
            currentSources[currentSources.size()] = sel;
        }
#endif
        
#ifndef TSPS_KINECT2
        static OpenNI2 dummyOpenNI;
        /*int numOpenNI = dummyOpenNI.numAvailable();
        
        for ( int i=0; i<numOpenNI; i++){
            source_types.push_back("OpenNI2 "+ofToString(i));
            
            SourceSelection sel;
            sel.type = CAMERA_OPENNI;
            sel.index = i;
            currentSources[currentSources.size()] = sel;
        }*/
#endif
        
        SourceSelection videofileSelection;
        videofileSelection.type = CAMERA_VIDEOFILE;
        videofileSelection.index = 0;
        currentSources[currentSources.size()] = videofileSelection;
        source_types.push_back("Video File");
        
#ifdef TARGET_OSX
        SourceSelection syphonSelection;
        syphonSelection.type = CAMERA_SYPHON;
        syphonSelection.index = 0;
        currentSources[currentSources.size()] = syphonSelection;
        source_types.push_back("Syphon");
#endif
        SourceSelection customSelection;
        customSelection.type = CAMERA_SYPHON;
        customSelection.index = 0;
        currentSources[currentSources.size()] = customSelection;
        source_types.push_back("custom");
        
        panel.addMultiToggle("source type", "SOURCE_TYPE", 0, source_types);
        
        // video files
        guiTypeGroup * videoFilesGroup = panel.addGroup("videoFiles");
        videoFilesGroup->setBackgroundColor(148,129,85);
        videoFilesGroup->setBackgroundSelectColor(148,129,85);
        videoFilesGroup->seBaseColor(58,187,147);
        videoFilesGroup->setShowText(false);
        
        panel.addTextField("video directory (inside data folder)", "VIDEO_FILE_DIR", "videos", 200, 20);
        videoFiles = new simpleFileLister();
        int numVideoFiles = videoFiles->listDir(ofToDataPath("videos", true));
        ofLogVerbose()<< "num video files found: " << numVideoFiles;
        panel.addFileLister("video files:", videoFiles, 240, 100);
        panel.addToggle("reload directory", "VIDEO_FILE_RELOAD", true);
        
        // flip + invert
        guiTypeGroup * adjustGroup = panel.addGroup("adjust camera");
        adjustGroup->setBackgroundColor(148,129,85);
        adjustGroup->setBackgroundSelectColor(148,129,85);
        adjustGroup->seBaseColor(244,136,136);
        adjustGroup->setShowText(false);
        panel.addToggle("flip horizontal", "FLIP_X", false);
        panel.addToggle("flip vertical", "FLIP_Y", false);
        panel.addToggle("invert", "INVERT", false);
        
        // amplification
        guiTypeGroup * amplificationGroup = panel.addGroup("amplification");
        amplificationGroup->setBackgroundColor(148,129,85);
        amplificationGroup->setBackgroundSelectColor(148,129,85);
        amplificationGroup->seBaseColor(244,136,136);
        amplificationGroup->setShowText(false);
        panel.addToggle("use amplification (video gain)", "USE_AMPLIFICATION", false);
        panel.addSlider("amplification amount:", "AMPLIFICATION_AMOUNT", 1, 1, 100, true);
        
//#ifdef TSPS_KINECT2
        guiTypeGroup * k2Group = panel.addGroup("kinect settings");
        k2Group->setBackgroundColor(148,129,85);
        k2Group->setBackgroundSelectColor(148,129,85);
        k2Group->seBaseColor(244,136,136);
        k2Group->setShowText(false);

        panel.addSlider("near threshold:", "K2_NEAR", 30., 0., 12000, false);
        panel.addSlider("far threshold:", "K2_FAR", 1000., 0., 12000, false);
//#endif

        // end setup source panel
        
        // setup sensing panel
        
        guiTypePanel * backgroundPanel = panel.addPanel("background", 1, false);
        backgroundPanel->setDrawLock( false );	
        backgroundPanel->setBackgroundColor(213,105,68);
        backgroundPanel->setBackgroundSelectColor(213,105,68);
        
        guiTypePanel * differencingPanel = panel.addPanel("differencing", 1, false);
        differencingPanel->setDrawLock( false );
        differencingPanel->setBackgroundColor(113,171,154);
        differencingPanel->setBackgroundSelectColor(113,171,154);
        
        guiTypePanel * sensingPanel = panel.addPanel("tracking", 1, false);
        sensingPanel->setDrawLock( false );
        sensingPanel->setBackgroundColor(191,120,0);
        sensingPanel->setBackgroundSelectColor(191,120,0);
        
        panelGroups["sensing"].push_back( backgroundPanel );
        panelGroups["sensing"].push_back( differencingPanel );
        panelGroups["sensing"].push_back( sensingPanel );
        
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
        panel.addToggle("blank out background", "BLACK_BACKGROUND", false);
        panel.addToggle("store background across sessions", "STORE_BG", false);
        panel.addToggle("capture background on startup", "LEARN_BG_STARTUP", false);
        panel.addSlider("capture X seconds after startup", "LEARN_BG_STARTUP_SECONDS", 1.0f, 0.0f, 10.0f, false);
        
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
        panel.addSlider("threshold:", "THRESHOLD", 100, 1, 255, false);
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
        panel.addToggle("use highpass", "USE_HIGHPASS", false);
        panel.addSlider("highpass pre-blur:", "HIGHPASS_BLUR", 1.0, 0.0f, 15.0f, false);
        panel.addSlider("highpass kernel size", "HIGHPASS_KERNEL", 1, 1, 7, true);
        panel.addSlider("highpass post-blur:", "HIGHPASS_NOISE", 1.0, 0.0f, 15.0f, false);
        
        guiTypeGroup * smoothingGroup = panel.addGroup("smoothing");
        smoothingGroup->setBackgroundColor(148,129,85);
        smoothingGroup->setBackgroundSelectColor(148,129,85);
        smoothingGroup->seBaseColor(34,151,210);
        smoothingGroup->setShowText(false);
        panel.addToggle("use shape smoothing", "USE_SMOOTHING", false);
        panel.addSlider("smooth amount:", "SMOOTH_AMOUNT", 0, 0, 15, false);
        
        //sensing
        
        panel.setWhichPanel("tracking");
        //TODO optionally disable people?
        
        guiTypeGroup * blobGroup = panel.addGroup("blobs");
        blobGroup->setBackgroundColor(148,129,85);
        blobGroup->setBackgroundSelectColor(148,129,85);
        blobGroup->seBaseColor(238,218,0);
        blobGroup->setShowText(false);
        blobGroup->setActive(true);
        
        panel.addSlider("minimum blob size (% of view):", "MIN_BLOB", 1.f, 0.01f, 50.0f, false);
        panel.addSlider("maximum blob size (% of view):", "MAX_BLOB", 100.f, 0.1f, 100.f, false);
        panel.addToggle("ignore nested blobs", "FIND_HOLES", false);
        
        haarGroup = panel.addGroup("haar tracking");
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
        ofLogVerbose()<< "haar files found " << numHaar;
        panel.addFileLister("types of features:", haarFiles, 240, 100);
        panel.addSlider("expand detection area:", "HAAR_PADDING", 0.0f, 0.0f, 200.0f, false);
        
        // end setup sensing panel
        
        // setup communication panel
        
        guiTypePanel * oscPanel = panel.addPanel("OSC", 1, false);
        oscPanel->setDrawLock( false );
        oscPanel->setBackgroundColor(180,87,128);
        oscPanel->setBackgroundSelectColor(180,87,128);
        
        guiTypePanel * websocketPanel = panel.addPanel("WebSockets", 1, false);
        websocketPanel->setDrawLock( false );
        websocketPanel->setBackgroundColor(180,87,128);
        websocketPanel->setBackgroundSelectColor(180,87,128);
        
        guiTypePanel * spacebrewPanel = panel.addPanel("Spacebrew", 1, false);
        spacebrewPanel->setDrawLock( false );
        spacebrewPanel->setBackgroundColor(180,87,128);
        spacebrewPanel->setBackgroundSelectColor(180,87,128);
        
        guiTypePanel * tuioPanel = panel.addPanel("TUIO", 1, false);
        tuioPanel->setDrawLock( false );
        tuioPanel->setBackgroundColor(180,87,128);
        tuioPanel->setBackgroundSelectColor(180,87,128);
        
        guiTypePanel * tcpPanel = panel.addPanel("TCP", 1, false);
        tcpPanel->setDrawLock( false );
        tcpPanel->setBackgroundColor(180,87,128);
        tcpPanel->setBackgroundSelectColor(180,87,128);
        
        
        panelGroups["communication"].push_back( oscPanel );
        panelGroups["communication"].push_back( websocketPanel );
        panelGroups["communication"].push_back( spacebrewPanel );
        panelGroups["communication"].push_back( tuioPanel );
        panelGroups["communication"].push_back( tcpPanel );
        
        //communication
        
        panel.setWhichPanel("OSC");
        
        guiTypeGroup * oscGroup = panel.addGroup("OSC");
        oscGroup->setBackgroundColor(148,129,85);
        oscGroup->setBackgroundSelectColor(148,129,85);
        oscGroup->seBaseColor(238,53,35);
        oscGroup->setShowText(false);
        panel.addToggle("send OSC", "SEND_OSC", false);
        panel.addTextField("receiver IP address (OSC host) :", "OSC_HOST", "127.0.0.1", 200, 20);
        panel.addTextField("receiver port (OSC port) :", "OSC_PORT", "12000", 200, 20);
        panel.addToggle("use legacy OSC", "LEGACY_OSC", false);
        
        panel.setWhichPanel("TUIO");
        
        guiTypeGroup * tuioGroup = panel.addGroup("TUIO");
        tuioGroup->setBackgroundColor(148,129,85);
        tuioGroup->setBackgroundSelectColor(148,129,85);
        tuioGroup->seBaseColor(207,202,65);
        tuioGroup->setShowText(false);
        panel.addToggle("send TUIO", "SEND_TUIO", false);
        panel.addTextField("receiver IP address (TUIO host):", "TUIO_HOST", "127.0.0.1", 200, 20);
        panel.addTextField("receiver port (TUIO port):", "TUIO_PORT", "3333", 200, 20);
        
        panel.setWhichPanel("TCP");
        
        guiTypeGroup * tcpGroup = panel.addGroup("TCP");
        tcpGroup->setBackgroundColor(148,129,85);
        tcpGroup->setBackgroundSelectColor(148,129,85);
        tcpGroup->seBaseColor(238,53,35);
        tcpGroup->setShowText(false);
        panel.addToggle("send TCP", "SEND_TCP", false);
        panel.addTextField("broadcast port (TCP port):", "TCP_PORT", "8888", 200, 20);
        
        panel.setWhichPanel("WebSockets");
        
        guiTypeGroup * wssGroup = panel.addGroup("WebSocket Server");
        wssGroup->setBackgroundColor(148,129,85);
        wssGroup->setBackgroundSelectColor(148,129,85);
        wssGroup->seBaseColor(238,53,35);
        panel.addToggle("send over WebSocket server", "SEND_WSS", false);
        panel.addTextField("webSocket port:", "WSS_PORT", "7681", 200, 20);
        
#if defined(_MSC_VER) || defined(_WIN32) || defined(WIN32) || defined(__MINGW32__)
        // this doesn't work on libwebsockets for windows.
#else
		panel.addButton("open debug URL");
        
#endif
        
        guiTypeGroup * wsGroup = panel.addGroup("WebSocket Client");
        wsGroup->setBackgroundColor(148,129,85);
        wsGroup->setBackgroundSelectColor(148,129,85);
        wsGroup->seBaseColor(238,53,35);
        panel.addToggle("send over WebSocket client", "SEND_WS", false);
        panel.addTextField("webSocket host:", "WS_HOST", "localhost", 200, 20);
        panel.addTextField("webSocket port:", "WS_PORT", "7681", 200, 20);
        panel.addToggle("use SSL", "WS_USESSL", false);
        panel.addTextField("webSocket channel (optional):", "WS_CHANNEL", "", 200, 20);
        
        panel.setValueB("SEND_WSS", false);
        panel.setValueB("SEND_WS", false);
        
        panel.setWhichPanel("Spacebrew");
        
        guiTypeGroup * sbGroup = panel.addGroup("Spacebrew");
        sbGroup->setBackgroundColor(148,129,85);
        sbGroup->setBackgroundSelectColor(148,129,85);
        sbGroup->seBaseColor(238,53,35);
        panel.addToggle("Send to Spacebrew", "SEND_SB", false);
        panel.addTextField("Spacebrew host:", "SB_HOST", "localhost", 200, 20);
        
        // end setup comm panel
        
        // setup data panel
        
        guiTypePanel * peoplePanel = panel.addPanel("people", 1, false);
        peoplePanel->setDrawLock( false );
        peoplePanel->setBackgroundColor(178,101,0);
        peoplePanel->setBackgroundSelectColor(178,101,0);
        
        guiTypePanel * scenePanel = panel.addPanel("scene", 1, false);
        scenePanel->setDrawLock( false );
        scenePanel->setBackgroundColor(178,101,0);
        scenePanel->setBackgroundSelectColor(178,101,0);
        
        panelGroups["data"].push_back( peoplePanel );
        panelGroups["data"].push_back( scenePanel );
        
        panel.setWhichPanel("people");
        
        guiTypeGroup * optionsGroup = panel.addGroup("options");
        optionsGroup->setBackgroundColor(148,129,85);
        optionsGroup->setBackgroundSelectColor(148,129,85);
        optionsGroup->seBaseColor(58,187,147);
        optionsGroup->setShowText(false);
        
        panel.addToggle("track and send contours", "SEND_OSC_CONTOURS", false);
        
        opticalFlowGroup = panel.addGroup("optical flow");
        opticalFlowGroup->setBackgroundColor(148,129,85);
        opticalFlowGroup->setBackgroundSelectColor(148,129,85);
        opticalFlowGroup->seBaseColor(34,151,210);
        opticalFlowGroup->setShowText(false);
        //optical flow
        // To-do: implement these in processors + ofxCv
        //panel.addSlider("filter vectors smaller than:", "MIN_OPTICAL_FLOW", 0, 0, 5.0, false);
        //panel.addSlider("clamp vectors: larger than", "MAX_OPTICAL_FLOW", 10, 5.0, 200, false);
        panel.addToggle("track and send optical flow in blobs", "SENSE_OPTICAL_FLOW", false);
        
        panel.setWhichPanel("scene");
        
        guiTypeGroup * sceneGroup = panel.addGroup("options");
        sceneGroup->setBackgroundColor(148,129,85);
        sceneGroup->setBackgroundSelectColor(148,129,85);
        sceneGroup->seBaseColor(58,187,147);
        sceneGroup->setShowText(false);
        
        panel.addToggle("send scene data", "SEND_SCENE_DATA", settings.bSendScene );
        panel.addSlider("scene grid horizontal", "SCENE_GRID_X", 0, 0, 10, true);
        panel.addSlider("scene grid vertical", "SCENE_GRID_Y", 0, 0, 10, true);
        
        // end setup data panel
        
        //listen for save / load / reload / saveas events from GUI + pass to quadgui
        
        ofAddListener(panel.getSaveButton()->buttonPressed, this, &GuiManager::saveEventCatcher);
        ofAddListener(panel.getRestoreButton()->buttonPressed, this, &GuiManager::reloadEventCatcher);
        ofAddListener(panel.getLoadButton()->buttonPressed, this, &GuiManager::loadEventCatcher);
        ofAddListener(panel.getSaveAsButton()->buttonPressed, this, &GuiManager::saveAsEventCatcher);
        
        bHasCustomPanel = false;
        
        ofEventArgs nullArgs;
        update(nullArgs);
        enableEvents();
        
        // set active group
        enableGroup("sensing");
        panel.setSelectedPanel("differencing");
    }
    
    
    void GuiManager::refreshSourcePanel(){
        // silent for now..
    }
    
    map<int,SourceSelection> * GuiManager::getCurrentSources(){
        return &currentSources;
    }
    
    int GuiManager::getSourceSelectionIndex( SourceType type, int deviceIndex){
        map<int,SourceSelection>::iterator it = currentSources.begin();
        for ( it; it != currentSources.end(); it++){
            if ( it->second.type == type && it->second.index == deviceIndex ){
                return it->first;
            }
        }
        return -1;
    }
    
    void GuiManager::addCustomGui(){
        if ( bHasCustomPanel ) return;
        customPanelCount = 0;
        customItemCount = 0;
        
        currentCustomPanel = "custom_"+ofToString(customPanelCount);
        guiTypePanel * customPanel = panel.addPanel(currentCustomPanel, 1, false);
        customPanel->setDrawLock( false );
        customPanel->setBackgroundColor(11,119,108);
        customPanel->setBackgroundSelectColor(11,119,108);
        bHasCustomPanel = true;
        
        panelGroups["custom"].push_back( customPanel );
        
        if ( enabledPanelGroup != "custom"){
            disableGroup("custom");
        }
        
        panel.setWhichPanel(currentCustomPanel);
        guiTypeGroup * customSettingsGroup = panel.addGroup("custom settings");
        customSettingsGroup->setBackgroundColor(148,129,85);
        customSettingsGroup->setBackgroundSelectColor(148,129,85);
        customSettingsGroup->seBaseColor(58,187,147);
        customSettingsGroup->setShowText(false);
        
        guiTypeButton * customButton = new guiTypeButton();
        customButton->setup("custom", buttonWidth*1.5, buttonHeight);
        customButton->setDimensions(50, 20);
        customButton->setPosition( buttonPosition.x, buttonPosition.y );
        customButton->setFont(&panel.guiTTFFont);
        customButton->setBackgroundColor(11,119,108,75);
        customButton->setBackgroundSelectColor(11,119,108);
        buttons["custom"] = (customButton);
        
        ofAddListener(customButton->buttonPressed, this, &GuiManager::enableGroup);
    }
    
    void GuiManager::enableGroup( string & groupName ){
        enableGroup(groupName.c_str());
    }
    
    void GuiManager::enableGroup( string groupName ){
        if ( panelGroups.count( groupName ) == 0 ){
            ofLogWarning("No group of name "+groupName );
            return;
        }
        
        map<string, vector<guiTypePanel*> >::iterator it = panelGroups.begin();
        for (it; it != panelGroups.end(); it++){
            disableGroup( it->first );
        }
        
        if (buttons.count(groupName)) buttons[ groupName ]->setSelected();
        enabledPanelGroup = groupName;
        
        for (int i=0; i<panelGroups[groupName].size(); i++){
            panelGroups[groupName][i]->enable();
        }
        if ( panelGroups[groupName].size() == 0) return;
        panel.setSelectedPanel(panelGroups[groupName][0]->name);
    }
    
    void GuiManager::disableGroup( string groupName ){
        if ( panelGroups.count( groupName ) == 0 ){
            ofLogWarning("No group of name "+groupName );
            return;
        }
        
        if (buttons.count(groupName)) buttons[ groupName ]->setNormal();
        
        for (int i=0; i<panelGroups[groupName].size(); i++){
            panelGroups[groupName][i]->disable();
        }
    }
    
    vector<string> GuiManager::getGroupNames(){
        vector<string> ret;
        map<string, vector<guiTypePanel*> >::iterator it = panelGroups.begin();
        for (it; it != panelGroups.end(); it++){
            ret.push_back( it->first );
        }
        return ret;
    }
    
    void GuiManager::addSlider(string name, int* value, int min, int max){
        addCustomGui();
        GUICustomParam p;
        string key = "CUSTOM" + ofToString(params.size());
        
        panel.setWhichPanel(currentCustomPanel);
        panel.setWhichColumn(0);	
        panel.addSlider(name, key, *value, min, max, true);
        
        p.key = key;
        p.type = PARAM_INT;
        p.i = value;
        params.push_back(p);
        panel.loadSettings( panel.getCurrentXMLFile() );
        
        // do we need to add another panel?
        customItemCount++;
        
        if ( customItemCount > 8 ){
            customItemCount = 0;
            customPanelCount++;
            currentCustomPanel = "custom_"+ofToString(customPanelCount);
            guiTypePanel * customPanel = panel.addPanel(currentCustomPanel, 1, false);
            customPanel->setDrawLock( false );
            customPanel->setBackgroundColor(11,119,108);
            customPanel->setBackgroundSelectColor(11,119,108);
            bHasCustomPanel = true;
            
            panelGroups["custom"].push_back( customPanel );
            
            if ( enabledPanelGroup != "custom"){
                disableGroup("custom");
            }
            
            panel.setWhichPanel(currentCustomPanel);
            guiTypeGroup * customSettingsGroup = panel.addGroup("custom settings");
            customSettingsGroup->setBackgroundColor(148,129,85);
            customSettingsGroup->setBackgroundSelectColor(148,129,85);
            customSettingsGroup->seBaseColor(58,187,147);
            customSettingsGroup->setShowText(false);
        }
    }
    
    void GuiManager::addSlider(string name, float* value, float min, float max)
    {
        addCustomGui();
        GUICustomParam p;
        string key = "CUSTOM" + ofToString(params.size());	
        panel.setWhichPanel(currentCustomPanel);
        panel.setWhichColumn(0);
        panel.addSlider(name, key, *value, min, max, false);
        
        p.key = key;
        p.type = PARAM_FLOAT;
        p.f = value;
        params.push_back(p);
        
        // do we need to add another panel?
        customItemCount++;
        
        if ( customItemCount > 8 ){
            customItemCount = 0;
            customPanelCount++;
            currentCustomPanel = "custom_"+ofToString(customPanelCount);
            guiTypePanel * customPanel = panel.addPanel(currentCustomPanel, 1, false);
            customPanel->setDrawLock( false );
            customPanel->setBackgroundColor(11,119,108);
            customPanel->setBackgroundSelectColor(11,119,108);
            bHasCustomPanel = true;
            
            panelGroups["custom"].push_back( customPanel );
            
            if ( enabledPanelGroup != "custom"){
                disableGroup("custom");
            }
            
            panel.setWhichPanel(currentCustomPanel);
            guiTypeGroup * customSettingsGroup = panel.addGroup("custom settings");
            customSettingsGroup->setBackgroundColor(148,129,85);
            customSettingsGroup->setBackgroundSelectColor(148,129,85);
            customSettingsGroup->seBaseColor(58,187,147);
            customSettingsGroup->setShowText(false);
        }
    }
    
    void GuiManager::addToggle(string name, bool* value)
    {
        addCustomGui();
        GUICustomParam p;	
        string key = "CUSTOM" + ofToString(params.size());
        
        panel.setWhichPanel(currentCustomPanel);
        panel.setWhichColumn(0);
        panel.addToggle(name, key, *value);
        
        p.key = key;
        p.type = PARAM_BOOL;
        p.b = value;
        params.push_back(p);
        
        // do we need to add another panel?
        customItemCount++;
        
        if ( customItemCount > 8 ){
            customItemCount = 0;
            customPanelCount++;
            currentCustomPanel = "custom_"+ofToString(customPanelCount);
            guiTypePanel * customPanel = panel.addPanel(currentCustomPanel, 1, false);
            customPanel->setDrawLock( false );
            customPanel->setBackgroundColor(11,119,108);
            customPanel->setBackgroundSelectColor(11,119,108);
            bHasCustomPanel = true;
            
            panelGroups["custom"].push_back( customPanel );
            
            if ( enabledPanelGroup != "custom"){
                disableGroup("custom");
            }
            
            panel.setWhichPanel(currentCustomPanel);
            guiTypeGroup * customSettingsGroup = panel.addGroup("custom settings");
            customSettingsGroup->setBackgroundColor(148,129,85);
            customSettingsGroup->setBackgroundSelectColor(148,129,85);
            customSettingsGroup->seBaseColor(58,187,147);
            customSettingsGroup->setShowText(false);
        }
    }
    
    /***************************************************************
     GET + SET PANEL INFO / SELECTED PANEL
     ***************************************************************/
    
    void GuiManager::setHaarEnabled( bool bHaar ){
        if ( bHaar ){
            haarGroup->enable();         
        } else {
            haarGroup->disable();        
        }    
    }
    
    void GuiManager::setOpticalFlowEnabled( bool bOpticalFlow ){
        if ( bOpticalFlow ){
            opticalFlowGroup->enable();         
        } else {
            opticalFlowGroup->disable();        
        }
    }
    
    
    /***************************************************************
     GET + SET PANEL INFO / SELECTED PANEL
     ***************************************************************/
    
    
    void GuiManager::addPanelToGroup( string groupName, guiTypePanel * panel ){
        if ( panelGroups.count(groupName) == 0 ){
            ofLogWarning()<<"No group with name "<<groupName<<", adding it!";
        }
        panelGroups[groupName].push_back( panel );
    }
    
    int GuiManager::getSelectedPanel(){
        return panel.getSelectedPanelIndex();
    }
    
    void GuiManager::setSelectedPanel( int index ){
        panel.setWhichPanel(index);
    }
    
    void GuiManager::setSelectedPanel( string name ){
        panel.setWhichPanel(name);
    }
    
    void GuiManager::update(ofEventArgs &e){
        update();
    }
    
    void GuiManager::update(){
        if(!enableGui){
            //if the gui is not shown no need to propagate values
            return;
        }
        
        panel.update();
        
        // camera
        
        //settings.cameraIndex = panel.getValueF("CAMERA_INDEX");
        //settings.bUseKinect  = panel.getValueB("USE_KINECT");
		int index = panel.getValueI("SOURCE_TYPE");
        
        settings.inputType      = currentSources[index].type;
        settings.cameraIndex    = currentSources[index].index;
        
        if ( settings.inputType == CAMERA_VIDEOFILE){
            panel.getElement("videoFiles")->enable();
        } else {
            panel.getElement("videoFiles")->disable();
        }
        
        // video files
        bool bIsNewDirectory = false;
        if ( settings.videoDirectory != panel.getValueS("VIDEO_FILE_DIR") ){
            settings.videoDirectory = panel.getValueS("VIDEO_FILE_DIR");    
            bIsNewDirectory = true;
        }
        if ( panel.getValueB("VIDEO_FILE_RELOAD") || bIsNewDirectory){
            int numVideos = 0;
            if ( bIsNewDirectory ) numVideos = videoFiles->listDir(ofToDataPath(settings.videoDirectory, true));
            else numVideos = videoFiles->refreshDir();
            panel.setValueB("VIDEO_FILE_RELOAD", false);
            
            if ( numVideos == 0 ){     
                ofLog( OF_LOG_VERBOSE, "No videos found, switching to camera input" );
            }
        }
        if(videoFiles->getSelectedName() != ""){
            settings.videoFile = videoFiles->getSelectedPath();
        }
        
        // threshold
        
        settings.threshold = panel.getValueF("THRESHOLD");
        settings.bSmooth = panel.getValueB("USE_SMOOTHING");
        settings.smooth = panel.getValueF("SMOOTH_AMOUNT");
        panel.setGroupActive("differencing", "smoothing", settings.bSmooth);
        
        settings.bHighpass = panel.getValueI("USE_HIGHPASS");
        settings.highpassBlur =  panel.getValueF("HIGHPASS_BLUR");
        settings.highpassKernel = panel.getValueI("HIGHPASS_KERNEL");
        settings.highpassNoise = panel.getValueF("HIGHPASS_NOISE");
        panel.setGroupActive("differencing", "highpass", settings.bHighpass);
        
        settings.bFlipX = panel.getValueB("FLIP_X");
        settings.bFlipY = panel.getValueB("FLIP_Y");
        settings.bInvert = panel.getValueB("INVERT");
        
        settings.bAmplify = panel.getValueB("USE_AMPLIFICATION");
        settings.highpassAmp = panel.getValueI("AMPLIFICATION_AMOUNT");
        panel.setGroupActive("video", "amplification", settings.bAmplify);
        
#ifndef OF_VIDEO_CAPTURE_QTKIT
        if(panel.getButtonPressed("open video settings") && settings.getSource() != NULL && settings.getInputType() == CAMERA_VIDEOGRABBER){
            ofVideoGrabber * grab = dynamic_cast<ofVideoGrabber *>(settings.getSource());
            grab->videoSettings();
        }
        
        if ( settings.getInputType() == CAMERA_VIDEOGRABBER ){
            panel.getElement("open video settings")->enable();
        } else {
            panel.getElement("open video settings")->disable();
        }
#endif
        
		#if defined(_MSC_VER) || defined(_WIN32) || defined(WIN32) || defined(__MINGW32__)
			// no websever on windows
#else
		if (panel.getButtonPressed("open debug URL")){
            ofLaunchBrowser( "http://localhost:"+panel.getValueS("WSS_PORT"));
        }
#endif
        
        
        //settings.bAdjustedViewInColor = panel.getValueB("ADJUSTED_VIEW_COLOR");
        //panel.setGroupActive("video", "adjustedViewColor", settings.bAdjustedViewInColor);
        
        settings.bLearnBackgroundOnStartup  = panel.getValueB("LEARN_BG_STARTUP");
        settings.captureSeconds             = panel.getValueF("LEARN_BG_STARTUP_SECONDS");
        
        settings.bLearnBackground = panel.getValueB("LEARN_BACKGROUND");
        if(settings.bLearnBackground == bLastBgState){
            panel.setValueB("LEARN_BACKGROUND", false);
        }
        bLastBgState = settings.bLearnBackground;
        
        settings.bBlankBackground = panel.getValueB("BLACK_BACKGROUND");
        if(settings.bBlankBackground){ 
            panel.setValueB("BLACK_BACKGROUND", false);
        }

        settings.bStoreBackground = panel.getValueB("STORE_BG");
        
        //panel.setValueB("LEARN_BACKGROUND", settings.bLearnBackground);
        //JG 12/8/09 GUI-REDUX Removing this feature
        //gui.addToggle("smart learn background", &settings.bSmartLearnBackground);
        
        settings.minBlob = panel.getValueF("MIN_BLOB")/100.0f; //scale var to be right for tracker
        settings.maxBlob = panel.getValueF("MAX_BLOB")/100.0f;	//scale var to be right for tracker
        settings.bLearnBackgroundProgressive = panel.getValueB("RELEARN");
        settings.fLearnRate = panel.getValueF("RELEARN_BACKGROUND");
        panel.setGroupActive("background", "background relearn", settings.bLearnBackgroundProgressive);
        
        settings.bFindHoles = !(panel.getValueB("FIND_HOLES"));
        settings.bTrackOpticalFlow = panel.getValueB("SENSE_OPTICAL_FLOW");
        panel.setGroupActive("sensing", "optical flow", settings.bTrackOpticalFlow);
        
        //JG 12/8/09 GUI-REDUX:
        //settings.minOpticalFlow = panel.getValueF("MIN_OPTICAL_FLOW");
        //settings.maxOpticalFlow = panel.getValueF("MAX_OPTICAL_FLOW");
        settings.trackType = panel.getValueI("BLOB_TYPE");
        settings.bDetectHaar = panel.getValueB("SENSE_HAAR");
        panel.setGroupActive("sensing", "haar tracking", settings.bDetectHaar);
        
        if(haarFiles->getSelectedName() != ""){
            settings.haarFile = haarFiles->getSelectedName();
        }
        settings.haarAreaPadding = panel.getValueF("HAAR_PADDING");
        //JG GUI-REDUX: removing this feature
        //gui.addToggle("send haar center as blob center", &settings.bUseHaarAsCenter);
        //JG 1/21/10 disabled this feature to simplify the interface
        //	settings.minHaarArea = panel.getValueF("MIN_HAAR");
        //	settings.maxHaarArea = panel.getValueF("MAX_HAAR");
        
        //update scene
        settings.bSendScene = panel.getValueB("SEND_SCENE_DATA");
        settings.sceneGridX = panel.getValueI("SCENE_GRID_X");
        settings.sceneGridY = panel.getValueI("SCENE_GRID_Y");
        
        //update osc stuff
        settings.bSendOsc = panel.getValueB("SEND_OSC");
        settings.bSendTuio = panel.getValueB("SEND_TUIO");
        settings.bSendTcp = panel.getValueB("SEND_TCP");
        settings.bSendWebSocketServer = panel.getValueB("SEND_WSS");
        settings.bSendWebSocketClient = panel.getValueB("SEND_WS");
        
        settings.oscHost = panel.getValueS("OSC_HOST", 0, "localhost");
        settings.oscPort = (int) atoi(panel.getValueS("OSC_PORT", 0, "12000").c_str());
        settings.bUseLegacyOsc = panel.getValueB("LEGACY_OSC");
        
        settings.tuioHost = panel.getValueS("TUIO_HOST", 0, "localhost");
        settings.tuioPort = (int) atoi(panel.getValueS("TUIO_PORT", 0, "3333").c_str());
        
        settings.tcpPort = (int) atoi(panel.getValueS("TCP_PORT", 0, "8888").c_str());
        
        settings.spacebrewHost = panel.getValueS("SB_HOST", 0, "sandbox.spacebrew.cc");
        settings.bSendSpacebrew = panel.getValueB("SEND_SB");
        
        settings.webSocketServerPort = (int) atoi(panel.getValueS("WSS_PORT", 0, "7681").c_str());
        settings.webSocketPort = (int) atoi(panel.getValueS("WS_PORT", 0, "7681").c_str());
        settings.webSocketHost = panel.getValueS("WS_HOST", 0, "localhost");
        settings.webSocketUseSSL = panel.getValueB("WS_USESSL");
        settings.webSocketChannel = panel.getValueS("WS_CHANNEL", 0, "localhost");
        
        settings.bSendOscContours = panel.getValueB("SEND_OSC_CONTOURS");
        panel.setGroupActive("sensing", "options", settings.bSendOscContours);
        
        panel.setGroupActive("communication", "OSC", settings.bSendOsc);
        panel.setGroupActive("communication", "TUIO", settings.bSendTuio);
        panel.setGroupActive("communication", "TCP", settings.bSendTcp);
        
        // UPDATE GUI QUADS HERE
        // because this returns a pointer to the actual points that get updated,
        // you store it in an array so it doesn't get updated when it draws
        ofPoint * scaledPoints = quadGui.getScaledQuadPoints(cameraWidth,cameraHeight);
        for (int i=0; i<4; i++){
            settings.quadWarpScaled[i] = scaledPoints[i];
        }
        
        //modify custom parameters
        vector<GUICustomParam>::iterator it;
        for(it = params.begin(); it != params.end(); it++){
            GUICustomParam p = *it;
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
                    printf("GUIManager: WARNING undefined parameter type encountered\n");
                    break;
            }
        }
        
        //get xml
        settings.currentXmlFile = panel.getCurrentXMLFile();
        
        //get current panel
        //settings.lastCurrentPanel = settings.currentPanel;
        settings.currentPanel = panel.getSelectedPanelIndex();
    }
    
    //BR: Added some messiness here to setup, draw, and update the gui quad...
    
    void GuiManager::setupQuadGui ( int _cameraWidth, int _cameraHeight )
    {
        cameraWidth = _cameraWidth;
        cameraHeight = _cameraHeight;
        
        // give the gui quad a starting setting
        
        quadGui.setup("QUAD");
        ofPoint quadSrc[4];
        quadSrc[0].set(0, 0);
        quadSrc[1].set(cameraWidth, 0);
        quadSrc[2].set(cameraWidth, cameraHeight);
        quadSrc[3].set(0, cameraHeight);
        quadGui.setQuadPoints(quadSrc);
        
        // give the gui quad a default setting
        settings.quadWarpOriginal[0].set(0, 0);
        settings.quadWarpOriginal[1].set(cameraWidth, 0);
        settings.quadWarpOriginal[2].set(cameraWidth, cameraHeight);
        settings.quadWarpOriginal[3].set(0, cameraHeight);
        
        //BR TO DO: add this into the normal settings file
        quadGui.width = cameraWidth;
        quadGui.height = cameraHeight;
        quadGui.disableAppEvents();
        
        quadGuiSetup = true;
    };
    
    void GuiManager::drawQuadGui(){
        quadGui.draw();
    };
    
    void GuiManager::drawQuadGui( int x, int y, int width, int height ){
        quadGui.x = x;
        quadGui.y = y;
        quadGui.setScale((float) width/quadGui.width, (float) height/quadGui.height );
        drawQuadGui();
    };
    
    
    // ZACK BOKA: Added so the quadGui instance can know when image warping is allowed to occur
    //            (i.e., the image can only get warped when in Camera View).
    void GuiManager::changeGuiCameraView(bool bCameraView) {
        quadGui.bCameraView = bCameraView;
    };
    
    /***************************************************************
     GET + SET SPECIFIC VALUES FROM THE GUI
     ***************************************************************/
    
    //----------------------------------------------------------
    bool GuiManager::getValueB( string name ){
        return panel.getValueB(name);
    }
    
    //----------------------------------------------------------
    int GuiManager::getValueI( string name ){
        return panel.getValueI(name);
    }
    
    //----------------------------------------------------------
    float GuiManager::getValueF( string name ){
        return panel.getValueF(name);    
    }
    
    //----------------------------------------------------------
    string GuiManager::getValueS( string name ){
        return panel.getValueS(name);    
    }
    
    //----------------------------------------------------------
    void GuiManager::setValueB( string name, bool val ){
        panel.setValueB(name, val);    
    }
    
    //----------------------------------------------------------
    void GuiManager::setValueI( string name, int val ){
        panel.setValueI(name, val);    
    }
    
    //----------------------------------------------------------
    void GuiManager::setValueF( string name, float val ){
        panel.setValueF(name, val);    
    }
    
    //----------------------------------------------------------
    void GuiManager::setValueS( string name, string val ){
        panel.setValueS(name, val);    
    }
    
    /***************************************************************
     ENABLE / DISABLE ELEMENTS
     ***************************************************************/
    
    //----------------------------------------------------------
    void GuiManager::enableElement( string name ){
        guiBaseObject * el = panel.getElement( name );
        if (el != NULL){
            el->enable();
        }
    };
    
    //----------------------------------------------------------
    void GuiManager::disableElement( string name ){
        guiBaseObject * el = panel.getElement( name );
        if (el != NULL){
            el->disable();
        }
    };
    
    /***************************************************************
     GET EVENTS FROM GUI BUTTONS
     ***************************************************************/
    
    void GuiManager::saveEventCatcher( string & buttonName){
        settings.currentXmlFile = panel.getCurrentXMLFile();
        quadGui.saveToFile(settings.currentXmlFile);
    };
    
    void GuiManager::reloadEventCatcher( string & buttonName){
        quadGui.readFromFile(settings.currentXmlFile);
    };
    
    void GuiManager::loadEventCatcher( string & buttonName){
        settings.currentXmlFile = panel.getCurrentXMLFile();
        quadGui.readFromFile(settings.currentXmlFile);
    };
    
    void GuiManager::saveAsEventCatcher( string & buttonName){
        settings.currentXmlFile = panel.getCurrentXMLFile();
        quadGui.readFromFile(settings.currentXmlFile);
    };
    
    /***************************************************************
     GET + SET PANEL INFO / SELECTED PANEL
     ***************************************************************/
    
    //forward to gui
    void GuiManager::mousePressed(ofMouseEventArgs &e)
    {
        if(enableGui) panel.mousePressed(e.x, e.y, e.button);
        if(quadGuiSetup){
        }
    }
    
    void GuiManager::mouseDragged(ofMouseEventArgs &e)
    {
        if(enableGui) panel.mouseDragged(e.x, e.y, e.button);
    }
    
    void GuiManager::mouseReleased(ofMouseEventArgs &e)
    {
        if(enableGui){
            panel.mouseReleased();
            map<string,guiTypeButton*>::iterator it = buttons.begin();
            
            for (it; it != buttons.end(); it++){
                if (it->second->enabled){
                    if ( it->second->checkHit(e.x, e.y, e.button) ){
                        return;
                    }
                }
            }
            
            for( it=customButtons.begin(); it!=customButtons.end(); it++ ){
                if ( it->second->enabled){
                    it->second->checkHit( e.x, e.y, e.button );                    
                }
            }
        }
    }
    
    void GuiManager::loadSettings( string xmlFile ){
        panel.loadSettings(xmlFile);	
        if (quadGuiSetup) quadGui.readFromFile(xmlFile);	
        update();
    };
    
    void GuiManager::keyPressed(ofKeyEventArgs &e)
    {
        if(enableGui) panel.keyPressed(e.key);
    };
    
    void GuiManager::keyReleased(ofKeyEventArgs &e)
    {
        if(enableGui) panel.keyReleased(e.key);
    };
    
    void GuiManager::draw() {
        if(enableGui){
            panel.draw();
            
            ofSetColor(184,169,121);
            ofFill();
            ofDrawRectangle(10,40,panel.getWidth(),20);
            ofSetColor(255);
            panel.guiTTFFont.drawString("CONFIGURE TSPS: "+ ofToUpper(enabledPanelGroup), 15, 45 + panel.guiTTFFont.getSize() );
            
            map<std::string, guiTypeButton*>::iterator it;
            for( it=buttons.begin(); it!=buttons.end(); it++ ){
                it->second->render();
            }
            
            // draw custom buttons
            for( it=customButtons.begin(); it!=customButtons.end(); it++ ){
                it->second->render();
            }
        }
    }
    
    /***************************************************************
     CUSTOM BUTTONS
     ***************************************************************/
    
    //----------------------------------------------------------
    guiTypeButton * GuiManager::addButton( string name, ofRectangle dimensions, ofColor color, ofColor bgcolor ){
        guiTypeButton * btn = new guiTypeButton();
        
        // if you didn't pass a rectangle, just use the default button settings
        if ( dimensions.x == dimensions.y == dimensions.width == dimensions.height){
            btn->setup( name, buttonWidth*1.5, buttonHeight);
            btn->setPosition(10,10);    
        } else {
            btn->setup( name, dimensions.width, dimensions.height);
            btn->setPosition(dimensions.x, dimensions.y);
        }
        btn->setFont(&panel.guiTTFFont);    
        
        if ( color != NULL){
            btn->setBackgroundSelectColor(color.r, color.g, color.b);        
        } else {
            btn->setBackgroundSelectColor(0,168,156);        
        }
        if ( bgcolor != NULL){
            btn->setBackgroundSelectColor(bgcolor.r, bgcolor.g, bgcolor.b);        
        } else {
            btn->setBackgroundColor(0,168,156); 
        }
        customButtons.insert( pair<std::string, guiTypeButton*>( name, btn) );
        return btn;
    }
    
    //----------------------------------------------------------
    guiTypeButton * GuiManager::getButton( string name ){
        if ( customButtons.find( name ) != customButtons.end() ){
            return customButtons[name];
        } else {
            return NULL;
        }    
    }
    
    /***************************************************************
     MINIMIZE + MAXIMIZE THE APP
     ***************************************************************/
    
    //----------------------------------------------------------
    void GuiManager::minimize(string & button){
        settings.bMinimized = true;
        ofSetWindowShape(100,40);
        minimizeButton->disable();
        maximizeButton->enable();
    };
    
    //----------------------------------------------------------
    void GuiManager::maximize(string & button){
        settings.bMinimized = false;  
        ofSetWindowShape(1024,768); 
        minimizeButton->enable();
        maximizeButton->disable();
    };
    
    
    /***************************************************************
     GET SETTINGS OBJECT (NO LONGER SINGLETON!)
     ***************************************************************/
    
    //----------------------------------------------------------
    Settings * GuiManager::getSettings(){
        return &settings;
    };
}
