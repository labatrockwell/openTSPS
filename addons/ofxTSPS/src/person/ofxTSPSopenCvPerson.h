//
//  ofxTSPSCvPerson.h
//  openTSPS
//
//  Created by Brett Renfer on 7/17/12.
//  Copyright (c) 2012 Rockwell Group. All rights reserved.
//

#include "ofxTSPSPerson.h"
#include "ofxOpenCv.h"

class ofxTSPSopenCvPerson : public ofxTSPSPerson {
public:
    
    ofxTSPSopenCvPerson(int pid, int oid, ofxCvBlob blob);
    ofxTSPSopenCvPerson();
    void update(ofxCvBlob blob, bool dampen);    
};
