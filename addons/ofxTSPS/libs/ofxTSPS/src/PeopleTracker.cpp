#include "ofxTSPS/PeopleTracker.h"

//scales down tracking images for improved performance
#define TRACKING_SCALE_FACTOR .5

//Fix for FMAX not in Visual Studio C++
#if defined _MSC_VER
#define fmax max
#define fmin min
#pragma warning (disable:4996)
#define snprintf sprintf_s
#endif

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
namespace ofxTSPS {
    
#pragma mark Setup
    
    //---------------------------------------------------------------------------
    PeopleTracker::PeopleTracker(){
        p_Settings = NULL;
        hasMouseEvents = false;
        tspsProcessor = NULL;
    }
    
    //---------------------------------------------------------------------------
    PeopleTracker::~PeopleTracker(){
        if ( hasMouseEvents ){
            hasMouseEvents = false;
            ofRemoveListener(ofEvents().mousePressed, this, &PeopleTracker::mousePressed);
        }
        tspsProcessor->exit();
        delete tspsProcessor;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setup(int w, int h, string settingsfile){	
        ofxAddTSPSListeners( this );
        ofAddListener(ofEvents().mousePressed, this, &PeopleTracker::mousePressed);
        hasMouseEvents = true;
        
        width  = w;
        height = h;
        
        //useful!
        blackPixels = ofPixels();
        blackPixels.allocate(width, height, 1);
        blackPixels.set(0);
        
        cameraImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        warpedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        backgroundImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        backgroundImage.setFromPixels(blackPixels);
        differencedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        
        grayDiff.allocate(width, height);
        
        //set tracker
        bOscEnabled = bTuioEnabled = bTcpEnabled = bWebSocketServerEnabled = bWebSocketClientEnabled = false;
        p_Settings = gui.getSettings();
        
        //setup gui
        gui.setup();
        gui.setupQuadGui( width, height );
        gui.loadSettings( settingsfile );
        
        activeHeight = ofGetHeight();
        activeWidth = ofGetWidth();
        activeViewIndex = 4;
        
        //setup view rectangles 
        
        cameraView.setup(width, height);
        adjustedView.setup(width, height);
        bgView.setup(width, height);
        processedView.setup(width, height);
        dataView.setup(width, height);
        
        updateViewRectangles();
        
        cameraView.setImage(&cameraImage);
        cameraView.setTitle("Camera Source View", "Camera");
        cameraView.setColor(218,173,90);
        
        adjustedView.setImage(&warpedImage);
        adjustedView.setTitle("Adjusted Camera View", "Adjusted");
        adjustedView.setColor(174,139,138);
        
        bgView.setImage(&backgroundImage);
        bgView.setTitle("Background Reference View", "Background");
        bgView.setColor(213,105,68);
		
        processedView.setImage(&differencedImage);
        processedView.setTitle("Differenced View", "Differencing");
        processedView.setColor(113,171,154);
        
        dataView.setTitle("Data View", "Data");
        dataView.setColor(191,120,0);
        
        setActiveView(PROCESSED_VIEW);
        
        lastHaarFile = "";
        
        // setup default processor
        if ( tspsProcessor == NULL ){
            setProcessor( new CvProcessor);
        }    
        tspsProcessor->setup( width, height, &scene, &trackedPeople );
        
        // setup gui based on processor capabilities
        gui.setHaarEnabled( tspsProcessor->canTrackHaar() );
        gui.setOpticalFlowEnabled( tspsProcessor->canTrackOpticalFlow() );
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setProcessor ( Processor * _processor ){
        tspsProcessor = _processor;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::onPersonEntered( EventArgs & tspsEvent ){
        ofPoint centroid = tspsEvent.person->getCentroidNormalized(width, height);
        
        if(bTuioEnabled){
            tuioClient.cursorPressed(1.0*centroid.x/width, 1.0*centroid.y/height, tspsEvent.person->oid );
        }
        if(bOscEnabled){
            oscClient.personEntered(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
        if(bTcpEnabled){
            tcpClient.personEntered(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
        if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.personEntered(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::onPersonUpdated( EventArgs & tspsEvent ){
        ofPoint centroid = tspsEvent.person->getCentroidNormalized(width, height);
        
        if (bTuioEnabled){
            ofPoint tuioCursor = tspsEvent.person->getCentroidNormalized(width, height);
            tuioClient.cursorDragged( tuioCursor.x, tuioCursor.y, tspsEvent.person->oid);
        }
        
        if (bOscEnabled){
            oscClient.personUpdated(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if (bTcpEnabled){
            tcpClient.personUpdated(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if ( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.personUpdated(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::onPersonWillLeave( EventArgs & tspsEvent ){
        ofPoint centroid = tspsEvent.person->getCentroidNormalized(width, height);
        if (bTuioEnabled) {
            tuioClient.cursorReleased(centroid.x, centroid.y, tspsEvent.person->oid);	
        }
        //send osc kill message if enabled
        if (bOscEnabled){
            oscClient.personWillLeave(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        };
        
        //send tcp kill message if enabled
        if(bTcpEnabled){
            tcpClient.personWillLeave(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.personWillLeave(tspsEvent.person, centroid, width, height, p_Settings->bSendOscContours);
        }
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::resize( int w, int h ){
        width  = w;
        height = h;
        
        cameraImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        warpedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        backgroundImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        differencedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        
        grayDiff.allocate(width, height);
        gui.setupQuadGui( width, height );
        
        activeViewIndex = 4;
        
        //setup view rectangles 
        
        cameraView.setup(width, height);
        adjustedView.setup(width, height);
        bgView.setup(width, height);
        processedView.setup(width, height);
        dataView.setup(width, height);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setHaarXMLFile(string haarFile){
        haarFile = "haar/" + haarFile;
        
        //check if haar file has changed
        if(lastHaarFile != haarFile){
            ofLog(OF_LOG_VERBOSE, "changing haar file to " + haarFile);
            tspsProcessor->setHaarXMLFile(haarFile);
            lastHaarFile = haarFile;
        }
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark Setup Communication
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setupTuio(string ip, int port){
        ofLog(OF_LOG_VERBOSE, "SEND TUIO");
        bTuioEnabled = true;
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->oscPort = port;
        p_Settings->oscHost = ip;
        tuioClient.setup(ip, port);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setupOsc(string ip, int port)
    {
        ofLog(OF_LOG_VERBOSE, "SEND OSC");
        bOscEnabled = true;
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->oscPort = port;
        p_Settings->oscHost = ip;
        oscClient.setupSender(ip, port);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setupTcp(int port)
    {
        bTcpEnabled = true;
        ofLog(OF_LOG_VERBOSE, "SEND TCP TO PORT "+port);
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->tcpPort = port;
        tcpClient.setup(port);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setupWebSocketServer( int port)
    {
        ofLog(OF_LOG_VERBOSE, "SEND WEBSOCKET SERVER ON PORT "+port);
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        bWebSocketServerEnabled = true;
        p_Settings->webSocketServerPort = port;
        gui.setValueI("WSS_PORT", port);
        bWebSocketServerEnabled = webSocketServer.setupServer(port);
        p_Settings->bSendWebSocketServer = bWebSocketServerEnabled;
        gui.setValueB("SEND_WSS", p_Settings->bSendWebSocketServer);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setupWebSocketClient( string host, int port, bool bUseSSL, string channel) {
        ofLog(OF_LOG_VERBOSE, "SEND WEBSOCKET CLIENT AT ws://"+host+channel+":"+ofToString( port ));
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        bWebSocketClientEnabled = true;
        p_Settings->webSocketHost = host;
        p_Settings->webSocketPort = port;
        p_Settings->webSocketChannel = channel;
        p_Settings->webSocketUseSSL = bUseSSL;
        bWebSocketClientEnabled = webSocketServer.setupClient( host, port, false, channel);
        p_Settings->bSendWebSocketClient = bWebSocketClientEnabled;
        gui.setValueS("WS_HOST", host);
        gui.setValueS("WS_PORT", ofToString( port ));
        gui.setValueS("WS_CHANNEL", channel);
        gui.setValueB("WS_USESSL", bUseSSL);
        gui.setValueB("SEND_WS", p_Settings->bSendWebSocketClient);
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark Track People
    
    //---------------------------------------------------------------------------
    void PeopleTracker::updateSettings()
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        setHaarXMLFile(p_Settings->haarFile);
        
        //----------------------------------------------
        // Processor
        //----------------------------------------------
        
        // processor settings
        //processor->setTrackContours ( true );
        tspsProcessor->setTrackHaar ( p_Settings->bDetectHaar );
        tspsProcessor->setTrackSkeleton ( p_Settings->bTrackSkeleton );
        tspsProcessor->setTrackOpticalFlow( p_Settings->bTrackOpticalFlow );
        
        tspsProcessor->setThreshold( p_Settings->threshold );
        tspsProcessor->setBlobSettings( p_Settings->minBlob, p_Settings->maxBlob, p_Settings->bFindHoles);
        tspsProcessor->setOpticalflowMinMax( p_Settings->maxOpticalFlow, p_Settings->minOpticalFlow );
        tspsProcessor->setHaarPadding( p_Settings->haarAreaPadding );
        
        //----------------------------------------------
        // COMMUNICATION : Enable
        //----------------------------------------------
        
        //check to enable OSC
        if (p_Settings->bSendOsc && !bOscEnabled) setupOsc(p_Settings->oscHost, p_Settings->oscPort);
        else if (!p_Settings->bSendOsc) bOscEnabled = false;
        
        //check to enable TUIO
        if (p_Settings->bSendTuio && !bTuioEnabled) setupTuio(p_Settings->tuioHost, p_Settings->tuioPort);
        else if (!p_Settings->bSendTuio) bTuioEnabled = false;
        
        //check to enable TCP
        if (p_Settings->bSendTcp && !bTcpEnabled) setupTcp(p_Settings->tcpPort);
        else if (!p_Settings->bSendTcp) bTcpEnabled = false;
        
        //check to enable websockets
        if (p_Settings->bSendWebSocketClient && !bWebSocketClientEnabled){
            setupWebSocketClient(p_Settings->webSocketHost, p_Settings->webSocketPort, p_Settings->webSocketUseSSL, p_Settings->webSocketChannel);
        } else if (!p_Settings->bSendWebSocketClient){
            bWebSocketClientEnabled = false;
            webSocketServer.closeClient();
        }
        if (p_Settings->bSendWebSocketServer && !bWebSocketServerEnabled){
            setupWebSocketServer(p_Settings->webSocketServerPort);
        } 
        if (!p_Settings->bSendWebSocketServer){
            bWebSocketServerEnabled = false;
            webSocketServer.closeServer();
        }
        //----------------------------------------------
        // COMMUNICATION : Send data
        //----------------------------------------------	
        
        if(bTuioEnabled){
            tuioClient.update();		
        }
        
        if (bOscEnabled){
            oscClient.ip = p_Settings->oscHost;
            oscClient.port = p_Settings->oscPort;
            oscClient.useLegacy = p_Settings->bUseLegacyOsc;
            oscClient.update();
        };
        
        if (bTcpEnabled){
            tcpClient.port = p_Settings->oscPort;
            tcpClient.update();
            tcpClient.send();
        }
        
        if ( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            if ( bWebSocketClientEnabled && (p_Settings->webSocketPort != webSocketServer.getPort() || p_Settings->webSocketHost != webSocketServer.getHost()) ){
                webSocketServer.closeClient();
                setupWebSocketClient( p_Settings->webSocketHost, p_Settings->webSocketPort );
            }
            
            if ( bWebSocketServerEnabled && p_Settings->webSocketServerPort != webSocketServer.getServerPort() ){
                webSocketServer.closeServer();
                setupWebSocketServer( p_Settings->webSocketServerPort );
            }
            
            //sent automagically
            webSocketServer.send();
        }
        
        //switch camera view if new panel is selected
        if (p_Settings->currentPanel != p_Settings->lastCurrentPanel) setActiveView(p_Settings->currentPanel + 1);
        
        // Set the current view within the gui so the image can only be warped when in Camera View
        if (cameraView.isActive()) {
            gui.changeGuiCameraView(true);
        } else {
            gui.changeGuiCameraView(false);
        }
        gui.update();
    }
    
    /**
     * Core Method
     * Run every frame to update
     * the system to the current location
     * of people
     */
    //---------------------------------------------------------------------------
    void PeopleTracker::trackPeople()
    {	
        if (p_Settings == NULL) p_Settings = gui.getSettings();
    	
        //-------------------------------
        // Camera view
        //-------------------------------
        
        //warp background
        //colorImage = grayImage;
        //colorImageWarped = colorImage;
        getQuadSubImage(cameraImage, warpedImage, p_Settings->quadWarpScaled, OF_IMAGE_GRAYSCALE);	
        
        // update scaled down images
        grayDiff.setFromPixels(warpedImage.getPixelsRef());
        //grayDiff = grayImageWarped;
        
        //amplify
        if(p_Settings->bAmplify){
            grayDiff.amplify(grayDiff, p_Settings->highpassAmp/15.0f);
            warpedImage.setFromPixels(grayDiff.getPixelsRef());
        }
        
        // set base camera image
        tspsProcessor->setCameraImage( warpedImage );
        
        //learn background
        if (p_Settings->bLearnBackground){
            backgroundImage = warpedImage;
            tspsProcessor->captureBackground( warpedImage );
        }
        
        //progressive relearn background
        if (p_Settings->bLearnBackgroundProgressive){
            tspsProcessor->progressiveBackground( warpedImage, p_Settings->fLearnRate * .00001 );
        }
        
        // black out background?
        if (p_Settings->bBlankBackground){
            tspsProcessor->blankBackground();
            backgroundImage.setFromPixels(blackPixels);
        }
        
        //-----------------------
        // Difference image
        //-----------------------
        grayDiff.setFromPixels( tspsProcessor->difference( grayDiff, (TrackingType) p_Settings->trackType ) );
        
        //-----------------------
        // Post-difference filters
        //-----------------------
        
        if(p_Settings->bSmooth){
            grayDiff.blur((p_Settings->smooth * 2) + 1); //needs to be an odd number
        }
        
        //highpass filter
        if(p_Settings->bHighpass){
            grayDiff.highpass(p_Settings->highpassBlur, p_Settings->highpassNoise);
        }
        
        //-----------------------
        // Track
        //-----------------------	
        differencedImage.setFromPixels( tspsProcessor->process( grayDiff ) );
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark Draw
    //---------------------------------------------------------------------------
    void PeopleTracker::draw()
    {
        draw(0,0);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::draw(int x, int y)
    {
        draw(x,y,drawMode);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::draw(int x, int y, int mode)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        // run lean + mean if we're minimized
        if (p_Settings->bMinimized) return;
        ofPushMatrix();
		ofTranslate(x, y, 0);
		// draw the incoming, the grayscale, the bg and the thresholded difference
		ofSetHexColor(0xffffff);
        
        // draw gui
        gui.draw();
        
		//draw large image
		if (activeViewIndex ==  CAMERA_SOURCE_VIEW){
			cameraView.drawLarge(activeView.x, activeView.y, activeView.width, activeView.height);		
			gui.drawQuadGui( activeView.x, activeView.y, activeView.width, activeView.height );
		} else if ( activeViewIndex == ADJUSTED_CAMERA_VIEW){
			adjustedView.drawLarge(activeView.x, activeView.y, activeView.width, activeView.height);				
		} else if ( activeViewIndex == REFERENCE_BACKGROUND_VIEW){
			bgView.drawLarge(activeView.x, activeView.y, activeView.width, activeView.height);			
		} else if ( activeViewIndex == PROCESSED_VIEW){ 
			processedView.drawLarge(activeView.x, activeView.y, activeView.width, activeView.height);
		} else if ( activeViewIndex == DATA_VIEW ){
			ofPushMatrix();
            ofTranslate(activeView.x, activeView.y);
            drawBlobs(activeView.width, activeView.height);
            ofPushMatrix();{
                ofScale( (float) activeView.width / width , (float) activeView.height / height );
                tspsProcessor->draw();
            } ofPopMatrix();
			ofPopMatrix();
			dataView.drawLarge(activeView.x, activeView.y, activeView.width, activeView.height);
		}
		
		//draw all images small
		cameraView.draw();
		adjustedView.draw();
		bgView.draw();
		processedView.draw();
		dataView.draw();	
		
		ofPushMatrix();
        ofTranslate(dataView.x, dataView.y);
        drawBlobs(dataView.width, dataView.height);
		ofPopMatrix();
		
        ofPopMatrix();
        
        //draw framerate in a box
        
        char frmrate[1024];
        sprintf(frmrate, "Frame rate: %f", ofGetFrameRate() );
        
        ofPushStyle();
        ofFill();
        ofSetColor(196,182,142);
        ofRect(cameraView.x, cameraView.y + cameraView.height + spacing*3 + 8, cameraView.width*2 + spacing, spacing*4);
        ofPopStyle();
        
        if (!bFontLoaded) ofDrawBitmapString(frmrate, cameraView.x + 10, cameraView.y + 10 + cameraView.height + spacing*5);
        else font.drawString(frmrate, (int)cameraView.x + 10, (int) (cameraView.y + 10 + cameraView.height + spacing*5));
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::drawBlobs( float drawWidth, float drawHeight){
        
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        float scaleVar = (float) drawWidth/width;
        
        ofFill();
        ofSetHexColor(0x333333);
        ofRect(0,0,drawWidth,drawHeight);
        ofSetHexColor(0xffffff);
        
        ofNoFill();
        
        ofPushMatrix();
        ofScale(scaleVar, scaleVar);
        
        // simpler way to draw contours: contourFinder.draw();
        for (int i=0; i < trackedPeople.size(); i++){
            
            //draw blobs				
            //if haarfinder is looking at these blobs, draw the area it's looking at
            Person* p = trackedPeople[i];
            
            p->draw( width, height, p_Settings->bSendOscContours, p_Settings->bDetectHaar, p_Settings->haarAreaPadding);													
        }
        ofPopMatrix();
        ofSetHexColor(0xffffff);				
        //ofDrawBitmapString("blobs and optical flow", 5, height - 5 );
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark mouse
    
    //---------------------------------------------------------------------------
    void PeopleTracker::mousePressed( ofMouseEventArgs &e )
    {
        if (isInsideRect(e.x, e.y, cameraView)){
            activeViewIndex = CAMERA_SOURCE_VIEW;
            cameraView.setActive();
            adjustedView.setActive(false);
            bgView.setActive(false);
            processedView.setActive(false);
            dataView.setActive(false);
        } else if (isInsideRect(e.x, e.y, adjustedView)){
            activeViewIndex = ADJUSTED_CAMERA_VIEW;
            adjustedView.setActive();
            cameraView.setActive(false);
            bgView.setActive(false);
            processedView.setActive(false);
            dataView.setActive(false);
        } else if (isInsideRect(e.x, e.y, bgView)){
            activeViewIndex = REFERENCE_BACKGROUND_VIEW;
            bgView.setActive();
            cameraView.setActive(false);
            adjustedView.setActive(false);
            processedView.setActive(false);
            dataView.setActive(false);
        } else if (isInsideRect(e.x, e.y, processedView)){
            activeViewIndex = PROCESSED_VIEW;
            processedView.setActive();
            cameraView.setActive(false);
            adjustedView.setActive(false);
            bgView.setActive(false);
            dataView.setActive(false);
        } else if (isInsideRect(e.x, e.y, dataView)){
            activeViewIndex = DATA_VIEW;
            dataView.setActive();
            cameraView.setActive(false);
            adjustedView.setActive(false);
            bgView.setActive(false);
            processedView.setActive(false);
        }
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::isInsideRect(float x, float y, ofRectangle rect){
        return ( x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height );
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark gui extension
    
    
    //---------------------------------------------------------------------------
    void PeopleTracker::enableGuiEvents(){
        gui.enableEvents();
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::disableGuiEvents(){
        gui.disableEvents();    
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::addSlider(string name, int* value, int min, int max){
        //forward to the gui manager
        gui.addSlider(name, value, min, max);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::addSlider(string name, float* value, float min, float max){
        gui.addSlider(name, value, min, max);	
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::addToggle(string name, bool* value){
        gui.addToggle(name, value);	
    }
    
    //---------------------------------------------------------------------------
    guiTypeButton * PeopleTracker::addExternalButton( string name, ofRectangle dimensions){
        return gui.addButton( name, dimensions );
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark accessors
    
    /**
     * simple public getter for external classes
     */
    //---------------------------------------------------------------------------
    Person* PeopleTracker::personAtIndex(int i) {
        return trackedPeople[i];
    }
    
    //---------------------------------------------------------------------------
    int PeopleTracker::totalPeople() {
        return trackedPeople.size();
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::enableHaarFeatures(bool doHaar){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bDetectHaar = doHaar;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::enableOpticalFlow(bool doOpticalFlow){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bTrackOpticalFlow = doOpticalFlow;
    }
    
    //---------------------------------------------------------------------------
    // for accessing the OSC sender whose parameters are adjusted in the GUI
    OscSender* PeopleTracker::getOSCsender() {
        return &oscClient;
    }
    
    //---------------------------------------------------------------------------
    WebSocketSender * PeopleTracker::getWebSocketServer(){
        return &webSocketServer;
    }
    
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::useKinect(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->bUseKinect;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseKinect( bool bUseKinect ){
        gui.setValueB( "USE_KINECT", bUseKinect );
        gui.update();
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bUseKinect = bUseKinect;
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark background management
    //---------------------------------------------------------------------------
    void PeopleTracker::relearnBackground()
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bLearnBackground = true;
    }
    
    //JG Disabled this feature
    //void PeopleTracker::enableBackgroundRelearnSmart(bool doSmartLearn)//auto-relearns if there are too many blobs in the scene
    //{
    //	p_Settings->bSmartLearnBackground = doSmartLearn;
    //}
    
    //---------------------------------------------------------------------------
    void PeopleTracker::enableBackgroundReleaernProgressive(bool doProgressive) //relearns over time using progessive frame averagering
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bLearnBackgroundProgressive = doProgressive;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setRelearnRate(float relearnRate)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->fLearnRate = relearnRate;
    }
    
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark image control
    //---------------------------------------------------------------------------
    void PeopleTracker::setThreshold(float thresholdAmount)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->threshold = thresholdAmount;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setMinBlobSize(float minBlobSize)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->minBlob = minBlobSize; 
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setMaxBlobSize(float maxBlobSize)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->maxBlob = maxBlobSize;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::enableSmooth(bool doSmooth)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bSmooth = doSmooth;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setSmoothAmount(int smoothAmount)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->smooth = smoothAmount;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::enableHighpass(bool doHighpass)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bHighpass = doHighpass;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setHighpassBlurAmount(int highpassBlurAmount)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->highpassBlur = highpassBlurAmount;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setHighpassNoiseAmount(int highpassNoiseAmount)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->highpassNoise = highpassNoiseAmount;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::enableAmplify(bool doAmp)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bAmplify = doAmp;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setAmplifyAmount(int amplifyAmount)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->highpassAmp = amplifyAmount;
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark filter controls
    //haar
    //---------------------------------------------------------------------------
    void PeopleTracker::setHaarExpandArea(float haarExpandAmount) //makes the haar rect +area bigger
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->haarAreaPadding = haarExpandAmount;
    }
    
    //blobs
    //---------------------------------------------------------------------------
    void PeopleTracker::enableFindHoles(bool findHoles)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bFindHoles = findHoles;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::trackDarkBlobs()
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->trackType = TRACK_DARK;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::trackLightBlobs()
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->trackType = TRACK_LIGHT;	
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setDrawMode(int mode)
    {
        drawMode = mode;
    }
    
    //---------------------------------------------------------------------------
    int PeopleTracker::getDrawMode() 
    {
        return drawMode;
    }
    
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
#pragma mark gui customization
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setActiveView( int viewIndex ){
        int oldActiveView = activeViewIndex;
        activeViewIndex = viewIndex;
        
        if (activeViewIndex == CAMERA_SOURCE_VIEW){
            cameraView.setActive();
            adjustedView.setActive(false);
            bgView.setActive(false);
            processedView.setActive(false);
            dataView.setActive(false);
        } else if (activeViewIndex == ADJUSTED_CAMERA_VIEW){
            adjustedView.setActive();
            cameraView.setActive(false);
            bgView.setActive(false);
            processedView.setActive(false);
            dataView.setActive(false);
        } else if (activeViewIndex == REFERENCE_BACKGROUND_VIEW){
            bgView.setActive();
            cameraView.setActive(false);
            adjustedView.setActive(false);
            processedView.setActive(false);
            dataView.setActive(false);
        } else if (activeViewIndex == PROCESSED_VIEW){
            processedView.setActive();		cameraView.setActive(false);
            adjustedView.setActive(false);
            bgView.setActive(false);
            dataView.setActive(false);
        } else if (activeViewIndex == DATA_VIEW){
            dataView.setActive();
            cameraView.setActive(false);
            adjustedView.setActive(false);
            bgView.setActive(false);
            processedView.setActive(false);
        } else {
            activeViewIndex = oldActiveView;
        }
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setActiveDimensions ( int actWidth, int actHeight){
        activeWidth = actWidth;
        activeHeight = actHeight;
        updateViewRectangles();
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::loadFont( string fontName, int fontSize){
        bFontLoaded = font.loadFont(fontName, fontSize);
        if (bFontLoaded){
            cameraView.setFont(&font);
            adjustedView.setFont(&font);
            bgView.setFont(&font);
            processedView.setFont(&font);
            dataView.setFont(&font);
        }
        return bFontLoaded;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setVideoGrabber(ofBaseVideo* grabber, CameraType inputType)
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->setVideoGrabber( grabber, inputType );
        if (inputType == CAMERA_VIDEOGRABBER){
            gui.enableElement( "open video settings" );
        } else if (inputType == CAMERA_KINECT || inputType == CAMERA_OPENNI || inputType == CAMERA_VIDEOFILE){
            gui.disableElement( "open video settings" );
        }
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::updateViewRectangles(){
        //build all rectangles for drawing views
        ofPoint smallView;
        smallView.x = (activeWidth - GUI_WIDTH - spacing*6)/5.f;
        smallView.y = (height*TRACKING_SCALE_FACTOR) * (smallView.x/(width*TRACKING_SCALE_FACTOR));
        
        activeView.x = GUI_WIDTH + spacing;
        activeView.y = spacing;
        activeView.width = (activeWidth - GUI_WIDTH - spacing*2);
        activeView.height = (height*TRACKING_SCALE_FACTOR)*activeView.width/(width*TRACKING_SCALE_FACTOR);
        
        cameraView.x = GUI_WIDTH + spacing;
        cameraView.y = activeView.y + activeView.height + spacing;
        cameraView.width = smallView.x;
        cameraView.height = smallView.y;
        
        adjustedView.x = cameraView.x + cameraView.width + spacing;
        adjustedView.y = cameraView.y;
        adjustedView.width = smallView.x;
        adjustedView.height = smallView.y;
        
        bgView.x = adjustedView.x + adjustedView.width + spacing;
        bgView.y = cameraView.y;
        bgView.width = smallView.x;
        bgView.height = smallView.y;
        
        processedView.x = bgView.x + bgView.width + spacing;
        processedView.y = cameraView.y;
        processedView.width = smallView.x;
        processedView.height = smallView.y;
        
        dataView.x = processedView.x + processedView.width + spacing;
        dataView.y = cameraView.y;
        dataView.width = smallView.x;
        dataView.height = smallView.y;	
        gui.drawQuadGui( activeView.x, activeView.y, activeView.width, activeView.height );
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::useVideoFile(){    
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->bUseVideoFile;
    }
    
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseVideoFile( bool bUseVideoFile ){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->bUseVideoFile = bUseVideoFile;
    }
    
    //---------------------------------------------------------------------------
    string PeopleTracker::getVideoFile(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->videoFile;    
    }
    //---------------------------------------------------------------------------
    void PeopleTracker::setVideoFile( string file ){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->videoFile = file;    
    }
    
    //---------------------------------------------------------------------------
    // for accessing which view is the current view
    bool PeopleTracker::inCameraView() {
        return cameraView.isActive();
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::inBackgroundView() {
        return bgView.isActive();
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::inDifferencingView() {
        return processedView.isActive();
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::inDataView() {
        return dataView.isActive();
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::inAdjustedView() {
        return adjustedView.isActive();
    }
    
}