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

        // ready for either live video or Kinect, will choose in the next step
        ofVideoGrabber 		vidGrabber;
        ofxKinect           kinect;
        ofVideoPlayer 		vidPlayer;
    
        // kinect, webcam, or video file?
        bool bKinect, bKinectConnected, bUseVideoFile;
        int cameraState;
        string videoFile;    
	
        bool initVideoInput();
        void closeVideoInput();
        bool initVideoFile();
    
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
