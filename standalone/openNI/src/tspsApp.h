#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "TSPSDelegate.h"
#include "OpenNI.h"
#include "ofxLabGui.h"
#define MAX_CAMERAS 4

class tspsApp : public ofBaseApp {
    
public:
    
    tspsApp( int numCameras = 1 );
    void setup();
    void update();
    void draw();
    
    // custom openNI source
    vector<ofxTSPS::OpenNI *> sources;
    
    int currentDelegate; // which one you are drawing
    vector<ofxTSPS::Delegate *> delegates;
    
    // buttons for switching between cameras + adding more cameras
    map<string, guiTypeButton *> buttons;
    void onButtonPressed( string & button );
    
    // normal of events
    void mouseReleased(int x, int y, int button);
};

#endif