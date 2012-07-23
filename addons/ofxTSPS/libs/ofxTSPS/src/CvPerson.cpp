//
//  CvPerson.cpp
//  openTSPS
//
//  Created by Brett Renfer on 7/22/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#include "ofxTSPS/cv/CvPerson.h"

using namespace ofxCv;
using namespace cv;

namespace ofxTSPS {
    
    CvPerson::CvPerson(int pid, int oid, ofxCv::ContourFinder& _contourFinder) :
    Person(pid, oid), contourFinder(_contourFinder){
        update();
    }
    
    void CvPerson::update(bool dampen){
        ofPoint c = toOf(contourFinder.getCentroid(oid));
        updateBoundingRect( toOf(contourFinder.getBoundingRect(oid)));
        updateCentroid( c, dampen );
        updateContour( contourFinder.getPolyline(oid) );
        area		 = contourFinder.getContourArea(oid);
        Person::update();
    }
}