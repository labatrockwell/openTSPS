/*
 *  FaceTracker.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  http://julapy.com/blog/
 *
 */

#include "ofxCvHaarTracker.h"

ofxCvHaarTracker :: ofxCvHaarTracker()
{
	haarItemIndex			= 0;
	haarItemIteratorIndex	= 0;
	
	haarItemIDCounter		= 10;
	haarItemIDCounterLimit	= 99;
}

ofxCvHaarTracker :: ~ofxCvHaarTracker()
{
	
}

ofxLABCvHaarFinder* ofxCvHaarTracker :: setup( string haarFile )
{
	haarFinder = new ofxLABCvHaarFinder();
	haarFinder->setup( haarFile );
	return haarFinder;
}

ofxLABCvHaarFinder* ofxCvHaarTracker :: setup( ofxLABCvHaarFinder* hf )
{
	haarFinder = hf;
	return haarFinder;
}

void ofxCvHaarTracker :: findHaarObjects( ofxCvGrayscaleImage &image )
{
	haarFinder->findHaarObjects( image );
	
	for( int i=0; i<haarFinder->blobs.size(); i++ )
	{
		float x		= haarFinder->blobs[i].boundingRect.x;
		float y		= haarFinder->blobs[i].boundingRect.y;
		float w		= haarFinder->blobs[i].boundingRect.width;
		float h		= haarFinder->blobs[i].boundingRect.height;
		
		bool isMatch = false;
		
		for( int j=0; j<haarItems.size(); j++ )
		{
			if( isMatch )
			{
				break;
			}
				
			if( !haarItems[ j ].hasBeenMatched() )
			{
				isMatch = haarItems[ j ].checkItem( x, y, w, h );
				
				if( isMatch )
				{
					haarItems[ j ].add( x, y, w, h );
				}
			}
		}
		
		if( !isMatch )
		{
			haarItems.push_back( ofxCvHaarTrackerItem() );
			haarItems.back().set( x, y, w, h );
			haarItems.back().setID( haarItemIDCounter );
		}
		
		++haarItemIDCounter;
		if( haarItemIDCounter > haarItemIDCounterLimit )
		{
			haarItemIDCounter = 10;
		}
	}
	
	for( int j=0; j<haarItems.size(); j++ )
	{
		haarItems[ j ].update();
		haarItems[ j ].easeItemPosition( 0.7 );
		haarItems[ j ].easeItemSize( 0.3 );
		
		if( haarItems[ j ].isItemIdle() )
		{
			haarItems.erase( haarItems.begin() + j );
		}
	}
    
    // update vector of current haar rects
    haarRects.clear();
    float x, y, w, h;
    while(hasNextHaarItem()){
        getHaarItemPropertiesEased( &x, &y, &w, &h );
        haarRects.push_back( ofRectangle(x,y,w,h) );
    }
}


void ofxCvHaarTracker :: findHaarObjects( ofxCvGrayscaleImage &image, int width, int height )
{
	haarFinder->findHaarObjects( image, width, height );
	
	for( int i=0; i<haarFinder->blobs.size(); i++ )
	{
		float x		= haarFinder->blobs[i].boundingRect.x;
		float y		= haarFinder->blobs[i].boundingRect.y;
		float w		= haarFinder->blobs[i].boundingRect.width;
		float h		= haarFinder->blobs[i].boundingRect.height;
		
		bool isMatch = false;
		
		for( int j=0; j<haarItems.size(); j++ )
		{
			if( isMatch )
			{
				break;
			}
			
			if( !haarItems[ j ].hasBeenMatched() )
			{
				isMatch = haarItems[ j ].checkItem( x, y, w, h );
				
				if( isMatch )
				{
					haarItems[ j ].add( x, y, w, h );
				}
			}
		}
		
		if( !isMatch )
		{
			haarItems.push_back( ofxCvHaarTrackerItem() );
			haarItems.back().set( x, y, w, h );
			haarItems.back().setID( haarItemIDCounter );
		}
		
		++haarItemIDCounter;
		if( haarItemIDCounter > haarItemIDCounterLimit )
		{
			haarItemIDCounter = 10;
		}
	}
	
	for( int j=0; j<haarItems.size(); j++ )
	{
		haarItems[ j ].update();
		haarItems[ j ].easeItemPosition( 0.7 );
		haarItems[ j ].easeItemSize( 0.3 );
		
		if( haarItems[ j ].isItemIdle() )
		{
			haarItems.erase( haarItems.begin() + j );
		}
	}
    
    // update vector of current haar rects
    haarRects.clear();
    float x, y, w, h;
    while(hasNextHaarItem()){
        getHaarItemPropertiesEased( &x, &y, &w, &h );
        haarRects.push_back( ofRectangle(x,y,w,h) );
    }
}


void ofxCvHaarTracker :: findHaarObjects( ofxCvGrayscaleImage &image, ofRectangle &roi ){
	haarFinder->findHaarObjects( image, roi );
	
	for( int i=0; i<haarFinder->blobs.size(); i++ )
	{
		float x		= haarFinder->blobs[i].boundingRect.x;
		float y		= haarFinder->blobs[i].boundingRect.y;
		float w		= haarFinder->blobs[i].boundingRect.width;
		float h		= haarFinder->blobs[i].boundingRect.height;
		
		bool isMatch = false;
		
		for( int j=0; j<haarItems.size(); j++ )
		{
			if( isMatch )
			{
				break;
			}
			
			if( !haarItems[ j ].hasBeenMatched() )
			{
				isMatch = haarItems[ j ].checkItem( x, y, w, h );
				
				if( isMatch )
				{
					haarItems[ j ].add( x, y, w, h );
				}
			}
		}
		
		if( !isMatch )
		{
			haarItems.push_back( ofxCvHaarTrackerItem() );
			haarItems.back().set( x, y, w, h );
			haarItems.back().setID( haarItemIDCounter );
		}
		
		++haarItemIDCounter;
		if( haarItemIDCounter > haarItemIDCounterLimit )
		{
			haarItemIDCounter = 10;
		}
	}
	
	for( int j=0; j<haarItems.size(); j++ )
	{
		haarItems[ j ].update();
		haarItems[ j ].easeItemPosition( 0.7 );
		haarItems[ j ].easeItemSize( 0.3 );
		
		if( haarItems[ j ].isItemIdle() )
		{
			haarItems.erase( haarItems.begin() + j );
		}
	}
    
    // update vector of current haar rects
    haarRects.clear();
    float x, y, w, h;
    while(hasNextHaarItem()){
        getHaarItemPropertiesEased( &x, &y, &w, &h );
        haarRects.push_back( ofRectangle(x,y,w,h) );
    }
};

bool ofxCvHaarTracker :: hasNextHaarItem ()
{
	bool b;
	
	b = true;
	
	while( haarItemIteratorIndex < haarItems.size() && !haarItems[ haarItemIteratorIndex ].isValid() )
	{
		++haarItemIteratorIndex;
	}
	
	b = ( haarItemIteratorIndex < haarItems.size() );
	
	if( b )
	{
		haarItemIndex = haarItemIteratorIndex;
		++haarItemIteratorIndex;
	}
	else
	{
		haarItemIndex			= 0;
		haarItemIteratorIndex	= 0;
	}
	
	return b;
}

int ofxCvHaarTracker :: getHaarItemID ()
{
	return haarItems[ haarItemIndex ].getID();
}

void ofxCvHaarTracker :: getHaarItemProperties( float *x, float *y, float *w, float *h )
{
	*x	= haarItems[ haarItemIndex ].rect.x;
	*y	= haarItems[ haarItemIndex ].rect.y;
	*w	= haarItems[ haarItemIndex ].rect.width;
	*h	= haarItems[ haarItemIndex ].rect.height;
}

void ofxCvHaarTracker :: getHaarItemPropertiesEased( float *x, float *y, float *w, float *h )
{
	*x	= haarItems[ haarItemIndex ].rectEase.x;
	*y	= haarItems[ haarItemIndex ].rectEase.y;
	*w	= haarItems[ haarItemIndex ].rectEase.width;
	*h	= haarItems[ haarItemIndex ].rectEase.height;
}

void ofxCvHaarTracker :: clearHaarItems ()
{
	haarItemIteratorIndex = 0;
	haarItems.clear();
}
