//
//  OpenNI2.h
//  openTSPS
//
//  Created by BRenfer on 7/19/13.
//
//

#pragma once


#include "ofxTSPS/source/Source.h"
#include "ofxNI2.h"

namespace ofxTSPS {
    class OpenNI2 : public Source, public ofxNI2::DepthStream {
    public:
        
        OpenNI2() : Source(), ofxNI2::DepthStream()
        {
            // type defaults to CAMERA_CUSTOM
            bCanTrackHaar = false;
            bDepthSetup = false;
            device = NULL;
        }
        
        ~OpenNI2(){
            device->exit();
            delete device;
        }
        
        // core
        bool available(){
            if ( device == NULL ){
                device = new ofxNI2::Device;
                device->setup();
            }
            return (device->listDevices() >= 1);
        }
        
        void update(){
            updateTextureIfNeeded();
#ifdef TARGET_OSX
            publishToSyphon( getTextureReference() );
#endif
        }
        
        unsigned char * getPixels(){
            return getPixelsRef().getPixels();
        }
        
        ofPixels & getPixelsRef(){
            static ofPixels retPix;
            depthRemapToRange(ofxNI2::DepthStream::getPixelsRef(), retPix, 0, stream.getMaxPixelValue(), true);
            return retPix;
        }
        
        bool doProcessFrame(){
            return ofxNI2::DepthStream::getPixelsRef().getWidth() != 0;
        }
        
        // fixed invert...
        inline void depthRemapToRange(const ofShortPixels &src, ofPixels &dst, int near, int far, int invert)
        {
            int N = src.getWidth() * src.getHeight();
            dst.allocate(src.getWidth(), src.getHeight(), 1);
            
            const unsigned short *src_ptr = src.getPixels();
            unsigned char *dst_ptr = dst.getPixels();
            
            float inv_range = 1. / (far - near);
            
            if (invert)
                std::swap(near, far);
            
            for (int i = 0; i < N; i++)
            {
                unsigned short C = *src_ptr;
                *dst_ptr = C == 0 ? 0 : ofMap(C, near, far, 0, 255, true);
                src_ptr++;
                dst_ptr++;
            }
        }
        
        bool openSource( int width, int height, string etc="" ){
            // setup device?
            if ( device == NULL ){
                device = new ofxNI2::Device;
                device->setup();
            }
            
            // only try to attach device once
            if ( !bDepthSetup ){
                bIsOpen = setup(*device);
//                setSize(320, 240);
//                setFps(30);
                bDepthSetup  = bIsOpen;
            } else {
                bIsOpen = true;
            }
            
            if (bIsOpen)
            {
                start();
            }
            return bIsOpen;
        }
        
        void closeSource(){
            stream.stop();
            bIsOpen = false;
        }
    private:
        ofxNI2::Device *device;
        bool bDepthSetup;
    };
}