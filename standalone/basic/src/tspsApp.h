#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

/*********************************************************
    INCLUDES + DEFINES
*********************************************************/

    // TSPS core
    #include "ofxTSPS.h"

    #define _USE_LIVE_VIDEO         // comment out to load a movie file
    //#define USE_CUSTOM_GUI		// uncomment to add a "custom" panel to the gui
	
    // kinect support
    #include "ofxKinect.h"

    #define TSPS_HOME_PAGE "http://opentsps.com"

    enum {
        CAMERA_NOT_INITED,
        CAMERA_KINECT,
        CAMERA_VIDEOGRABBER
    };

/*********************************************************
    APP
*********************************************************/


class tspsApp : public ofBaseApp, public ofxPersonListener {

	public:
		
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
    
		void personEntered( ofxTSPSPerson* newPerson, ofxTSPSScene* scene );
		void personMoved( ofxTSPSPerson* activePerson, ofxTSPSScene* scene );
		void personWillLeave( ofxTSPSPerson* leavingPerson, ofxTSPSScene* scene );
		void personUpdated( ofxTSPSPerson* updatedPerson, ofxTSPSScene* scene );
            
        #ifdef _USE_LIVE_VIDEO

        // ready for either live video or Kinect, will choose in the next step
        ofVideoGrabber 		vidGrabber;
        ofxKinect           kinect;

        // kinect or live video?
        bool bKinect, bKinectConnected;
        int cameraState;
    
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif
	
        void initVideoInput();
        void closeVideoInput();
    
		int camWidth, camHeight;

		ofxCvColorImage		colorImg;
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
