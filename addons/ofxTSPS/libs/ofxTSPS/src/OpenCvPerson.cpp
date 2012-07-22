/*
 *  OpenCvPerson.cpp
 *  openTSPS
 */

#include "ofxTSPS/opencv/OpenCvPerson.h"

namespace ofxTSPS {
    OpenCvPerson::OpenCvPerson(int pid, int oid, ofxCvBlob blob) : Person(pid, oid){
        update(blob, false);
    }
    
    void OpenCvPerson::update(ofxCvBlob blob, bool dampen){
        updateCentroid(blob.centroid, true);
        updateBoundingRect(blob.boundingRect);
        updateContour(blob.pts);
        area		 = blob.area;
        Person::update();
    }    
}
