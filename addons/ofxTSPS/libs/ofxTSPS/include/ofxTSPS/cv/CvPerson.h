//
//  CvPerson.h
//  openTSPS
//
//  Created by Brett Renfer on 7/22/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#pragma once

#include "ofxTSPS/Person.h"
#include "ofxCv.h"

namespace ofxTSPS {
    class CvPerson : public Person {
    public:
        
        CvPerson();
        CvPerson(int pid, int oid, ofxCv::ContourFinder& _contourFinder);
        void update(bool dampen=false);    
        
    private:
        
        ofxCv::ContourFinder& contourFinder;
    };
}