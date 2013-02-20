#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "TSPSDelegate.h"

class tspsApp : public ofBaseApp, public ofxTSPS::Delegate {

	public:
    
		void setup();
		void update();
		void draw();
		void keyPressed  (int key);
};

#endif