#ifndef OF_CV_CONSTANTS_H
#define OF_CV_CONSTANTS_H

#include <iostream>
#include "cv.h"
#include "ofxCvBlob.h"
#include "ofxCvTrackedBlob.h"


#define  MAX_NUM_CONTOURS_TO_FIND   128  // alther this if you think you will
                                         // be looking for more....
#define  MAX_CONTOUR_LENGTH        1024  // alther this if you think your
                                         // contours will be longer than this



class ofxCvBlobListener {
  public:

    virtual void blobOn( int x, int y, int id, int order ) = 0;
    virtual void blobMoved( int x, int y, int id, int order ) = 0;
    virtual void blobOff( int x, int y, int id, int order ) = 0;

};


#endif
