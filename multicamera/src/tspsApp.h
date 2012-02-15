#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

/*********************************************************
    INCLUDES + DEFINES
*********************************************************/
	
    #define TSPS_HOME_PAGE "http://opentsps.com"
    #include "TSPSDelegate.h"
    #include "ofxKinect.h"
    #define MAX_CAMERAS 4

/*********************************************************
    APP
*********************************************************/


class tspsApp : public ofBaseApp {

	public:
		
		void setup();
		void update();
		void draw();
        void exit();
		
        int currentDelegate; // which one you are drawing
        vector<TSPSDelegate *> delegates;
    
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
            	
	//status bar stuff
		ofImage statusBar;
		int		drawStatus[3];
		ofImage personEnteredImage;
		ofImage personUpdatedImage;
		ofImage personLeftImage;
		ofTrueTypeFont timesBoldItalic;
	
	//other gui images
		ofImage background;   
    
};

#endif
