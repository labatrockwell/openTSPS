/*
 *  GuiManagerUI.cpp
 *  openTSPS
 */

#include "ofxTSPS/gui/GuiManagerUI.h"
#include "ofxTSPS/gui/Settings.h"

namespace ofxTSPS {
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
        
        // these will become global settings
        float guiWidth  = 300;
        float guiHeight = 600;
        float guiTabHeight = 20;
        
        // setup each panel group
        guis["source"];
        guis["sensing"];
        guis["communication"];
        guis["data"];
        guis["custom"];
        
        // to-do: "custom" panel
        
        // master tab bar
        guiController = new ofxUITabBar(0,0,guiWidth, guiTabHeight);
        
        // setup panel switch buttons
        
        //---- setup source panel ----
        ofxUIScrollableCanvas * videoPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        videoPanel->setName("video");
        guis["source"]["video"]= videoPanel;
        
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
            currentSources[currentSources.size()] = sel;
        }
        
        Kinect dummyKinect;
        int numKinects = dummyKinect.numAvailable();
        
        for ( int i=0; i<numKinects; i++){
            source_types.push_back("Kinect "+ofToString(i));
            
            SourceSelection sel;
            sel.type = CAMERA_KINECT;
            sel.index = i;
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
            currentSources[currentSources.size()] = sel;
        }
        
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
        
        ofxUIDropDownList * sourceSelect = videoPanel->addDropDownList("source type", source_types);
        sourceSelect->setAllowMultiple(false);
        sourceSelect->getToggles()[0]->setValue(true);
        
        // video files
        ofxUILabel * videoGroup = videoPanel->addLabel("video files");
        videoPanel->addTextInput("video directory (inside data folder)", "videos")->setParent(videoGroup);
        
        vector<string> ext; ext.push_back("mov");
        videoFileDropdown = new ofxUIDirectoryDropdown("videoFiles", "videos", ext);
        videoPanel->addWidgetDown(videoFileDropdown);
        videoFileDropdown->setParent(videoGroup);
        videoPanel->addToggle("refresh directory", &videoFileDropdown->needToRefresh)->setParent(videoGroup);
        
        // flip + invert
        videoPanel->addLabel("adjust camera");
        videoPanel->addToggle("flip horizontal", &settings.bFlipX);
        videoPanel->addToggle("flip vertical", &settings.bFlipY);
        videoPanel->addToggle("invert", &settings.bInvert);
        
        // amplification
        videoPanel->addLabel("amplification");
        videoPanel->addToggle("use amplification (video gain)", &settings.bAmplify);
        videoPanel->addIntSlider("amplification amount", 1, 100, &settings.highpassAmp);
        
        //---- end setup source panel ----
        
        // ---- setup sensing panel ----
        ofxUIScrollableCanvas * backgroundPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        backgroundPanel->setName("background");
        guis["sensing"]["background"] = ( backgroundPanel );
        
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
        ofxUIScrollableCanvas * differencingPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        differencingPanel->setName("differencing");
        guis["sensing"]["differencing"] = ( differencingPanel );
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
        
        ofxUIScrollableCanvas * trackingPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        trackingPanel->setName("tracking");
        guis["sensing"]["tracking"] = ( trackingPanel );
        
        trackingPanel->addLabel("blobs");
        trackingPanel->addSlider("minimum blob size (% of view)", 0.01f, 50.0f, 1.f); // not auto
        trackingPanel->addSlider("maximum blob size (% of view)", 0.01f, 50.0f, 100.f); // not auto
        trackingPanel->addToggle("find nested blobs", &settings.bFindHoles);
        
        // haar
        
        ofxUILabel * haarGroup = trackingPanel->addLabel("haar tracking");
        trackingPanel->addToggle("detect and send features in blobs", &settings.bDetectHaar)->setParent(haarGroup);
        vector<string> haar_ext; haar_ext.push_back("xml");
        haarFileDropdown = new ofxUIDirectoryDropdown("haar files", "haar", haar_ext);
        trackingPanel->addWidgetDown(haarFileDropdown);
        haarFileDropdown->setParent(haarGroup);
        trackingPanel->addSlider("expand detection area:", 0.0, 200.0f, &settings.haarAreaPadding)->setParent(haarGroup);
        
        // ---- end setup sensing panel ----
        
        // ---- setup communication panel ----
        
        ofxUIScrollableCanvas * oscPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        oscPanel->setName("OSC");
        guis["communication"]["OSC"] = oscPanel;
        oscPanel->addToggle("send OSC", &settings.bSendOsc);
        oscPanel->addTextInput("receiver IP address (OSC host)", "127.0.0.1");
        oscPanel->addTextInput("receiver port", "12000");
        oscPanel->addToggle("use legacy OSC", &settings.bUseLegacyOsc);
        
        ofxUIScrollableCanvas * websocketPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        websocketPanel->setName("WebSockets");
        guis["communication"]["websockets"] = websocketPanel;
        
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
        
        ofxUIScrollableCanvas * spacebrewPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        spacebrewPanel->setName("Spacebrew");
        guis["communication"]["spacebrew"] = spacebrewPanel;
        spacebrewPanel->addTextInput("Spacebrew host", "sandbox.spacebrew.cc");
        
        ofxUIScrollableCanvas * tuioPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        tuioPanel->setName("TUIO");
        guis["communication"]["tuio"] = tuioPanel;
        tuioPanel->addToggle("send TUIO", &settings.bSendTuio);
        tuioPanel->addTextInput("receiver IP address (TUIO host)", "127.0.0.1");
        tuioPanel->addTextInput("receiver port", "3333");
        
        ofxUIScrollableCanvas * tcpPanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        tcpPanel->setName("TCP");
        guis["communication"]["tcp"] = tcpPanel;
        tcpPanel->addToggle("send TCP", &settings.bSendTcp);
        tcpPanel->addTextInput("broadcast port (TCP port)", "8888");
        
        // ----- end setup comm panel -----
        
        // ---- setup data panel ----
        
        ofxUIScrollableCanvas * peoplePanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        peoplePanel->setName("people");
        guis["data"]["people"] = peoplePanel;
        
        peoplePanel->addLabel("options");
        peoplePanel->addToggle("track and send contours", &settings.bSendContours);
        
        ofxUILabel * opticalFlowGroup = peoplePanel->addLabel("optical flow");
        peoplePanel->addToggle("track and send optical flow in blobs", &settings.bTrackOpticalFlow)->setParent(opticalFlowGroup);
        //optical flow
        // To-do: implement these in processors + ofxCv
        //panel.addSlider("filter vectors smaller than:", "MIN_OPTICAL_FLOW", 0, 0, 5.0, false);
        //panel.addSlider("clamp vectors: larger than", "MAX_OPTICAL_FLOW", 10, 5.0, 200, false);
        
        ofxUIScrollableCanvas * scenePanel = new ofxUIScrollableCanvas(0, 50, guiWidth, guiHeight);
        scenePanel->setName("scene");
        guis["data"]["scene"] = scenePanel;
        scenePanel->addLabel("options");
        scenePanel->addToggle("send scene data", &settings.bSendScene);
        scenePanel->addIntSlider("scene grid horizontal", 0, 10, &settings.sceneGridX);
        scenePanel->addIntSlider("scene grid vertical", 0, 10, &settings.sceneGridY);
        
        // ---- end setup data panel ----
        
        // set up main tabs
        guiController = new ofxUITabBar();
        for ( auto key : guis ){
            guiControllers[key.first] = new ofxUITabBar(200,0,guiWidth,guiTabHeight);
            guiControllers[key.first]->setName(key.first);
            
            for ( auto g : guis[key.first] ){
                guiControllers[key.first]->addCanvas(g.second);
            }
            guiController->addCanvas(guiControllers[key.first]);
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
        guis["sensing"]["tracking"]->getWidget("haar tracking")->setVisible(bHaar);
    }
    
    void GuiManagerUI::setOpticalFlowEnabled( bool bOpticalFlow ){
        guis["data"]["people"]->getWidget("optical flow")->setVisible(bOpticalFlow);
    }
    
    
    /***************************************************************
     GET + SET PANEL INFO / SELECTED PANEL
     ***************************************************************/
    
    int GuiManagerUI::getSelectedPanel(){
    }
    
    void GuiManagerUI::setSelectedPanel( int index ){
    }
    
    void GuiManagerUI::setSelectedPanel( string name ){
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
        vector<int> inds = ((ofxUIDropDownList*) guis["source"]["video"]->getWidget("source type"))->getSelectedIndeces();
		int index = inds.size() > 0 ? inds[0] : 0;
        
        settings.inputType      = currentSources[index].type;
        settings.cameraIndex    = currentSources[index].index;
        
        if ( settings.inputType == CAMERA_VIDEOFILE){
            guis["source"]["video"]->getWidget("video files")->setVisible(true);
        } else {
            guis["source"]["video"]->getWidget("video files")->setVisible(false);
        }
        
        // video files
        string currentDir = ((ofxUITextInput*) guis["source"]["video"]->getWidget("video directory (inside data folder)"))->getTextString();
        if ( settings.videoDirectory != currentDir ){
            settings.videoDirectory = currentDir;
            
            int numVideos = videoFileDropdown->listDir(ofToDataPath(settings.videoDirectory, true));
            
            if ( numVideos == 0 ){
                ofLog( OF_LOG_VERBOSE, "No videos found, switching to camera input" );
            }
        }
        if(videoFileDropdown->getSelectedNames().size() != 0 && videoFileDropdown->getSelectedNames()[0] != "" ){
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
        
        if(haarFileDropdown->getSelectedNames().size() != 0 && haarFileDropdown->getSelectedNames()[0] != "" ){
            settings.haarFile = haarFileDropdown->getSelectedPath();
        }
        
        //update osc stuff
        
        settings.oscHost =
            ((ofxUITextInput*) guis["communication"]["OSC"]->getWidget("receiver IP address (OSC host)") )->getTextString();
        settings.oscPort =
            ofToInt(((ofxUITextInput*) guis["communication"]["OSC"]->getWidget("receiver port") )->getTextString());
        
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
//        ofPoint * scaledPoints = quadGui.getScaledQuadPoints(cameraWidth,cameraHeight);
//        for (int i=0; i<4; i++){
//            settings.quadWarpScaled[i] = scaledPoints[i];
//        }
        
        //get xml
//        settings.currentXmlFile = panel.getCurrentXMLFile();
    }
    
    /*
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
     */
    
    /***************************************************************
     GET + SET SPECIFIC VALUES FROM THE GUI
     ***************************************************************/
    
    //----------------------------------------------------------
    bool GuiManagerUI::getValueB( string name ){
    }
    
    //----------------------------------------------------------
    int GuiManagerUI::getValueI( string name ){
    }
    
    //----------------------------------------------------------
    float GuiManagerUI::getValueF( string name ){
    }
    
    //----------------------------------------------------------
    string GuiManagerUI::getValueS( string name ){
    }
    
    //----------------------------------------------------------
    void GuiManagerUI::setValueB( string name, bool val ){
    }
    
    //----------------------------------------------------------
    void GuiManagerUI::setValueI( string name, int val ){
    }
    
    //----------------------------------------------------------
    void GuiManagerUI::setValueF( string name, float val ){
    }
    
    //----------------------------------------------------------
    void GuiManagerUI::setValueS( string name, string val ){
    }
    
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
