//
//  TSPSDelegate.h
//  openTSPS
//
//  Created by Brett Renfer on 2/14/12.
//  Copyright 2012 Rockwell Group. All rights reserved.
//
//  Sets up and manages a separate copy of TSPS

#pragma once

#include "ofMain.h"
#include "ofxTSPS.h"

class TSPSDelegate{
public:
    // setup which delegate + optionally force using a file instead of live input
    TSPSDelegate( int which = 0 );
    ~TSPSDelegate();
    
    void update();
    void draw();
    void exit();
    
    void disableEvents();
    void enableEvents();
    
    ofImage		grayImg;
    
protected:
    ofxTSPS::PeopleTracker peopleTracker;
    
    int     cameraIndex;
    int camWidth, camHeight;   
};