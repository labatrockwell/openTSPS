//
//  SwipeDown.h
//  openTSPS
//
//  Created by rockwell on 10/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofEvents.h"
#include "ofxOpenNI.h"
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnVHandPointContext.h>
#include <XnVSessionManager.h>
#include <XnVFlowRouter.h>
#include <XnVSwipeDetector.h>
#include <XnVSelectableSlider1D.h>
#include <XnVSteadyDetector.h>
#include <XnVBroadcaster.h>
#include <XnVPushDetector.h>

class ofxSwipeEvent
{
public:
    
    ofPoint velocity;
    float   angle;    
};

class SwipeDetector
{
public:
    
    SwipeDetector();
    
    void setup( ofxOpenNI & _openNI );
    static void XN_CALLBACK_TYPE callbackOnSwipeUp(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt);
    static void XN_CALLBACK_TYPE callbackOnSwipeDown(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt);
    static void XN_CALLBACK_TYPE callbackOnSwipeLeft(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt);
    static void XN_CALLBACK_TYPE callbackOnSwipeRight(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt);
    
    void update();
    
    ofEvent <ofxSwipeEvent> onSwipeUpEvent;
    ofEvent <ofxSwipeEvent> onSwipeDownEvent;
    ofEvent <ofxSwipeEvent> onSwipeLeftEvent;
    ofEvent <ofxSwipeEvent> onSwipeRightEvent;
    
private:
    
    ofxOpenNI *         openNI;
    XnVSwipeDetector *  detector;
	XnVFlowRouter*      m_pInnerFlowRouter;
	XnVSteadyDetector*  m_pSteadyDetector;
	XnVBroadcaster      m_Broadcaster;
    XnVSessionManager* g_pSessionManager;
};

