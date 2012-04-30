/*
 *  FaceTracker.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  http://julapy.com/blog/
 *
 */

#pragma once

#include "ofxCvMain.h"
#include "ofxLABCvHaarFinder.h"
#include "ofxCvHaarTrackerItem.h"

class ofxCvHaarTracker
{

public :	

	ofxCvHaarTracker();
	~ofxCvHaarTracker();
	
	ofxLABCvHaarFinder*	setup( string haarFile );
	ofxLABCvHaarFinder*	setup( ofxLABCvHaarFinder* hf );
	
	
	void		findHaarObjects( ofxCvGrayscaleImage &image );
	void		findHaarObjects( ofxCvGrayscaleImage &image, int width, int height );
	void		findHaarObjects( ofxCvGrayscaleImage &image, ofRectangle &roi );
	bool		hasNextHaarItem();
	int			getHaarItemID();
	void		getHaarItemProperties( float *x, float *y, float *w, float *h );
	void		getHaarItemPropertiesEased( float *x, float *y, float *w, float *h );
	void		clearHaarItems();
	
	ofxLABCvHaarFinder		*haarFinder;
	
private :	
	
	vector<ofxCvHaarTrackerItem>		haarItems;
	int									haarItemIndex;
	int									haarItemIteratorIndex;
	
	int									haarItemIDCounter;
	int									haarItemIDCounterLimit;
	
};