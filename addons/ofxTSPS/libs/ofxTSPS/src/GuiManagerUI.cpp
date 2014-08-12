/*
 *  GuiManagerUI.cpp
 *  openTSPS
 */

#include "ofxTSPS/gui/GuiManagerUI.h"
#include "ofxTSPS/gui/Settings.h"
#include "ofAppGLFWWindow.h"

namespace ofxTSPS {
    // these will become global settings
    float guiWidth  = 300;
    float guiHeight = 600;
    float guiTabHeight = 20;
    bool bRetina = false;
    
    enum{
        PARAM_INT, PARAM_FLOAT, PARAM_BOOL
    };
    
    GuiManagerUI::GuiManagerUI() {
        bLastBgState   = false;
        bEventsEnabled = false;
    }
    
    GuiManagerUI::~GuiManagerUI(){
        disableEvents();
    }
    
    void GuiManagerUI::enableEvents(){
        if (bEventsEnabled) return;
        bEventsEnabled = true;
        
    }
    
    void GuiManagerUI::disableEvents(){
        if (!bEventsEnabled) return;
        bEventsEnabled = false;
        
    }
    
    void GuiManagerUI::setup(){
        enableGui = true;
        
        // retina?
        ofAppGLFWWindow * window = (ofAppGLFWWindow*) ofGetWindowPtr();
        if ( window->getPixelScreenCoordScale() != 1 ){
            bRetina = true;
        }
        
        // setup each panel group
        guis["source"];
        guis["sensing"];
        guis["communication"];
        guis["data"];
        guis["custom"];
        
        // to-do: "custom" panel
        
        // setup panel switch buttons
        
        //---- setup source panel ----
        GUICanvas * videoPanel = setupGUI("Select Input", "video", "source");
        
        // video settings
        videoPanel->addLabel("camera settings");
#ifndef OF_VIDEO_CAPTURE_QTKIT
        videoPanel->addToggle("open video settings", &bOpenVideoSettings);
#endif
        vector<string> source_types;
        ofVideoGrabber dummyGrabber;
        vector<ofVideoDevice> devices = dummyGrabber.listDevices();
        for ( int i=0; i<devices.size(); i++){
            source_types.push_back(devices[i].deviceName);
            SourceSelection sel;
            sel.type = CAMERA_VIDEOGRABBER;
            sel.index = i;
            sel.name = devices[i].deviceName;
            currentSources[currentSources.size()] = sel;
        }
        
        Kinect dummyKinect;
        int numKinects = dummyKinect.numAvailable();
        
        for ( int i=0; i<numKinects; i++){
            source_types.push_back("Kinect "+ofToString(i));
            
            SourceSelection sel;
            sel.type = CAMERA_KINECT;
            sel.index = i;
            sel.name = "Kinect "+ofToString(i);
            currentSources[currentSources.size()] = sel;
        }
        
        // todo: Kinect v2
        
        // do we want to remove this?
        
        static OpenNI2 dummyOpenNI;
        int numOpenNI = dummyOpenNI.numAvailable();
        
        for ( int i=0; i<numOpenNI; i++){
            source_types.push_back("OpenNI2 "+ofToString(i));
            
            SourceSelection sel;
            sel.type = CAMERA_OPENNI;
            sel.index = i;
            sel.name = "OpenNI2 "+ofToString(i);
            currentSources[currentSources.size()] = sel;
        }
        
        SourceSelection videofileSelection;
        videofileSelection.type = CAMERA_VIDEOFILE;
        videofileSelection.index = 0;
        videofileSelection.name = "Video File";
        currentSources[currentSources.size()] = videofileSelection;
        source_types.push_back("Video File");
        
#ifdef TARGET_OSX
        SourceSelection syphonSelection;
        syphonSelection.type = CAMERA_SYPHON;
        syphonSelection.index = 0;
        syphonSelection.name = "Syphon";
        currentSources[currentSources.size()] = syphonSelection;
        source_types.push_back("Syphon");
#endif
        SourceSelection customSelection;
        customSelection.type = CAMERA_SYPHON;
        customSelection.index = 0;
        customSelection.name = "custom";
        currentSources[currentSources.size()] = customSelection;
        source_types.push_back("custom");
        
        ofxUIRadio * sourceSelect = videoPanel->addRadio("source type", source_types);
        sourceSelect->getToggles()[0]->setValue(true);
        
        // video files
        GUICanvas * videoFiles = setupGUI("Select Video File", "videoFile", "source");
        
        // video files
        ofxUILabel * videoGroup = videoFiles->addLabel("video files");
        videoFiles->addTextInput("video directory (inside data folder)", "videos");
        vector<string> ext; ext.push_back("mov");
        videoFileDropdown = new ofxUIDynamicRadio("videoFiles", "videos", ext);
        videoFiles->addToggle("refresh directory", &videoFileDropdown->needToRefresh);
        
        videoFiles->addWidgetDown(videoFileDropdown->getWidget());
        
        // camera settings
        GUICanvas * cameraSettings = setupGUI("Setup Image", "image", "source");
        
        // flip + invert
        cameraSettings->addLabel("adjust camera");
        cameraSettings->addToggle("flip horizontal", &settings.bFlipX);
        cameraSettings->addToggle("flip vertical", &settings.bFlipY);
        cameraSettings->addToggle("invert", &settings.bInvert);
        
        // amplification
        cameraSettings->addLabel("amplification");
        cameraSettings->addToggle("use amplification (video gain)", &settings.bAmplify);
        cameraSettings->addIntSlider("amplification amount", 1, 100, &settings.highpassAmp);
        
        //---- end setup source panel ----
        
        // ---- setup sensing panel ----
        GUICanvas * backgroundPanel = setupGUI("background", "sensing");
        
        //background settings
        backgroundPanel->addLabel("background");
        backgroundPanel->addToggle("capture background", &settings.bLearnBackground);
        backgroundPanel->addToggle("blank out background", &settings.bBlankBackground);
        backgroundPanel->addToggle("capture background on startup", &settings.bLearnBackgroundOnStartup);
        backgroundPanel->addSlider("capture X seconds after startup", 0.0f, 10.0f, &settings.captureSeconds);
        
        backgroundPanel->addLabel("background relearn");
        backgroundPanel->addToggle("progressive background recapture", &settings.bLearnBackgroundProgressive);
        backgroundPanel->addSlider("recapture rate", 0.0f, 1000.0f, &settings.fLearnRate);
        
        //differencing settings
        GUICanvas * differencingPanel = setupGUI("differencing", "sensing");
        
        differencingPanel->addLabel("differencing");
        differencingPanel->addIntSlider("threshold", 0, 255, &settings.threshold);
        
        vector<string> multi;
        multi.push_back("light on dark");
        multi.push_back("dark on light");
        multi.push_back("absolute difference");
        differencingPanel->addRadio("type of differencing", multi)->activateToggle("absolute difference");
        
        differencingPanel->addLabel("highpass");
        differencingPanel->addToggle("use highpass", &settings.bHighpass);
        differencingPanel->addIntSlider("highpass pre-blur", 0.0f, 15.0f, &settings.highpassBlur);
        differencingPanel->addIntSlider("highpass kernel size", 0.0f, 15.0f, &settings.highpassKernel);
        differencingPanel->addIntSlider("highpass post-blur", 0.0f, 15.0f, &settings.highpassNoise); // should be float??
       
        differencingPanel->addLabel("smoothing");
        differencingPanel->addToggle("use smoothing", &settings.bSmooth);
        differencingPanel->addIntSlider("smooth amount", 0, 15, &settings.smooth);
        
        //sensing
        
        GUICanvas * trackingPanel = setupGUI("tracking", "sensing");
        
        trackingPanel->addLabel("blobs");
        trackingPanel->addSlider("minimum blob size (% of view)", 0.01f, 50.0f, 1.f); // not auto
        trackingPanel->addSlider("maximum blob size (% of view)", 0.01f, 50.0f, 100.f); // not auto
        trackingPanel->addToggle("find nested blobs", &settings.bFindHoles);
        
        // haar
        
        ofxUILabel * haarGroup = trackingPanel->addLabel("haar tracking");
        trackingPanel->addToggle("detect and send features in blobs", &settings.bDetectHaar)->setParent(haarGroup);
        vector<string> haar_ext; haar_ext.push_back("xml");
        haarFileDropdown = new ofxUIDynamicRadio("haar files", "haar", haar_ext);
        trackingPanel->addWidgetDown(haarFileDropdown->getWidget());
        trackingPanel->addSlider("expand detection area:", 0.0, 200.0f, &settings.haarAreaPadding)->setParent(haarGroup);
        
        // ---- end setup sensing panel ----
        
        // ---- setup communication panel ----
        GUICanvas * oscPanel = setupGUI("OSC", "communication");
        oscPanel->addToggle("send OSC", &settings.bSendOsc);
        oscPanel->addTextInput("receiver IP address (OSC host)", "127.0.0.1");
        oscPanel->addTextInput("receiver port", "12000");
        oscPanel->addToggle("use legacy OSC", &settings.bUseLegacyOsc);
        
        GUICanvas * websocketPanel = setupGUI("WebSockets", "communication");
        
        websocketPanel->addLabel("WebSocket Server");
        websocketPanel->addToggle("send over WebSocket server", &settings.bSendWebSocketServer);
        websocketPanel->addTextInput("websocket server port", "7681");
        websocketPanel->addToggle("open debug URL", &settings.bOpenWebSocketDebugURL);
        
        websocketPanel->addLabel("WebSocket Client");
        websocketPanel->addToggle("send over WebSocket client", &settings.bSendWebSocketClient);
        websocketPanel->addTextInput("websocket host", "localhost");
        websocketPanel->addTextInput("websocket port", "7681");
        websocketPanel->addTextInput("websocket channel (optional)", "");
        websocketPanel->addToggle("use SSL", &settings.webSocketUseSSL);
        
        GUICanvas * spacebrewPanel = setupGUI("Spacebrew", "communication");
        spacebrewPanel->addTextInput("Spacebrew host", "sandbox.spacebrew.cc");
        
        GUICanvas * tuioPanel = setupGUI("TUIO", "communication");
        guis["communication"]["tuio"] = tuioPanel;
        tuioPanel->addToggle("send TUIO", &settings.bSendTuio);
        tuioPanel->addTextInput("receiver IP address (TUIO host)", "127.0.0.1");
        tuioPanel->addTextInput("receiver port", "3333");
        
        GUICanvas * tcpPanel = setupGUI("TCP", "communication");
        tcpPanel->addToggle("send TCP", &settings.bSendTcp);
        tcpPanel->addTextInput("broadcast port (TCP port)", "8888");
        
        // ----- end setup comm panel -----
        
        // ---- setup data panel ----
        GUICanvas * peoplePanel = setupGUI("people", "data");
        peoplePanel->addLabel("options");
        peoplePanel->addToggle("track and send contours", &settings.bSendContours);
        
        ofxUILabel * opticalFlowGroup = peoplePanel->addLabel("optical flow");
        peoplePanel->addToggle("track and send optical flow in blobs", &settings.bTrackOpticalFlow)->setParent(opticalFlowGroup);
        //optical flow
        // To-do: implement these in processors + ofxCv
        //panel.addSlider("filter vectors smaller than:", "MIN_OPTICAL_FLOW", 0, 0, 5.0, false);
        //panel.addSlider("clamp vectors: larger than", "MAX_OPTICAL_FLOW", 10, 5.0, 200, false);
        
        GUICanvas * scenePanel = setupGUI("scene", "data");
        scenePanel->addLabel("options");
        scenePanel->addToggle("send scene data", &settings.bSendScene);
        scenePanel->addIntSlider("scene grid horizontal", 0, 10, &settings.sceneGridX);
        scenePanel->addIntSlider("scene grid vertical", 0, 10, &settings.sceneGridY);
        
        // ---- end setup data panel ----
        
        // master tab bar
        guiController = new ofxUITabBar(0,0,guiWidth, guiTabHeight);
        guiController->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        //guiController->setFont("fonts/times.ttf");
        if ( bRetina ) guiController->setRetinaResolution();
        
        // set up main tabs
        for ( auto s : guiTypes ){
//        for ( auto key : guis ){
            guiControllers[s] = new ofxUITabBar(0,guiTabHeight,guiWidth,guiTabHeight);
            guiControllers[s]->setName(s);
            guiControllers[s]->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            //guiControllers[s]->setFont("fonts/times.ttf");
            if ( bRetina ) guiControllers[s]->setRetinaResolution();
            
            for ( auto g : guis[s] ){
                guiControllers[s]->addCanvas(g.second);
            }
            guiController->addCanvas(guiControllers[s]);
        }
        
        // set up section tabs
        
//        guiTypeButton * sourceButton = new guiTypeButton();
//        sourceButton->setup("source", buttonWidth*1.5, buttonHeight);
//        sourceButton->setPosition(buttonPosition.x, buttonPosition.y);
//        sourceButton->setDimensions(50, 20);
//        sourceButton->setFont(&panel.guiTTFFont);
//        sourceButton->setBackgroundColor(174,139,138,75);
//        sourceButton->setBackgroundSelectColor(174,139,138);
//        buttonPosition.x += 60;
//        
//        guiTypeButton * sensingButton = new guiTypeButton();
//        sensingButton->setup("sensing", buttonWidth*1.5, buttonHeight);
//        sensingButton->setPosition(buttonPosition.x, buttonPosition.y);
//        sensingButton->setDimensions(60, 20);
//        sensingButton->setFont(&panel.guiTTFFont);
//        sensingButton->setBackgroundColor(113,171,154,75);
//        sensingButton->setBackgroundSelectColor(113,171,154);
//        buttonPosition.x += 70;
//        
//        guiTypeButton * communicationButton = new guiTypeButton();
//        communicationButton->setup("communication", buttonWidth*1.5, buttonHeight);
//        communicationButton->setDimensions(90, 20);
//        communicationButton->setPosition(buttonPosition.x, buttonPosition.y);
//        communicationButton->setFont(&panel.guiTTFFont);
//        communicationButton->setBackgroundColor(180,87,128,75);
//        communicationButton->setBackgroundSelectColor(180,87,128);
//        buttonPosition.x += 100;
//        
//        guiTypeButton * dataButton = new guiTypeButton();
//        dataButton->setup("data", buttonWidth*1.5, buttonHeight);
//        dataButton->setDimensions(40, 20);
//        dataButton->setPosition(buttonPosition.x, buttonPosition.y);
//        dataButton->setFont(&panel.guiTTFFont);
//        dataButton->setBackgroundColor(178,101,0,75);
//        dataButton->setBackgroundSelectColor(178,101,0);
//        buttonPosition.x += 50;
//        
//        buttons["source"] = sourceButton;
//        buttons["sensing"] = sensingButton;
//        buttons["communication"] = communicationButton;
//        buttons["data"] = dataButton;
//        
//        ofAddListener(sourceButton->buttonPressed, this, &GuiManagerUI::enableGroup);
//        ofAddListener(sensingButton->buttonPressed, this, &GuiManagerUI::enableGroup);
//        ofAddListener(communicationButton->buttonPressed, this, &GuiManagerUI::enableGroup);
//        ofAddListener(dataButton->buttonPressed, this, &GuiManagerUI::enableGroup);
//        
//        // setup min + max button
//        
//        minimizeButton = new guiTypeButton();
//        minimizeButton->setup("minimize", buttonWidth*1.5, buttonHeight);
//        minimizeButton->setPosition(10,10);
//        minimizeButton->setFont(&panel.guiTTFFont);
//        minimizeButton->setBackgroundColor(205,198,63);
//        minimizeButton->setBackgroundSelectColor(205,198,63);
//        
//        maximizeButton = new guiTypeButton();
//        maximizeButton->setup("maximize", buttonWidth*1.5, buttonHeight);
//        maximizeButton->setPosition(10,10);
//        maximizeButton->disable();
//        maximizeButton->setFont(&panel.guiTTFFont);
//        maximizeButton->setBackgroundSelectColor(0,168,156);
//        maximizeButton->setBackgroundColor(0,168,156);
//        
//        buttons["minimize"] = (minimizeButton);
//        buttons["maximize"] = (maximizeButton);
//        
//        ofAddListener(minimizeButton->buttonPressed, this, &GuiManagerUI::minimize);
//        ofAddListener(maximizeButton->buttonPressed, this, &GuiManagerUI::maximize);
        
        //listen for save / load / reload / saveas events from GUI + pass to quadgui
        
//        ofAddListener(panel.getSaveButton()->buttonPressed, this, &GuiManagerUI::saveEventCatcher);
//        ofAddListener(panel.getRestoreButton()->buttonPressed, this, &GuiManagerUI::reloadEventCatcher);
//        ofAddListener(panel.getLoadButton()->buttonPressed, this, &GuiManagerUI::loadEventCatcher);
//        ofAddListener(panel.getSaveAsButton()->buttonPressed, this, &GuiManagerUI::saveAsEventCatcher);
        
        bHasCustomPanel = false;
        
        ofEventArgs nullArgs;
        update(nullArgs);
        enableEvents();
    }
    
    GUICanvas * GuiManagerUI::setupGUI( string name, string type ){
        return setupGUI(name, ofToLower(name), type);
    }
    
    GUICanvas * GuiManagerUI::setupGUI( string longName, string name, string type ){
        GUICanvas * panel = new GUICanvas(0, 50, guiWidth, guiHeight);
        panel->setName(longName);
        if ( bRetina ) panel->setRetinaResolution();
        //panel->setFont("fonts/times.ttf");
        if ( guis[type].size() == 0 ) guiTypes.push_back(type);
        guis[type][name] = panel;
        guiNames.push_back(name);
        
        return panel;
    }
    
    void GuiManagerUI::refreshSourcePanel(){
        // silent for now..
    }
    
    map<int,SourceSelection> * GuiManagerUI::getCurrentSources(){
        return &currentSources;
    }
    
    int GuiManagerUI::getSourceSelectionIndex( SourceType type, int deviceIndex){
        map<int,SourceSelection>::iterator it = currentSources.begin();
        for ( it; it != currentSources.end(); it++){
            if ( it->second.type == type && it->second.index == deviceIndex ){
                return it->first;
            }
        }
        return -1;
    }
    
    
    void GuiManagerUI::selectSource( SourceType type, int deviceIndex ){
        // camera
        int index = -1;
        for ( auto s : currentSources ){
            if ( s.second.type == type && s.second.index == deviceIndex ){
                index = s.first;
            }
        }
        ((ofxUIRadio*) guis["source"]["video"]->getWidget("source type"))->activateToggle( currentSources[index].name );
        
        settings.inputType      = currentSources[index].type;
        settings.cameraIndex    = currentSources[index].index;
        
        if ( settings.inputType == CAMERA_VIDEOFILE ){
        } else {
            guis["source"]["videoFile"]->disable();
        }
    }
    
    void GuiManagerUI::addCustomGui(){
    }
    
    void GuiManagerUI::enableGroup( string & groupName ){
        enableGroup(groupName.c_str());
    }
    
    void GuiManagerUI::enableGroup( string groupName ){}
    
    void GuiManagerUI::disableGroup( string groupName ){}
    
    vector<string> GuiManagerUI::getGroupNames(){}
    
    void GuiManagerUI::addSlider(string name, int* value, int min, int max){}
    
    void GuiManagerUI::addSlider(string name, float* value, float min, float max){}
    
    void GuiManagerUI::addToggle(string name, bool* value){}
    
    /***************************************************************
     GET + SET PANEL INFO / SELECTED PANEL
     ***************************************************************/
    
    void GuiManagerUI::setHaarEnabled( bool bHaar ){
        showHideWidgetChildren( guis["sensing"]["tracking"], "haar tracking", bHaar);
    }
    
    void GuiManagerUI::setOpticalFlowEnabled( bool bOpticalFlow ){
        showHideWidgetChildren( guis["data"]["people"], "optical flow", bOpticalFlow);
    }
    
    
    /***************************************************************
     GET + SET PANEL INFO / SELECTED PANEL
     ***************************************************************/
    
    void GuiManagerUI::showHideWidgetChildren( ofxUICanvas * group, string name, bool bShow ){
        ofxUIWidget * parent = group->getWidget(name);
        if ( parent ){
            for ( auto * w : group->getWidgets() ){
                if ( w->getParent() == parent ){
                    w->setVisible(bShow);
                }
            }
            parent->setVisible(bShow);
        }
    }
    
    void GuiManagerUI::update(ofEventArgs &e){
        update();
    }
    
    void GuiManagerUI::update(){
        if(!enableGui){
            //if the gui is not shown no need to propagate values
            return;
        }
        
        // camera
		int index = ((ofxUIRadio*) guis["source"]["video"]->getWidget("source type"))->getValue();
        
        settings.inputType      = currentSources[index].type;
        settings.cameraIndex    = currentSources[index].index;
        
        if ( settings.inputType == CAMERA_VIDEOFILE ){
        } else {
            guis["source"]["videoFile"]->disable();
        }
        
        // hack for layout
        for ( auto key : guis ){
            if ( guiControllers.count(key.first) == 0 ) continue;
            bool bParentActive = guiControllers[key.first]->isEnabled();
            bool bSomeoneEnabled = false;
            
            for ( auto g : guis[key.first] ){
                if (!bParentActive) g.second->disable();
                if (!bSomeoneEnabled) bSomeoneEnabled = g.second->isEnabled();
            }
            if ( bParentActive && !bSomeoneEnabled ){
                guiControllers[key.first]->setActiveCanvas( guis[key.first].begin()->second );
            }
        }
        
        // video files
        string currentDir = ((ofxUITextInput*) guis["source"]["videoFile"]->getWidget("video directory (inside data folder)"))->getTextString();
        
        if ( settings.videoDirectory != currentDir ){
            settings.videoDirectory = currentDir;
            
            int numVideos = videoFileDropdown->listDir(ofToDataPath(settings.videoDirectory, true));
            
            if ( numVideos == 0 ){
                ofLog( OF_LOG_VERBOSE, "No videos found, switching to camera input" );
            }
        }
        if( videoFileDropdown != NULL && videoFileDropdown->getSelectedPath() != "" ){
            settings.videoFile = videoFileDropdown->getSelectedPath();
        }
        
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
        
        //scale var to be right for tracker
        settings.minBlob = ((ofxUISlider*) guis["sensing"]["tracking"]->getWidget("minimum blob size (% of view)"))->getValue()/100.0f;
        settings.maxBlob = ((ofxUISlider*) guis["sensing"]["tracking"]->getWidget("maximum blob size (% of view)"))->getValue()/100.0f;
        
        settings.trackType = ((ofxUIRadio*) guis["sensing"]["differencing"]->getWidget("type of differencing"))->getValue();
        
        if(haarFileDropdown->getSelectedPath() != "" ){
            settings.haarFile = haarFileDropdown->getSelectedPath();
        }
        
        //update osc stuff
        
        settings.oscHost =
            ((ofxUITextInput*) guis["communication"]["osc"]->getWidget("receiver IP address (OSC host)") )->getTextString();
        settings.oscPort =
            ofToInt(((ofxUITextInput*) guis["communication"]["osc"]->getWidget("receiver port") )->getTextString());
        
        // update TUIO
        settings.tuioHost =
            ((ofxUITextInput*) guis["communication"]["tuio"]->getWidget("receiver IP address (TUIO host)") )->getTextString();
        
        settings.tuioPort =
            ofToInt(((ofxUITextInput*) guis["communication"]["tuio"]->getWidget("receiver port") )->getTextString());
        
        // update TCP
        settings.tcpPort =
            ofToInt(((ofxUITextInput*) guis["communication"]["tcp"]->getWidget("broadcast port (TCP port)") )->getTextString());
        
        // update Spacebrew
        settings.spacebrewHost =
            ((ofxUITextInput*) guis["communication"]["spacebrew"]->getWidget("Spacebrew host") )->getTextString();
        
        // update WebSockets
        settings.webSocketServerPort =
            ofToInt(((ofxUITextInput*) guis["communication"]["websockets"]->getWidget("websocket server port") )->getTextString());
        settings.webSocketPort =
            ofToInt(((ofxUITextInput*) guis["communication"]["websockets"]->getWidget("websocket port") )->getTextString());
        settings.webSocketHost =
            ((ofxUITextInput*) guis["communication"]["websockets"]->getWidget("websocket host") )->getTextString();
        
        settings.webSocketChannel =
            ((ofxUITextInput*) guis["communication"]["websockets"]->getWidget("websocket channel (optional)") )->getTextString();
        
		if (settings.bOpenWebSocketDebugURL){
            settings.bOpenWebSocketDebugURL = false;
            ofLaunchBrowser( "http://localhost:"+ ofToString(settings.webSocketServerPort)) ;
        }
        
        // UPDATE GUI QUADS HERE
        // because this returns a pointer to the actual points that get updated,
        // you store it in an array so it doesn't get updated when it draws
        ofPoint * scaledPoints = quadGui.getScaledQuadPoints(cameraWidth,cameraHeight);
        for (int i=0; i<4; i++){
            settings.quadWarpScaled[i] = scaledPoints[i];
        }
        
        //get xml
//        settings.currentXmlFile = panel.getCurrentXMLFile();
    }
    
    
    void GuiManagerUI::setupQuadGui ( int _cameraWidth, int _cameraHeight )
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
    
    void GuiManagerUI::drawQuadGui(){
        quadGui.draw();
    };
    
    void GuiManagerUI::drawQuadGui( int x, int y, int width, int height ){
        quadGui.x = x;
        quadGui.y = y;
        quadGui.setScale((float) width/quadGui.width, (float) height/quadGui.height );
        drawQuadGui();
    };
    
    
    // ZACK BOKA: Added so the quadGui instance can know when image warping is allowed to occur
    //            (i.e., the image can only get warped when in Camera View).
    void GuiManagerUI::changeGuiCameraView(bool bCameraView) {
        quadGui.bCameraView = bCameraView;
    };
        
    /***************************************************************
     ENABLE / DISABLE ELEMENTS
     ***************************************************************/
    
    //----------------------------------------------------------
    void GuiManagerUI::enableElement( string name ){
    };
    
    //----------------------------------------------------------
    void GuiManagerUI::disableElement( string name ){
    };
    
    /***************************************************************
     GET EVENTS FROM GUI BUTTONS
     ***************************************************************/
    
    void GuiManagerUI::saveEventCatcher( string & buttonName){
//        settings.currentXmlFile = panel.getCurrentXMLFile();
        quadGui.saveToFile(settings.currentXmlFile);
    };
    
    void GuiManagerUI::reloadEventCatcher( string & buttonName){
        quadGui.readFromFile(settings.currentXmlFile);
    };
    
    void GuiManagerUI::loadEventCatcher( string & buttonName){
//        settings.currentXmlFile = panel.getCurrentXMLFile();
        quadGui.readFromFile(settings.currentXmlFile);
    };
    
    void GuiManagerUI::saveAsEventCatcher( string & buttonName){
//        settings.currentXmlFile = panel.getCurrentXMLFile();
        quadGui.readFromFile(settings.currentXmlFile);
    };
    
    /***************************************************************
     GET + SET PANEL INFO / SELECTED PANEL
     ***************************************************************/
    
    void GuiManagerUI::loadSettings( string xmlFile ){
//        panel.loadSettings(xmlFile);
        if (quadGuiSetup) quadGui.readFromFile(xmlFile);
        update();
    };
    
    void GuiManagerUI::draw() {
        if(enableGui){
            
            ofSetColor(184,169,121);
            ofFill();
//            ofRect(10,40,panel.getWidth(),20);
            ofSetColor(255);
//            panel.guiTTFFont.drawString("CONFIGURE TSPS: "+ ofToUpper(enabledPanelGroup), 15, 45 + panel.guiTTFFont.getSize() );
//            
//            map<std::string, guiTypeButton*>::iterator it;
//            for( it=buttons.begin(); it!=buttons.end(); it++ ){
//                it->second->render();
//            }
//            
//            // draw custom buttons
//            for( it=customButtons.begin(); it!=customButtons.end(); it++ ){
//                it->second->render();
//            }
        }
    }
    
    /***************************************************************
     MINIMIZE + MAXIMIZE THE APP
     ***************************************************************/
    
    //----------------------------------------------------------
    void GuiManagerUI::minimize(string & button){
        settings.bMinimized = true;
        ofSetWindowShape(100,40);
//        minimizeButton->disable();
//        maximizeButton->enable();
    };
    
    //----------------------------------------------------------
    void GuiManagerUI::maximize(string & button){
        settings.bMinimized = false;
        ofSetWindowShape(1024,768);
//        minimizeButton->enable();
//        maximizeButton->disable();
    };
    
    
    /***************************************************************
     GET SETTINGS OBJECT (NO LONGER SINGLETON!)
     ***************************************************************/
    
    //----------------------------------------------------------
    Settings * GuiManagerUI::getSettings(){
        return &settings;
    };
}
