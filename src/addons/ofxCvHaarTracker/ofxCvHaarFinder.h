#pragma once

/*
Originally written by charli_e, adapted from code by stefanix.
Completely rewritten by kyle mcdonald. opencv hack by theo watson.

See the discussion for more info:
http://www.openframeworks.cc/forum/viewtopic.php?f=10&t=2006
*/

#include "ofMain.h"
#include "ofxOpenCv.h"

#define HAAR_HACK
// http://www.comp.leeds.ac.uk/vision/opencv/opencvref_cv.html#decl_cvHaarDetectObjects
#define HAAR_DEFAULT_MIN_SIZE 0

#ifndef CV_HAAR_FIND_MODE
#define CV_HAAR_FIND_MODE CV_HAAR_DO_CANNY_PRUNING
#endif

class ofxCvHaarFinder {
public:
	vector<ofxCvBlob> blobs;

	ofxCvHaarFinder();
	ofxCvHaarFinder(const ofxCvHaarFinder& finder);
	~ofxCvHaarFinder();

	int findHaarObjects(ofImage& input,
		int minWidth = HAAR_DEFAULT_MIN_SIZE, int minHeight = HAAR_DEFAULT_MIN_SIZE);
	int findHaarObjects(ofxCvGrayscaleImage& input,
		int minWidth = HAAR_DEFAULT_MIN_SIZE, int minHeight = HAAR_DEFAULT_MIN_SIZE);
	int findHaarObjects(ofxCvGrayscaleImage& input, ofRectangle& roi,
		int minWidth = HAAR_DEFAULT_MIN_SIZE, int minHeight = HAAR_DEFAULT_MIN_SIZE);
	int findHaarObjects(ofxCvGrayscaleImage&, int x, int y, int w, int h,
		int minWidth = HAAR_DEFAULT_MIN_SIZE, int minHeight = HAAR_DEFAULT_MIN_SIZE);

	void setup(string haarFile);
	string getHaarFile();
	bool ready();
	void setScaleHaar(float scaleHaar);
	void setNeighbors(unsigned neighbors);

	float getWidth();
	float getHeight();

protected:
	CvHaarClassifierCascade* cascade;
	string haarFile;
	ofxCvGrayscaleImage img;
	float scaleHaar;
	unsigned neighbors;
};
