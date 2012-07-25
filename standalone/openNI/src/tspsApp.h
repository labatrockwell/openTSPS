#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

/*********************************************************
    INCLUDES + DEFINES
*********************************************************/

    // TSPS core
    #include "ofxTSPS.h"
    
    //include openNI TSPS Processor
    #include "ofxTSPS/openni/OpenNIProcessor.h"

    #define TSPS_HOME_PAGE "http://opentsps.com"

    enum {
        CAMERA_NOT_INITED,
        CAMERA_KINECT,
        CAMERA_VIDEOGRABBER,
        CAMERA_VIDEOFILE
    };

/*********************************************************
    APP
*********************************************************/


class tspsApp : public ofBaseApp {

	public:
		
        ~tspsApp(){ exit(); };
    
		void setup();
		void update();
		void draw();
        void exit();
		
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
        
        // TSPS events
    
    void onPersonEntered( ofxTSPS::EventArgs & tspsEvent );
        void onPersonUpdated( ofxTSPS::EventArgs & tspsEvent );
		void onPersonWillLeave( ofxTSPS::EventArgs & tspsEvent );
		int camWidth, camHeight;
        
        ofPixels niPixels;
        ofImage	grayImg;

	//status bar stuff
		ofImage statusBar;
		int		drawStatus[3];
		ofImage personEnteredImage;
		ofImage personUpdatedImage;
		ofImage personLeftImage;
		ofTrueTypeFont timesBoldItalic;
	
	//other gui images
		ofImage background;


        ofxTSPS::PeopleTracker peopleTracker;
	    
    
    
};

#endif
