//
//  SwipeDetector.cpp
//  openTSPS
//
//  Created by rockwell on 10/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SwipeDetector.h"

SwipeDetector::SwipeDetector(){
    detector = new XnVSwipeDetector( false );
    g_pSessionManager = new XnVSessionManager;
    m_pInnerFlowRouter = new XnVFlowRouter;
    m_Broadcaster.AddListener(m_pInnerFlowRouter);
}

void SwipeDetector::setup( ofxOpenNI & _openNI ){
    openNI = &_openNI;
    detector->RegisterSwipeUp(this, &callbackOnSwipeUp);
    detector->RegisterSwipeDown(this, &callbackOnSwipeDown);
    detector->RegisterSwipeLeft(this, &callbackOnSwipeLeft);
    detector->RegisterSwipeRight(this, &callbackOnSwipeRight);
    g_pSessionManager->Initialize(&openNI->getContext(), "Wave", "RaiseHand");
    g_pSessionManager->AddListener(m_pInnerFlowRouter);
    
    m_pInnerFlowRouter->SetActive(detector);
}

void SwipeDetector::update(){
	g_pSessionManager->Update(&openNI->getContext());    
}

void XN_CALLBACK_TYPE SwipeDetector::callbackOnSwipeUp(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt){
    cout << "swipe up" <<endl;
    SwipeDetector* swipeDetector = static_cast<SwipeDetector*>(pUserCxt);
    ofxSwipeEvent event;
    event.velocity.set( 0, fVelocity );
    event.angle = fAngle;
    ofNotifyEvent(swipeDetector->onSwipeUpEvent, event );
}

void XN_CALLBACK_TYPE SwipeDetector::callbackOnSwipeDown(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt){
    cout << "swipe down" <<endl;
    SwipeDetector* swipeDetector = static_cast<SwipeDetector*>(pUserCxt);
    ofxSwipeEvent event;
    event.velocity.set( 0, fVelocity );
    event.angle = fAngle;
    ofNotifyEvent(swipeDetector->onSwipeDownEvent, event);
    
}

void XN_CALLBACK_TYPE SwipeDetector::callbackOnSwipeLeft(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt){
    cout << "swipe left" <<endl;
    SwipeDetector* swipeDetector = static_cast<SwipeDetector*>(pUserCxt);
    ofxSwipeEvent event;
    event.velocity.set( fVelocity, 0 );
    event.angle = fAngle;
    ofNotifyEvent(swipeDetector->onSwipeLeftEvent, event);
}

void XN_CALLBACK_TYPE SwipeDetector::callbackOnSwipeRight(XnFloat fVelocity, XnFloat fAngle, void* pUserCxt){
    cout << "swipe right" <<endl;
    SwipeDetector* swipeDetector = static_cast<SwipeDetector*>(pUserCxt);
    ofxSwipeEvent event;
    event.velocity.set( fVelocity, 0 );
    event.angle = fAngle;
    ofNotifyEvent(swipeDetector->onSwipeRightEvent, event);
    
}