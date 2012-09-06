//
//  TSPSDelegate.cpp
//  openTSPS
//
//  Created by rockwell on 2/14/12.
//  Copyright 2012 Rockwell Group. All rights reserved.
//

#include "TSPSDelegate.h"

//------------------------------------------------------------------------
TSPSDelegate::TSPSDelegate( int which ){
	camWidth = 640;
	camHeight = 480;
    peopleTracker.setup(camWidth, camHeight, "settings/settings"+ofToString(which)+".xml", which);
    peopleTracker.loadFont("fonts/times.ttf", 10);
}

//------------------------------------------------------------------------
TSPSDelegate::~TSPSDelegate(){
    exit();
}

//------------------------------------------------------------------------
void TSPSDelegate::exit(){
}

//------------------------------------------------------------------------
void TSPSDelegate::update(){
    peopleTracker.update();
}

//------------------------------------------------------------------------
void TSPSDelegate::draw(){    
	peopleTracker.draw();
}

//------------------------------------------------------------------------
void TSPSDelegate::disableEvents(){
    peopleTracker.disableGuiEvents();
}

//------------------------------------------------------------------------
void TSPSDelegate::enableEvents(){
    peopleTracker.enableGuiEvents();    
}
