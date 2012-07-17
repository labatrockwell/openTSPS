#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

/*********************************************************
    INCLUDES + DEFINES
*********************************************************/

    // TSPS core
    #include "ofxTSPS.h"
	
    // kinect support
    #include "ofxKinect.h"

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
    
        void onPersonEntered( ofxTSPSEventArgs & tspsEvent );
        void onPersonUpdated( ofxTSPSEventArgs & tspsEvent );
		void onPersonWillLeave( ofxTSPSEventArgs & tspsEvent );
		int camWidth, camHeight;
        
        ofPixels            niPixels;
        ofxCvGrayscaleImage	grayImg;

	//status bar stuff
		ofImage statusBar;
		int		drawStatus[3];
		ofImage personEnteredImage;
		ofImage personUpdatedImage;
		ofImage personLeftImage;
		ofTrueTypeFont timesBoldItalic;
	
	//other gui images
		ofImage background;


        ofxTSPSPeopleTracker peopleTracker;
	    
    
    
};

#endif
