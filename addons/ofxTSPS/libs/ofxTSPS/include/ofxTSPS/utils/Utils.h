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
    unsigned char * inpix;
    unsigned char * outpix;
    inpix   = inputImage.getPixels();
    outpix  = outputImage.getPixels();
    
    int inW, inH, outW, outH;
    inW = inputImage.width;
    inH = inputImage.height;
    outW = outputImage.width;
    outH = outputImage.height;
    
    int bpp = 1;
    if (imageType == OF_IMAGE_COLOR){
        bpp = 3;
    } else if (imageType == OF_IMAGE_COLOR_ALPHA){
        bpp = 4;
    }
    
    for(int x=0;x<outW;x++) {
        for(int y=0;y<outH;y++) {
            float xlrp = x/(float)outW;
            float ylrp = y/(float)outH;
            int xinput = ((quad)[0].x*(1-xlrp)+(quad)[1].x*xlrp)*(1-ylrp) + ((quad)[3].x*(1-xlrp)+(quad)[2].x*xlrp)*ylrp;
            int yinput = ((quad)[0].y*(1-ylrp)+(quad)[3].y*ylrp)*(1-xlrp) + ((quad)[1].y*(1-ylrp)+(quad)[2].y*ylrp)*xlrp;
            int inIndex = (xinput + yinput*inW)*bpp;
            int outIndex = (x+y*outW)*bpp;
            memcpy((outpix+outIndex),(inpix+inIndex),sizeof(unsigned char)*bpp);
        }
    }
    outputImage.setFromPixels(outpix, outW, outH, imageType);
}
