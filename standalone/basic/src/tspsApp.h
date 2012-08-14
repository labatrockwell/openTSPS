#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

/*********************************************************
    INCLUDES + DEFINES
*********************************************************/

    // TSPS core
    #include "ofxTSPS.h"
	
    #define TSPS_HOME_PAGE "http://opentsps.com"

/*********************************************************
    APP
*********************************************************/

class tspsApp : public ofBaseApp {

	public:
    
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
        
        // TSPS core: PeopleTracker + events
        
        ofxTSPS::PeopleTracker peopleTracker;
        void onPersonEntered( ofxTSPS::EventArgs & tspsEvent );
        void onPersonUpdated( ofxTSPS::EventArgs & tspsEvent );
        void onPersonWillLeave( ofxTSPS::EventArgs & tspsEvent );

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
