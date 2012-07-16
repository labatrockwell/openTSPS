//
//  ofxTSPSConstants.h
//  openTSPS
//
//  Created by Brett Renfer on 7/15/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

enum TSPSTrackingType {
	TRACK_DARK, TRACK_LIGHT, TRACK_ABSOLUTE
};

enum TSPSDrawMode {
    DRAW_MODE_NORMAL,
    DRAW_MODE_GUI,
    DRAW_MODE_CAMERA_CALIBRATE,
    DRAW_MODE_FULLSCREEN
};

enum TSPSViewType {
    CAMERA_SOURCE_VIEW,
    ADJUSTED_CAMERA_VIEW,
    REFERENCE_BACKGROUND_VIEW,
    PROCESSED_VIEW,
    DATA_VIEW
};

enum TSPSCameraType {
    TSPS_CAMERA_KINECT,
    TSPS_CAMERA_VIDEOGRABBER,
    TSPS_CAMERA_VIDEOFILE,
    TSPS_CAMERA_CUSTOM
};

//layout vars
static int spacing      = 10;
static int GUI_WIDTH    = 340;

#ifndef TSPS_GOOGLE_PAGE
#define TSPS_GOOGLE_PAGE "http://github.com/labatrockwell/openTSPS"
#endif