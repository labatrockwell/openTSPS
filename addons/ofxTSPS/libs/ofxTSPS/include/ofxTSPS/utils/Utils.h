//
//  ofxTSPS::Utils.h
//  openTSPS
//
//  Created by brenfer on 11/11/11.
//  Copyright 2011 Rockwell Group. All rights reserved.
//

// useful! from http://forum.openframeworks.cc/index.php/topic,6398.0.html

#include "ofMain.h"

static void getQuadSubImage(ofImage& inputImage, ofImage& outputImage, vector <ofPoint>& quad, ofImageType imageType) {
    if ( quad.size() < 4 ){
        ofLog( OF_LOG_ERROR, "You must pass a vector of four points to this function");
        return;
    } // weird thing that could happen...
    
    static unsigned char * inpix;
    static unsigned char * outpix;
    inpix   = inputImage.getPixels().getData();
    outpix  = outputImage.getPixels().getData();
    
    int inW, inH, outW, outH;
    inW = inputImage.getWidth();
    inH = inputImage.getHeight();
    outW = outputImage.getWidth();
    outH = outputImage.getHeight();
    
    int bpp = 1;
    if (imageType == OF_IMAGE_COLOR){
        bpp = 3;
    } else if (imageType == OF_IMAGE_COLOR_ALPHA){
        bpp = 4;
    }
    
    int xinput =0;
    int yinput = 0;
    int inIndex = 0;
    int outIndex = 0;
    
    float xlrp = 0.0;
    float ylrp = 0.0;
    
    ofPoint p1, p2, p3, p4;
    p1 = quad[0];
    p2 = quad[1];
    p3 = quad[2];
    p4 = quad[3];
    
    for(int x=0;x<outW;x++) {
        for(int y=0;y<outH;y++) {
            xlrp = x/(float)outW;
            ylrp = y/(float)outH;
            xinput = (p1.x*(1-xlrp)+p2.x*xlrp)*(1-ylrp) + (p4.x*(1-xlrp)+p3.x*xlrp)*ylrp;
            yinput = ((p1.y*(1-ylrp))+(p4.y*ylrp))*(1-xlrp) + (p2.y*(1-ylrp)+p3.y*ylrp)*xlrp;
            inIndex = (xinput + (yinput*inW))*bpp;
            outIndex = (x+y*outW)*bpp;
            memcpy((outpix+outIndex),(inpix+inIndex),sizeof(unsigned char)*bpp);
        }
    }
    outputImage.setFromPixels(outpix, outW, outH, imageType);
}

