#ifndef _COORD_WARPING_H
#define _COORD_WARPING_H

/*************************************************************
 I'M NOT SURE WHERE THIS IS FROM... IT WAS IN  AN OLDER
 OF VERSION OF 00573 WITH POCO...
*************************************************************/

#include "ofMain.h"

//we use openCV to calculate our transform matrix
#include "ofxCvConstants.h"
#include "ofxCvContourFinder.h"

class ofxCvCoordWarping{
public:
	
	//---------------------------
	ofxCvCoordWarping();
	
	//first calculate the matrix
	//do this only when it changes - saves cpu!
	void calculateMatrix(ofPoint src[4], ofPoint dst[4]);
	
	//returns true if the point is inside the src quad
	bool bInQuad(ofPoint pt);
	
	//then when ever you need to warp the coords call this
	ofPoint transform(float xIn, float yIn);
	
protected:
	
	ofPoint srcQuad[4];
	ofPoint dstQuad[4];
	
	CvPoint2D32f cvsrc[4];
	CvPoint2D32f cvdst[4];
	CvMat *translate;
	
};

#endif