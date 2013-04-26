//
//  TSPSDelegate.cpp
//  openTSPS
//
//  Created by BRenfer on 1/14/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "TSPSDelegate.h"

namespace ofxTSPS {
    
    //--------------------------------------------------------------
    Delegate::Delegate( int _id ){
        id = _id;
        settingsFile = "settings/settings"+ofToString(id)+".xml";
    }
    
    //--------------------------------------------------------------
    void Delegate::setSettingsFile( string filename ){
        settingsFile = filename;
    }
    
    //--------------------------------------------------------------
    string Delegate::getSettingsFile(){
        return settingsFile;
    }
    
    //--------------------------------------------------------------
    void Delegate::setup(){
        peopleTracker.setup(0,0,settingsFile,id);
        peopleTracker.loadFont("fonts/times.ttf", 10);
        ofxAddTSPSListeners(this);
        
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
    void Delegate::update(){
        peopleTracker.update();
    }
    
    
    //--------------------------------------------------------------
    ofxTSPS::PeopleTracker * Delegate::getPeopleTracker(){
        return &peopleTracker;
    }

    //------------------------------------------------------------------------
    void Delegate::disableEvents(){
        peopleTracker.disableGuiEvents();
    }
    
    //------------------------------------------------------------------------
    void Delegate::enableEvents(){
        peopleTracker.enableGuiEvents();
    }
    
    //--------------------------------------------------------------
    //delegate methods for people entering and exiting
    void Delegate::onPersonEntered( const void * p_peopleTracker, ofxTSPS::EventArgs & tspsEvent ){
        // make sure its from this delegate (for >1 camera setup)
        if ( (ofxTSPS::PeopleTracker *) p_peopleTracker != &peopleTracker ) return;
        
        //do something with them
        ofLog(OF_LOG_VERBOSE, "person %d of size %f entered!\n", tspsEvent.person->pid, tspsEvent.person->area);
        drawStatus[0] = 10;
        
        // call virtual event
        onPersonEntered(tspsEvent.person, tspsEvent.scene);
    }
    
    //--------------------------------------------------------------
    void Delegate::onPersonUpdated( const void * p_peopleTracker, ofxTSPS::EventArgs & tspsEvent ){
        // make sure its from this delegate (for >1 camera setup)
        if ( (ofxTSPS::PeopleTracker *) p_peopleTracker != &peopleTracker ) return;
        
        ofLog(OF_LOG_VERBOSE, "updated %d person\n", tspsEvent.person->pid);
        drawStatus[1] = 10;
        
        // call virtual event
        onPersonUpdated(tspsEvent.person, tspsEvent.scene);
    }

    //--------------------------------------------------------------
    void Delegate::onPersonWillLeave( const void * p_peopleTracker, ofxTSPS::EventArgs & tspsEvent ){
        // make sure its from this delegate (for >1 camera setup)
        if ( (ofxTSPS::PeopleTracker *) p_peopleTracker != &peopleTracker ) return;
        
        //do something to clean up
        ofLog(OF_LOG_VERBOSE, "person %d left after being %d frames in the system\n", tspsEvent.person->pid, tspsEvent.person->age);
        drawStatus[2] = 10;
        
        // call virtual event
        onPersonWillLeave(tspsEvent.person, tspsEvent.scene);
    }

    //--------------------------------------------------------------
    void Delegate::draw(){
        // bg image
        ofEnableAlphaBlending();
        ofSetHexColor(0xffffff);
        background.draw(0,0);
        
        // render TSPS interface
        peopleTracker.draw();
        
        //draw status bar stuff
        statusBar.draw(0,700);
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
        
        ofSetColor(0, 169, 157);
        timesBoldItalic.drawString(ofToString(peopleTracker.totalPeople()) ,350,740);
    }
}