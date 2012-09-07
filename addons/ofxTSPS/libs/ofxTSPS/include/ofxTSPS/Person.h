/***************************************************************************
 *
 *  ofxTSPS::Person.h
 *  Rockwell LAB + IDEO LAB peopleVision project
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are 
 * met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer as
 * the first lines of this file unmodified.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NUI GROUP ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BEN WOODHOUSE BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 * Web: http://code.google.com/p/peoplevision/
 *
 * (...)
 *
 ***************************************************************************/

#ifndef OFX_PERSON_OBJECT
#define OFX_PERSON_OBJECT

#include "ofxOsc.h"

namespace ofxTSPS {
    class Person {
	public: 
		Person( int pid, int oid );
		~Person();
		
        virtual void update();
        virtual void updateBoundingRect( ofRectangle _rect );
        virtual void updateCentroid( ofPoint _centroid, bool dampen );
        virtual void updateContour( ofPolyline _contour );
        
        virtual void draw( int cameraWidth, int cameraHeight, bool bSendContours=false, bool bSendHaar=false, float haarPadding=0 );
        
		//this can be a pointer to whatever you want to store in this person
		void* customAttributes;
        
		virtual void setHaarRect(ofRectangle _haarRect);
		virtual bool hasHaarRect();
		virtual ofRectangle getHaarRect();
		virtual void noHaarThisFrame();
        
		ofPoint opticalFlowVectorAccumulation;
        
		int pid; //persistent id from frame to frame
		int oid; //ordered id, used for TUIO messaging
		int age; //how many frames has this person been in the system                
		
		ofPoint centroid; //center of mass of the person
		ofRectangle boundingRect; //enclosing area
		ofPolyline contour; //shape contour
		ofPolyline simpleContour; //simplified shape contour
		ofPoint velocity; //most recent movement of centroid
		float area; //area as a scalar size
        
        float depth; // raw depth from kinect
        ofPoint highest;  // highest point in a blob (brightest pixel, will really only work correctly with kinect)
        ofPoint lowest;  // lowest point in a blob (dark pixel, will really only work correctly with kinect)
		
		//normalized accessors for use in TUIO communication
		ofRectangle getBoundingRectNormalized(float videoWidth, float videoHeight);
		ofRectangle getHaarRectNormalized(float videoWidth, float videoHeight);
		ofPoint getCentroidNormalized(float videoWidth, float videoHeight);
		ofPoint getHaarCentroidNormalized(float videoWidth, float videoHeight);
        
        // get JSON        
        virtual string getJSON( string type, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours=false, string append="" );
        virtual string getString( ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours=false ){ return ""; };
        
        // get OSC message(s)    
        virtual vector<ofxOscMessage> getOSCMessages( string type, bool bUseLegacy, int cameraWidth, int cameraHeight, bool sendContours );
        
	protected:
		
		bool hasHaar;
		ofRectangle haarRect;
		int framesWithoutHaar; //how long have we gone without seeing Haar item
		
    };
}

#endif