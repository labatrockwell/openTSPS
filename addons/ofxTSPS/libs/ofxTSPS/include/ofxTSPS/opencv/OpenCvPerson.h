//
//  ofxTSPSCvPerson.h
//  openTSPS
//
//  Created by Brett Renfer on 7/17/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#include "ofxTSPS/person/Person.h"
#include "ofxOpenCv.h"

namespace ofxTSPS {
    class OpenCvPerson : public Person {
    public:
        
        OpenCvPerson(int pid, int oid, ofxCvBlob blob);
        OpenCvPerson();
        void update(ofxCvBlob blob, bool dampen);    
    };
}
