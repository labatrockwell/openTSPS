//
//  ofxTSPS::Constants.h
//  openTSPS
//
//  Created by Brett Renfer on 7/15/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include <cstdlib>
#include <string>

namespace ofxTSPS {
    enum TrackingType {
        TRACK_DARK, TRACK_LIGHT, TRACK_ABSOLUTE
    };
    
    enum DrawMode {
        DRAW_MODE_NORMAL,
        DRAW_MODE_GUI,
        DRAW_MODE_CAMERA_CALIBRATE,
        DRAW_MODE_FULLSCREEN
    };
    
    enum ViewType {
        CAMERA_SOURCE_VIEW,
        ADJUSTED_CAMERA_VIEW,
        REFERENCE_BACKGROUND_VIEW,
        PROCESSED_VIEW,
        DATA_VIEW
    };
    
    enum SourceType {
        CAMERA_VIDEOGRABBER,
        CAMERA_VIDEOFILE,
        CAMERA_KINECT,
        CAMERA_OPENNI,
        CAMERA_SYPHON,
        CAMERA_CUSTOM,
        CAMERA_UNDEFINED
    };
    
    //layout vars
    static int spacing      = 10;
    static int GUI_WIDTH    = 340;
    
    static std::string URL = "http://github.com/labatrockwell/openTSPS";
}