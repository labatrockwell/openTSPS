#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxTSPS.h"
//#define USE_KINECT true			// uncomment this to use Kinect
#define _USE_LIVE_VIDEO		// uncomment this to use a normal OF camera
								// otherwise, we'll use a movie file
//#define USE_CUSTOM_GUI		// uncomment to add a "custom" panel to the gui
								// for adding your own parameters
#define TSPS_HOME_PAGE "http://opentsps.com"

#ifdef USE_KINECT
#include "ofxKinect.h"
#endif 

class testApp : public ofBaseApp, public ofxPersonListener {

	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void personEntered( ofxTSPSPerson* newPerson, ofxTSPSScene* scene );
		void personMoved( ofxTSPSPerson* activePerson, ofxTSPSScene* scene );
		void personWillLeave( ofxTSPSPerson* leavingPerson, ofxTSPSScene* scene );
		void personUpdated( ofxTSPSPerson* updatedPerson, ofxTSPSScene* scene );
	
        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#elif USE_KINECT
			ofxKinect kinect;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif
	
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
	
	//example test values
	int itestValue;
	float ftestValue;
	bool btestValue;
	
};

#endif
