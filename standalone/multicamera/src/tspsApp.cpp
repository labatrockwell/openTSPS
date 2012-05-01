#include "tspsApp.h"

//--------------------------------------------------------------
void tspsApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(223, 212, 190);
	
    // get number of cameras
    ofxKinect kTester;
    kTester.init();
    
    int numKinects = kTester.numAvailableDevices();
    
    // do we have 1 or more Kinects? set them up
    // NOTE: openCamera will first lookif it's supposed to be a live video
    
    if (numKinects >= 1){
        kTester.clear();
        
        for (int i=0; i<numKinects; i++){
            delegates.push_back( new TSPSDelegate(i) );
            delegates.back()->disableEvents();            
            bool bConnectedSuccessfully = delegates.back()->openCamera( i, true );
            /*if ( !bConnectedSuccessfully ){
                ofLog( OF_LOG_ERROR, "Kinect "+ofToString(i)+" failed, aborting setup!");
                TSPSDelegate * d = delegates.back();
                delegates.erase( delegates.begin() + delegates.size() - 1 );
                delete d;
                break;
            }*/
        }
        
        // add disabled delegates
        for ( int i=numKinects; i<MAX_CAMERAS; i++){
            delegates.push_back( new TSPSDelegate(i) );
            delegates.back()->disableEvents();            
        }
    } else {
        kTester.clear();
        
        ofVideoGrabber v; v.listDevices();
        
        // is there a smart way to count avail cameras?
        // for now: trying up to 4
        for (int i=0; i<MAX_CAMERAS; i++){            
            delegates.push_back( new TSPSDelegate(i) );
            delegates.back()->disableEvents();
            bool bConnectedSuccessfully = delegates.back()->openCamera( i, false );
            /*if ( !bConnectedSuccessfully ){
                TSPSDelegate * d = delegates.back();
                delegates.erase( delegates.begin() + delegates.size() - 1 );
                delete d;
                //break;
            }*/
        }
    }
        
    // which delegate is getting drawn
    currentDelegate = 0;
    if ( delegates.size() > 0 ){
        delegates[currentDelegate]->enableEvents();        
    }
    
    // add buttons for switching between cameras
    ofRectangle dimensions = ofRectangle( 105, 10, 50, buttonHeight);
    
    for ( int i=0; i<MAX_CAMERAS; i++){
        string name = "C:"+ofToString( i+1 );
        guiTypeButton * btn = new guiTypeButton();
        btn->setup( name, dimensions.width, dimensions.height );
        btn->setPosition( dimensions.x, dimensions.y );
        if ( delegates.size() < i || !delegates[i]->isOpen() ){
            //btn->disable();
        }
        btn->setBackgroundSelectColor(0,168,156);
        btn->setBackgroundColor(0,84,78);
        ofAddListener(btn->buttonPressed, this, &tspsApp::onButtonPressed );
        buttons.insert( pair<string, guiTypeButton*>( name, btn ) );
        
        if ( i == currentDelegate ){
            btn->setSelected();
        } else {
            btn->setNormal();            
        }
                       
        dimensions.x += 10 + dimensions.width;
    }
        
	//load GUI / interface images

	personEnteredImage.loadImage("graphic/triggers/PersonEntered_Active.png");
	personUpdatedImage.loadImage("graphic/triggers/PersonUpdated_Active.png");
	personLeftImage.loadImage("graphic/triggers/PersonLeft_Active.png");
	statusBar.loadImage("graphic/bottomBar.png");
	background.loadImage("graphic/background.png");
	timesBoldItalic.loadFont("fonts/timesbi.ttf", 16);
    
	drawStatus[0] = 0;
	drawStatus[1] = 0;
	drawStatus[2] = 0;
}

//--------------------------------------------------------------
void tspsApp::update(){
    for (int i=0; i<delegates.size(); i++){
        delegates[i]->update();
    }
}

//--------------------------------------------------------------
void tspsApp::draw(){
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	ofPushStyle();
	background.draw(0,0);
	ofPopStyle();

	//draw status bar stuff

	statusBar.draw(0,700);//ofGetHeight()-statusBar.height);
	if (drawStatus[0] > 0){
		drawStatus[0]--;
		personEnteredImage.draw(397,728);
	}
	if (drawStatus[1] > 0){
		drawStatus[1]--;
		personUpdatedImage.draw(533,728);
	}
	if (drawStatus[2] > 0){
		drawStatus[2]--;
		personLeftImage.draw(666,728);
	}
    
    if ( delegates.size() > 0 ){
        delegates[currentDelegate]->draw();
    }
    
    // draw custom buttons
    map<std::string, guiTypeButton*>::iterator it;
    for( it=buttons.begin(); it!=buttons.end(); it++ ){
        it->second->render();
    }
}

//--------------------------------------------------------------
void tspsApp::exit(){
    for (int i=0; i<delegates.size(); i++){
        delegates[i]->exit();
    }
}

//--------------------------------------------------------------
void tspsApp::keyPressed  (int key){

	switch (key){
		case ' ':{
			//peopleTracker.relearnBackground();
		} break;
		case 'f':{
			ofToggleFullscreen();
		} break;
	}
}

//--------------------------------------------------------------
void tspsApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void tspsApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void tspsApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void tspsApp::mouseReleased(int x, int y, int button){
    map<std::string, guiTypeButton*>::iterator it;
    for( it=buttons.begin(); it!=buttons.end(); it++ ){
        //if ( it->second->enabled){
            it->second->checkHit( x, y, button );                    
        //}
    }
}

//--------------------------------------------------------------
void tspsApp::onButtonPressed( string & button ){
    for (int i=0; i<MAX_CAMERAS; i++){
        string name = "C:"+ofToString( i+1 );
        if ( button == name ){
            // does this delegate exist?
            if ( i < delegates.size() ){
                if ( delegates.size() > 0 ){
                    delegates[currentDelegate]->disableEvents();
                }
                currentDelegate = i;
                if ( delegates.size() > 0 ){
                    delegates[currentDelegate]->enableEvents();
                }
            }
            cout<<"selected "<<button<<endl;
            buttons[ name ]->setSelected();
        } else {
            buttons[ name ]->setNormal();
        }
    }
    
}

//--------------------------------------------------------------
void tspsApp::windowResized(int w, int h){}
