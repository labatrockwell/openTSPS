//
//  OpenNI.h
//  openTSPS
//
//  Created by Brett Renfer on 9/4/12.
//
//

#pragma once

#include "ofxOpenNI.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class OpenNI : public Source, public ofxOpenNI {
    public:
        
        OpenNI() : Source(), ofxOpenNI()
        {
            // type defaults to CAMERA_CUSTOM
            bCanTrackHaar = false;
            invertedPixels.allocate( 320,240, OF_IMAGE_GRAYSCALE);
            tiltAmount = 0;
        }
        
        /** Tilt amount (-1 to 1)  */
        float tiltAmount;
        
        // core
        bool available(){
            return (getNumDevices() >= 1);
        }
        
        void update(){
            ofxOpenNI::update();
            if ( isDepthOn() ){
                //this is pretty annoying, but ofxOpenNI colors are backwards
                ofPixelsRef pix = getDepthPixels();
                int channels    = pix.getNumChannels();
                int dims        = invertedPixels.getWidth() * invertedPixels.getHeight();
                float shift       = 0.0f;
                
                for ( int i=0; i<dims; i++){
                    int y = i / getWidth();
                    
                    if ( tiltAmount > 0 ){
                        float div = (float) y/getHeight();
                        shift = (div * tiltAmount);
                    } else if ( tiltAmount < 0) {
                        float div = ((float) (getHeight()-y)/getHeight() );
                        shift = div * (-tiltAmount);
                    }
                    // invert
                    invertedPixels[i] = pix[2+i * channels] == 0 ? 0 : fabs(255.0f-pix[2+i * channels]);
                    
                    // shift
                    invertedPixels[i] *= (float) (1.0f-shift);
                }
#ifdef TARGET_OSX
                invertedTexture.loadData(invertedPixels);
                publishToSyphon( invertedTexture );
#endif
            }
        }
        
        bool doProcessFrame(){
            return isNewFrame();
        }
        
        // required bc ofxOpenNI doesn't declare it
        unsigned char*  getPixels(){
            return invertedPixels.getPixels();
        }
        
        ofPixelsRef getPixelsRef(){
            return invertedPixels;
        }
        
        bool openSource( int width, int height, string etc="" ){
            bool bSetup = isContextReady();
            if(!bSetup) bIsOpen = setup(true, width, height);
            if ( bIsOpen ){
                if ( invertedPixels.getWidth() != width || invertedPixels.getHeight() != height ){
                    invertedPixels.clear();
                    invertedPixels.allocate( width, height, OF_IMAGE_GRAYSCALE);
                }
                setDepthColoring(COLORING_BLUES);
                addDepthGenerator();
                start();
#ifdef TARGET_OSX
                invertedTexture.allocate(invertedPixels);
#endif
            }
            return bIsOpen;
        }
        
        void closeSource(){
            stop();
        }
    private:
        ofPixels    invertedPixels;
        ofTexture   invertedTexture;
    };
}

