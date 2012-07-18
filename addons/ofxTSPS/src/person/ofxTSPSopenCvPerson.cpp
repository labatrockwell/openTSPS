//
//  ofxTSPSopenCvPerson.cpp
//  openTSPS
//
//  Created by Brett Renfer on 7/17/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#include "ofxTSPSopenCvPerson.h"

ofxTSPSopenCvPerson::ofxTSPSopenCvPerson(int pid, int oid, ofxCvBlob blob) : ofxTSPSPerson(pid, oid){
	update(blob, false);
}

void ofxTSPSopenCvPerson::update(ofxCvBlob blob, bool dampen)
{
	updateCentroid(blob.centroid, true);
    updateBoundingRect(blob.boundingRect);
    updateContour(blob.pts);
	area		 = blob.area;
    ofxTSPSPerson::update();
}
