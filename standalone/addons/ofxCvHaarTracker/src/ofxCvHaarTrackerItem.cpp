/*
 *  FaceItem.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  http://julapy.com/blog/
 *
 */

#include "ofMain.h"
#include "ofxCvHaarTrackerItem.h"

ofxCvHaarTrackerItem :: ofxCvHaarTrackerItem()
{
	itemID			= 0;
	
	isMatch			= true;
	
	isIdle			= false;
	idleCount		= 0;
	idleCountLimit	= 20;
	
	rectBufferCount	= 0;
	rectBufferMin	= 7;
	rectBufferLimit	= 20;
	
	centreDistanceLimit	= 50;
}

ofxCvHaarTrackerItem :: ~ofxCvHaarTrackerItem()
{
	clearRectBuffer();
}

void ofxCvHaarTrackerItem :: setID( int hid )
{
	itemID = hid;
}

int ofxCvHaarTrackerItem :: getID ( )
{
	return itemID;
}

void ofxCvHaarTrackerItem :: set( float x, float y, float w, float h )
{
	int i;
	
	for( i=0; i<rectBufferLimit; i++ )
	{
		rectBuffer.push_back( ofRectangle() );
		rectBuffer.back().x			= x;
		rectBuffer.back().y			= y;
		rectBuffer.back().width		= w;
		rectBuffer.back().height	= h;
	}
	
	rect.x		= x;
	rect.y		= y;
	rect.width	= w;
	rect.height = h;

	rectEase.x		= x;
	rectEase.y		= y;
	rectEase.width	= w;
	rectEase.height = h;
}

void ofxCvHaarTrackerItem :: add( float x, float y, float w, float h )
{
	rectBuffer.erase( rectBuffer.begin() );
	rectBuffer.push_back( ofRectangle() );
	rectBuffer.back().x			= x;
	rectBuffer.back().y			= y;
	rectBuffer.back().width		= w;
	rectBuffer.back().height	= h;
	
	rect.x		= x;
	rect.y		= y;
	rect.width	= w;
	rect.height = h;
}

bool ofxCvHaarTrackerItem :: checkItem( float x, float y, float w, float h )
{
	isMatch = checkAgainstCentreDistance( x + w / 2.0, y + h / 2.0 );
	
	return isMatch;
}

bool ofxCvHaarTrackerItem :: checkAgainstCentreDistance ( float cx, float cy )
{
	float dx, dy;
	float len;
	
	dx = ( rect.x + rect.width / 2.0 ) - cx;
	dy = ( rect.y + rect.height / 2.0 ) - cy;

	len = sqrt( dx * dx + dy * dy );
	
	return ( len < centreDistanceLimit );
}

bool ofxCvHaarTrackerItem :: hasBeenMatched ()
{
	return isMatch;
}

void ofxCvHaarTrackerItem :: update()
{
	if( isMatch )
	{
		if( rectBufferCount < rectBufferLimit )
		{
			++rectBufferCount;
		}
	}
	else
	{
		++idleCount;
		
		if( idleCount >= idleCountLimit )
		{
			isIdle = true;
		}
	}
	
	isMatch = false;
}

void ofxCvHaarTrackerItem :: easeItemPosition ( float easeScale )
{
	rectEase.x += ( rect.x - rectEase.x ) * easeScale;
	rectEase.y += ( rect.y - rectEase.y ) * easeScale;
}

void ofxCvHaarTrackerItem :: easeItemSize ( float easeScale )
{
	rectEase.width += ( rect.width - rectEase.width ) * easeScale;
	rectEase.height += ( rect.height - rectEase.height ) * easeScale;
}

bool ofxCvHaarTrackerItem :: isItemIdle ()
{
	return isIdle;
}

bool ofxCvHaarTrackerItem :: isValid()
{
	return ( rectBufferCount > rectBufferMin );
}

void ofxCvHaarTrackerItem :: clearRectBuffer ()
{
	int i;
	
	for( i=0; i<rectBuffer.size(); i++ )
	{
		rectBuffer.erase( rectBuffer.begin() + i );
	}
}