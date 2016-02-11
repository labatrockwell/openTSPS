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
        currentSource = NULL;
        doRelearnBackground = false;
    }
    
    //---------------------------------------------------------------------------
    PeopleTracker::~PeopleTracker(){
        if ( hasMouseEvents ){
            hasMouseEvents = false;
            ofRemoveListener(ofEvents().mousePressed, this, &PeopleTracker::mousePressed);
        }
        if ( currentSource != NULL ){
            currentSource->closeSource();
            //delete currentSource;
        }
        tspsProcessor->exit();
//        delete tspsProcessor;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setup(int w, int h, string settingsfile, int deviceID){
        ofAddListener(ofEvents().mousePressed, this, &PeopleTracker::mousePressed);
        hasMouseEvents = true;
        
        // load defaults
        bool bDefaultsLoaded = defaults.loadFile("settings/defaults.xml");
        
        // build defaults + save if not found
        if ( !bDefaultsLoaded ){
            defaults.clear();
            defaults.addTag("defaults");
            defaults.pushTag("defaults"); {
                defaults.addValue("width", 640);
                defaults.addValue("height", 480);
                defaults.addValue("settings_file", "settings/settings.xml");
            } defaults.popTag();
            defaults.saveFile("settings/defaults.xml");
        }
        
        defaults.pushTag("defaults");
        
        width  = w == 0 ? defaults.getValue("width", 640)  : w;
        height = h == 0 ? defaults.getValue("height", 480) : h;
        
        //useful!
        blackPixels = ofPixels();
        blackPixels.allocate(width, height, 1);
        blackPixels.set(0);
        
        cameraImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        warpedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        backgroundImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        differencedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        
        cameraImage.setFromPixels(blackPixels);
        warpedImage.setFromPixels(blackPixels);
        backgroundImage.setFromPixels(blackPixels);
        differencedImage.setFromPixels(blackPixels);
        grayDiff.setFromPixels(blackPixels);
        
        //set tracker
        bOscEnabled = bTuioEnabled = bTcpEnabled = bWebSocketServerEnabled = bWebSocketClientEnabled = false;
        p_Settings = gui.getSettings();
        
        //setup gui
        gui.setup();
        gui.setupQuadGui( width, height );
        
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
        setActiveDimensions( ofGetWidth(), ofGetHeight()-68 );
        
        lastHaarFile = "";
        
        // setup source
        if ( currentSource == NULL ){
            // are we loading from a video file?
            if ( useVideoFile() ){
                setupSource( CAMERA_VIDEOFILE );
            
            #ifndef TSPS_ONLY_OPENNI
            // kinect?
            } else if ( useKinect() ){
                // are there any kinects out there?
                Kinect kinectSource;
                if ( kinectSource.available() ){
                    setupSource( CAMERA_KINECT, deviceID );
                } else {
                    ofLog(OF_LOG_ERROR, "No Kinects connected!");
                    setupSource( CAMERA_VIDEOGRABBER, deviceID );
                }
            // kinect2?
            } else if ( useKinect2() ){
                // are there any kinects out there?
                
#ifdef TARGET_OSX
                if ( true ){
#else
				Kinect2 kinectSource;
                if ( kinectSource.available() ){
#endif
                    setupSource( CAMERA_KINECT2, deviceID );
                } else {
                    ofLog(OF_LOG_ERROR, "No Kinect 2s connected!");
                    setupSource( CAMERA_VIDEOGRABBER, deviceID );
                }
            // video grabber
            #endif
            } else {
                setupSource( CAMERA_VIDEOGRABBER, deviceID );
            }
        }
        
        gui.loadSettings( settingsfile == "" ? defaults.getValue("settings_file", "settings/settings.xml") : settingsfile );
        
        // setup default processor
        if ( tspsProcessor == NULL ){
            setProcessor( new CvProcessor);
        }    
        tspsProcessor->setup( width, height, this );
        
        // setup gui based on processor capabilities
        gui.setHaarEnabled( tspsProcessor->canTrackHaar() );
        gui.setOpticalFlowEnabled( tspsProcessor->canTrackOpticalFlow() );
        
        defaults.popTag();
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::update(){
        // update settings
        updateSettings();

        // load stored background on startup?
        if ( p_Settings->bStoreBackground && !p_Settings->bStoredBgLoaded ){
            string filename = "settings/background.jpg";
            ofFile file = ofFile(filename);
            if (file.exists()) {
                ofImage storedBg = ofImage("settings/background.jpg");
                backgroundImage.setFromPixels( storedBg.getPixels() );
                tspsProcessor->captureBackground( storedBg );
            } else {
                ofLog(OF_LOG_WARNING, "No stored background file exists");
            }
            p_Settings->bStoredBgLoaded = true;
        }

        // learn background on startup?
        if ( p_Settings->bLearnBackgroundOnStartup && !p_Settings->bStartupBgCaptured && ofGetElapsedTimef() > p_Settings->captureSeconds){
            p_Settings->bStartupBgCaptured = true;

            // only capture if the setting for using stored backgrounds is not on
            if (!p_Settings->bStoreBackground) {
                doRelearnBackground = true;
            }
        }
        
        // change source?
        // 1: Kinect?
        #ifndef TSPS_ONLY_OPENNI
        if ( useKinect() && (currentSource == NULL || currentSource->getType() != CAMERA_KINECT)){
            setupSource( CAMERA_KINECT, p_Settings->cameraIndex );
        
        // 1.5: Kinect2
        } else if ( useKinect2() && (currentSource == NULL || currentSource->getType() != CAMERA_KINECT2)){
                setupSource( CAMERA_KINECT2, p_Settings->cameraIndex );
        // 2: Video Grabber?
        } else
        #endif 
        if (useVideoGrabber() && (currentSource == NULL || currentSource->getType() != CAMERA_VIDEOGRABBER)){
            setupSource( CAMERA_VIDEOGRABBER, p_Settings->cameraIndex );
        
        // 3: Video File?
        } else if ( useVideoFile() && (currentSource == NULL || currentSource->getType() != CAMERA_VIDEOFILE) ){
            setupSource( CAMERA_VIDEOFILE );
        // 3.5: New video file?
        } else if ( useVideoFile() && currentSource->getType() == CAMERA_VIDEOFILE && getVideoFile() != currentSource->getCustomData() ){
            setupSource( CAMERA_VIDEOFILE );
        }
#ifndef TSPS_KINECT2
        // 4: OpenNI
        else if ( useOpenNI() && ( currentSource == NULL || currentSource->getType() != CAMERA_OPENNI)){
            setupSource( CAMERA_OPENNI, p_Settings->cameraIndex );
        }
#endif
#ifdef TARGET_OSX
        // 5: syphon
        else if ( useSyphon() && (currentSource == NULL || currentSource->getType() != CAMERA_SYPHON) ){
            setupSource( CAMERA_SYPHON );
        }
#endif
        
        // update source
        bool bNewFrame = false;
        if ( currentSource != NULL && currentSource->isOpen() ){
//#ifdef TSPS_KINECT2
			if ( currentSource->getType() == CAMERA_KINECT2 ){
				Kinect2 * kRef = dynamic_cast<Kinect2*>(currentSource);
				kRef->nearClipping = gui.getValueF("K2_NEAR");
				kRef->farClipping = gui.getValueF("K2_FAR");
			}
//#endif

            currentSource->update();
            bNewFrame = currentSource->doProcessFrame();
        }
        
        if ( bNewFrame ){
            ofImageType currentType = currentSource->getPixels().getImageType();
            if ( currentType != OF_IMAGE_GRAYSCALE ){
                // TO-DO: this should probably be in the Processor
                // convert to grayscale and resize
                if ( currentSource->getPixels().getWidth() != width || currentSource->getPixels().getHeight() != height ){
                    ofImage tempImage;
                    tempImage.setFromPixels( currentSource->getPixels() );
                    ofxCv::convertColor( currentSource->getPixels(), tempImage, currentType == OF_IMAGE_COLOR_ALPHA ? CV_RGB2GRAY : CV_RGBA2GRAY);
                    ofxCv::resize(tempImage, cameraImage);
                } else {
                    ofxCv::convertColor( currentSource->getPixels(), cameraImage, CV_RGB2GRAY);
                }
                cameraImage.update();
            } else {
                // either resize or just copy pixels
                if ( currentSource->getPixels().getWidth() != width || currentSource->getPixels().getHeight() != height ){
                    ofImage tempImage;
                    tempImage.setFromPixels( currentSource->getPixels() );
                    ofxCv::resize(tempImage, cameraImage);
                } else {
                    cameraImage.setFromPixels( currentSource->getPixels() );
                }
            }
            
            trackPeople();
            
            if ( p_Settings->bSendScene ){
                // send scene
                if (bTuioEnabled){
                    // no scene for tuio?
                }
                
                if (bOscEnabled){
                    oscClient.sceneUpdated(scene);
                }
                
                if (bTcpEnabled){
                    tcpClient.sceneUpdated(scene);
                }
                
                if ( bWebSocketClientEnabled || bWebSocketServerEnabled ){
                    webSocketServer.sceneUpdated(scene);
                }
                
                if ( bSpacebrewEnabled ){
                    spacebrewSender.sceneUpdated(scene);
                }
            }
        }
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::update( ofBaseImage & image ){
        if ( currentSource->getPixels().getImageType() != OF_IMAGE_GRAYSCALE ){
            // TO-DO: this should probably be in the Processor
            // convert to grayscale and resize
            if ( currentSource->getPixels().getWidth() != width || currentSource->getPixels().getHeight() != height ){
                ofImage tempImage;
                tempImage.setFromPixels( currentSource->getPixels() );
                ofxCv::convertColor( currentSource->getPixels(), tempImage, CV_RGB2GRAY);
                ofxCv::resize(tempImage, cameraImage);
            } else {
                ofxCv::convertColor( currentSource->getPixels(), cameraImage, CV_RGB2GRAY);
            }
            cameraImage.update();
        } else {
            // either resize or just copy pixels
            if ( currentSource->getPixels().getWidth() != width || currentSource->getPixels().getHeight() != height ){
                ofImage tempImage;
                tempImage.setFromPixels( currentSource->getPixels() );
                ofxCv::resize(tempImage, cameraImage);
            } else {
                cameraImage.setFromPixels( currentSource->getPixels() );
            }
        }
        
        updateSettings();
        trackPeople();
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setProcessor ( Processor * _processor ){
        tspsProcessor = _processor;
    }
    
#pragma mark events
    
    void PeopleTracker::triggerCustomEvent( string eventName, string param ){
        if(bTuioEnabled){
            // TUIO doesn't care about custom events
        }
        if(bOscEnabled){
            oscClient.customEvent( eventName, param );
        }
        if(bTcpEnabled){
            tcpClient.customEvent( eventName, param );
        }
        if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.customEvent( eventName, param );
        }
    }
    
    void PeopleTracker::triggerCustomEvent( string eventName, vector<string>params ){
        if(bTuioEnabled){
            // TUIO doesn't care about custom events
        }
        if(bOscEnabled){
            oscClient.customEvent( eventName, params );
        }
        if(bTcpEnabled){
            tcpClient.customEvent( eventName, params );
        }
        if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.customEvent( eventName, params );
        }
    }
    
    void PeopleTracker::triggerCustomEvent( string eventName, map<string,string>params ){
        if(bTuioEnabled){
            // TUIO doesn't care about custom events
        }
        if(bOscEnabled){
            oscClient.customEvent( eventName, params );
        }
        if(bTcpEnabled){
            tcpClient.customEvent( eventName, params );
        }
        if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.customEvent( eventName, params );
        }
    }
    
    //---------------------------------------------------------------------------
    // events called from processor
    void PeopleTracker::personEntered( Person * person, Scene * scene ){
        ofPoint centroid = person->getCentroidNormalized(width, height);
        
        if(bTuioEnabled){
            tuioClient.cursorPressed(1.0*centroid.x/width, 1.0*centroid.y/height, person->pid );
        }
        if(bOscEnabled){
            oscClient.personEntered(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        if(bTcpEnabled){
            tcpClient.personEntered(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.personEntered(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if ( bSpacebrewEnabled ){
            spacebrewSender.personEntered(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        trackedPeople.push_back( person );
        
        // notify listeners
        EventArgs args;
        args.person = person;
        args.scene  = scene;
        ofNotifyEvent( ofxTSPS::Events().personEntered, args, this );
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::personUpdated( Person * person, Scene * scene ){
        ofPoint centroid = person->getCentroidNormalized(width, height);
        
        if (bTuioEnabled){
            ofPoint tuioCursor = person->getCentroidNormalized(width, height);
            tuioClient.cursorDragged( tuioCursor.x, tuioCursor.y, person->pid);
        }
        
        if (bOscEnabled){
            oscClient.personUpdated(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if (bTcpEnabled){
            tcpClient.personUpdated(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if ( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.personUpdated(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if ( bSpacebrewEnabled ){
            spacebrewSender.personUpdated(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        // notify listeners
        EventArgs args;
        args.person = person;
        args.scene  = scene;
        ofNotifyEvent( Events().personUpdated, args, this );
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::personWillLeave( Person * person, Scene * scene ){
        ofPoint centroid = person->getCentroidNormalized(width, height);
        if (bTuioEnabled) {
            tuioClient.cursorReleased(centroid.x, centroid.y, person->pid);
        }
        //send osc kill message if enabled
        if (bOscEnabled){
            oscClient.personWillLeave(person, centroid, width, height, p_Settings->bSendOscContours);
        };
        
        //send tcp kill message if enabled
        if(bTcpEnabled){
            tcpClient.personWillLeave(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if( bWebSocketClientEnabled || bWebSocketServerEnabled ){
            webSocketServer.personWillLeave(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        
        if ( bSpacebrewEnabled ){
            spacebrewSender.personWillLeave(person, centroid, width, height, p_Settings->bSendOscContours);
        }
        // notify listeners
        EventArgs args;
        args.person = person;
        args.scene  = scene;
        ofNotifyEvent( Events().personWillLeave, args, this );
    }
    
#pragma mark source
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setSource( Source & newSource ){
        if ( currentSource != NULL ){
            currentSource->closeSource();
        }
        currentSource = &newSource;
        
        // make settings so they don't boot what you did
        switch( currentSource->getType() ){
            case CAMERA_KINECT:
                #ifndef TSPS_ONLY_OPENNI
                setUseKinect();
                #else
                // hm
                #endif
                break;
            case CAMERA_KINECT2:
                setUseKinect2();
                break;
            case CAMERA_VIDEOFILE:
                setUseVideoFile();
                break;
            case CAMERA_VIDEOGRABBER:
                setUseVideoGrabber();
                break;
            case CAMERA_SYPHON:
#ifdef TARGET_OSX
                setUseSyphon();
#endif
                break;
#ifndef TSPS_KINECT2
            case CAMERA_OPENNI:
                setUseOpenNI();
                break;
#endif
            case CAMERA_CUSTOM:
                setUseCustomSource();
                break;
                
            case CAMERA_UNDEFINED:
                break;
        }
        
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->setSource( currentSource );
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::setupSource( SourceType type, int which ){
        if ( currentSource != NULL ){
            currentSource->closeSource();
            currentSource = NULL;
        }
        string etc = "";
        switch ( type ){
            case CAMERA_KINECT:
#ifndef TSPS_ONLY_OPENNI
#ifdef TSPS_KINECT2
                currentSource = new Kinect2();
#else
                currentSource = new Kinect();
#endif
#endif
                break;
                
            case CAMERA_KINECT2:
                currentSource = new Kinect2();
                break;
            case CAMERA_VIDEOGRABBER:
                currentSource = new VideoGrabber();
                break;
            case CAMERA_VIDEOFILE:
                // see if there is a valid video file set first
                etc = getVideoFile();
                if ( etc == ""){
                    return false;
                }
                currentSource = new VideoFile();
                break;
#ifndef TSPS_KINECT2
            case CAMERA_OPENNI:
                currentSource = new OpenNI2();
                break;
#endif
#ifdef TARGET_OSX
            case CAMERA_SYPHON:
                currentSource = new Syphon();
                break;
#endif
            default:
                break;
        }
        currentSource->setSourceIndex( which );
        bSourceSetup = currentSource->openSource( width, height, etc );
        
        // override settings (if necessary)
        switch( currentSource->getType() ){
            case CAMERA_KINECT:
#ifndef TSPS_ONLY_OPENNI
                setUseKinect(true, which);
#endif
                break;
            case CAMERA_VIDEOFILE:
                setUseVideoFile();
                break;
#ifndef TSPS_KINECT2
            case CAMERA_OPENNI:
                setUseOpenNI(true, which);
                break;
#endif
            case CAMERA_VIDEOGRABBER:
                setUseVideoGrabber(true, which);
                break;
            case CAMERA_CUSTOM:
                setUseCustomSource();
                break;
            default:
                break;
        }
        
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->setSource( currentSource );
        
        return bSourceSetup;
    }
    
#pragma mar settings
    
    //---------------------------------------------------------------------------
    void PeopleTracker::resize( int w, int h ){
        width  = w;
        height = h;
        
        cameraImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        warpedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        backgroundImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        differencedImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        grayDiff.allocate(width, height, OF_IMAGE_GRAYSCALE);
        
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
        ofLogVerbose()<<"SEND TCP TO PORT "<<port;
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        p_Settings->tcpPort = port;
        tcpClient.setup(port);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setupSpacebrew( string host )
    {
        spacebrewSender.setHost(host);
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setupWebSocketServer( int port)
    {
        ofLog(OF_LOG_VERBOSE, "SEND WEBSOCKET SERVER ON PORT "+ofToString(port));
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
        // Check defaults
        //----------------------------------------------
        
        defaults.pushTag("defaults");
        if ( p_Settings->currentXmlFile != defaults.getValue("settings_file", "")){
            defaults.setValue("settings_file", p_Settings->currentXmlFile);
            defaults.saveFile(ofToDataPath("settings/defaults.xml") );
        }
        defaults.popTag();
        
        //----------------------------------------------
        // Localize vars
        //----------------------------------------------
        
        if ( !doRelearnBackground )
            doRelearnBackground = p_Settings->bLearnBackground;
        
        //----------------------------------------------
        // Scene
        //----------------------------------------------
        
        if ( scene.getGridX() != p_Settings->sceneGridX || scene.getGridY() != p_Settings->sceneGridY ){
            scene.buildGrid(p_Settings->sceneGridX, p_Settings->sceneGridY);
        }
        
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
        
        // check to enable spacebrew
        if (p_Settings->bSendSpacebrew && !bSpacebrewEnabled ){
            bSpacebrewEnabled = true;
            spacebrewSender.enable();
        } else if (!p_Settings->bSendSpacebrew){
            bSpacebrewEnabled = false;
            spacebrewSender.disable();
        }
        
        if ( bSpacebrewEnabled ){
            spacebrewSender.setHost(p_Settings->spacebrewHost);
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
        if (p_Settings->currentPanel != p_Settings->lastCurrentPanel){
            setActiveView(p_Settings->currentPanel + 1);
            p_Settings->lastCurrentPanel = p_Settings->currentPanel;
        }
        
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
        getQuadSubImage(cameraImage, warpedImage, p_Settings->quadWarpScaled, OF_IMAGE_GRAYSCALE);	
        
        // mirror?
        if ( p_Settings->bFlipX || p_Settings->bFlipY ){
            int mode = 1;
            if ( p_Settings->bFlipX && p_Settings->bFlipY ) mode = -1;
            else if ( p_Settings->bFlipY ) mode = 0;
            
			static ofImage temp = warpedImage;
            cv::Mat src = ofxCv::toCv( warpedImage ), dst = ofxCv::toCv( warpedImage );
            cv::flip( src, dst, mode );
            ofxCv::toOf( dst, temp.getPixels() );
			warpedImage.setFromPixels( temp.getPixels() );
            warpedImage.update();
        }
        
        // invert
        if ( p_Settings->bInvert ){
            ofPixelsRef pix = warpedImage.getPixels();
            for (int i=0, len=pix.getWidth()*pix.getHeight()*pix.getNumChannels();i<len;i++){
                pix[i]=255-pix[i];
            }
            warpedImage.update();
        }
        
        // update scaled down images
        grayDiff.setFromPixels(warpedImage.getPixels());
        
        //amplify
        if(p_Settings->bAmplify){
            float scalef = p_Settings->highpassAmp / 15.0f / 128.0f;
			static ofImage temp = warpedImage;
            cv::Mat src = ofxCv::toCv( warpedImage ), dst = ofxCv::toCv( warpedImage );
            cv::multiply(src, src, dst, scalef);
            ofxCv::toOf( dst, temp.getPixels());
			warpedImage.setFromPixels( temp.getPixels() );
            warpedImage.update();
        }
        
        // set base camera image
        tspsProcessor->setCameraImage( warpedImage );
        
        //learn background
        if (doRelearnBackground){
            backgroundImage.setFromPixels(warpedImage.getPixels());
            tspsProcessor->captureBackground( warpedImage );

            if (p_Settings->bStoreBackground) {
                warpedImage.save("settings/background.jpg");
            }

            doRelearnBackground = false;
        }
        
        //progressive relearn background
        if (p_Settings->bLearnBackgroundProgressive){
            backgroundImage.setFromPixels( tspsProcessor->progressiveBackground( warpedImage, p_Settings->fLearnRate * .00001 ));
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
        
        //highpass filter
        if(p_Settings->bHighpass){
            cv::Mat mat = ofxCv::toCv(grayDiff);
            cv::Mat dst = ofxCv::toCv(grayDiff);

            //Blur Original Image
            if(p_Settings->highpassBlur > 0){
                cv::blur(mat, mat, cv::Size((p_Settings->highpassBlur * 2 )+1, (p_Settings->highpassBlur * 2 )+1));
            }
            
            int kernelSize = p_Settings->highpassKernel;
            if ( kernelSize % 2 == 0){
                kernelSize += 1;
            }
            
            mutex.lock();
            cv::Laplacian(mat, dst, mat.type(), (int) kernelSize);
            mutex.unlock();
            
            //Post-Blur  Image
            if(p_Settings->highpassBlur > 0){
                cv::blur(dst, dst, cv::Size((p_Settings->highpassNoise * 2 )+1, (p_Settings->highpassNoise * 2 )+1));
            }
            
			static ofImage temp;
            ofxCv::toOf( dst, temp.getPixels() );
			grayDiff.setFromPixels( temp.getPixels() );
            //grayDiff.update();
        }
        
        // blur filter
        if(p_Settings->bSmooth){
            cv::Mat mat = ofxCv::toCv(grayDiff);
            cv::blur(mat, mat, cv::Size((p_Settings->smooth * 2) + 1, (p_Settings->smooth * 2) + 1));
            static ofImage temp;
            ofxCv::toOf( mat, temp.getPixels() );
			grayDiff.setFromPixels( temp.getPixels() );
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
            scene.draw( activeView.width, activeView.height );
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
        ofDrawRectangle(cameraView.x, cameraView.y + cameraView.height + spacing*3 + 8, cameraView.width*2 + spacing, spacing*4);
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
        ofDrawRectangle(0,0,drawWidth,drawHeight);
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
    //---------------------------------------------------------------------------
#pragma mark background management
    //---------------------------------------------------------------------------
    void PeopleTracker::relearnBackground()
    {
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        doRelearnBackground = true;
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
        gui.setValueF( "THRESHOLD", thresholdAmount );
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
    
#pragma mark filter controls
    //---------------------------------------------------------------------------
    //haar
    //---------------------------------------------------------------------------
    //makes the haar rect +area bigger 
    void PeopleTracker::setHaarExpandArea(float haarExpandAmount) {
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
        if ( viewIndex < CAMERA_SOURCE_VIEW ) viewIndex = CAMERA_SOURCE_VIEW;
        else if ( viewIndex > DATA_VIEW ) viewIndex = DATA_VIEW;
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
        bFontLoaded = font.load(fontName, fontSize);
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
    bool PeopleTracker::useVideoGrabber(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->inputType == CAMERA_VIDEOGRABBER;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseVideoGrabber( bool bUseVideoGrabber, int deviceIndex ){
        if ( bUseVideoGrabber ){
            if (p_Settings == NULL) p_Settings = gui.getSettings();
            gui.setValueI( "SOURCE_TYPE", gui.getSourceSelectionIndex( CAMERA_VIDEOGRABBER, deviceIndex) );
            gui.update();
            p_Settings->inputType = CAMERA_VIDEOGRABBER;
        }
    }
    
#ifndef TSPS_ONLY_OPENNI
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::useKinect(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->inputType == CAMERA_KINECT;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseKinect( bool bUseKinect, int deviceIndex ){
        if ( bUseKinect ){
            gui.setValueI( "SOURCE_TYPE", gui.getSourceSelectionIndex( CAMERA_KINECT, deviceIndex) );
            gui.update();
            if (p_Settings == NULL) p_Settings = gui.getSettings();
            p_Settings->inputType = CAMERA_KINECT;
        }
    }
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::useKinect2(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->inputType == CAMERA_KINECT2;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseKinect2( bool bUseKinect, int deviceIndex ){
        if ( bUseKinect ){
            gui.setValueI( "SOURCE_TYPE", gui.getSourceSelectionIndex( CAMERA_KINECT2, deviceIndex) );
            gui.update();
            if (p_Settings == NULL) p_Settings = gui.getSettings();
            p_Settings->inputType = CAMERA_KINECT2;
        }
    }
    
#endif
#ifndef TSPS_KINECT2
    //---------------------------------------------------------------------------
    bool    PeopleTracker::useOpenNI(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->inputType == CAMERA_OPENNI;
    }
    
    //---------------------------------------------------------------------------
    void    PeopleTracker::setUseOpenNI( bool bUseOpenNI, int deviceIndex ){
        if ( bUseOpenNI ){
            gui.setValueI( "SOURCE_TYPE", gui.getSourceSelectionIndex( CAMERA_OPENNI, deviceIndex) );
            gui.update();
            if (p_Settings == NULL) p_Settings = gui.getSettings();
            p_Settings->inputType = CAMERA_OPENNI;
        }
    }
#endif
    //---------------------------------------------------------------------------
    bool PeopleTracker::useVideoFile(){    
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->inputType == CAMERA_VIDEOFILE;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseVideoFile( bool bUseVideoFile, int deviceIndex ){
        if ( bUseVideoFile ){
            gui.setValueI( "SOURCE_TYPE", gui.getSourceSelectionIndex( CAMERA_VIDEOFILE, deviceIndex) );
            gui.update();
            if (p_Settings == NULL) p_Settings = gui.getSettings();
            p_Settings->inputType = CAMERA_VIDEOFILE;            
        }
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
    
#ifdef TARGET_OSX
    //---------------------------------------------------------------------------
    bool PeopleTracker::useSyphon(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->inputType == CAMERA_SYPHON;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseSyphon( bool bUseSyphon ){
        if ( bUseSyphon ){
            gui.setValueI( "SOURCE_TYPE", CAMERA_SYPHON );
            gui.update();
            if (p_Settings == NULL) p_Settings = gui.getSettings();
            p_Settings->inputType = CAMERA_SYPHON;
        }
    }
#endif
    
    //---------------------------------------------------------------------------
    bool PeopleTracker::useCustomSource(){
        if (p_Settings == NULL) p_Settings = gui.getSettings();
        return p_Settings->inputType == CAMERA_CUSTOM;
    }
    
    //---------------------------------------------------------------------------
    void PeopleTracker::setUseCustomSource( bool bUseCustom ){
        if ( bUseCustom ){
            gui.setValueI( "SOURCE_TYPE", CAMERA_CUSTOM );
            gui.update();
            if (p_Settings == NULL) p_Settings = gui.getSettings();
            p_Settings->inputType = CAMERA_CUSTOM;
        }
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
    
    //---------------------------------------------------------------------------
    ofImage * PeopleTracker::getCameraImage(){
        return &cameraImage;
    }
    
    //---------------------------------------------------------------------------
    ofImage * PeopleTracker::getWarpedImage(){
        return &warpedImage;
    }
    
    //---------------------------------------------------------------------------
    ofImage * PeopleTracker::getBackgroundImage(){
        return &backgroundImage;
    }
    
    //---------------------------------------------------------------------------
    ofImage * PeopleTracker::getDifferencedImage(){
        return &differencedImage;
    }
    
}