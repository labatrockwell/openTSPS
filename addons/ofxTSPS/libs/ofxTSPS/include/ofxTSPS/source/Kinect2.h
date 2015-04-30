//
//  Kinect2.h
//  openTSPS
//
//  Created by Brett Renfer on 4/30/15.
//
//
#pragma once

#include "ofxKinectForWindows2.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class Kinect2 : public Source, public ofxKFW2::Device {
    public:
                
        Kinect2() : Source(){
            type = CAMERA_KINECT;
			bDepthInited = false;
			
			// these will eventually be dynamic, OK
			nearClipping = 50.0;
			farClipping = 1000.0;
        }
        
        // core
        bool available(){
			return true; // hm~
		}
		bool doProcessFrame(){
			return getPixelsRef().getWidth() != 0; //hm;
        }
        
        int numAvailable(){
            return 1; // no multi for now!
        };
        
        void update(){
            ofxKFW2::Device::update();
#ifdef TARGET_OSX
            if ( bPublishTexture ){
                //publishToSyphon( ofxKinect::getTextureReference() );
            }
#endif
			// I like this routine! From here:
			// https://github.com/rickbarraza/KinectV2_Lessons/blob/master/3_MakeRawDepthBrigther/src/testApp.cpp
			static vector<unsigned char> depthLookupTable;
			
			if ( depthLookupTable.size() == 0){

				// IF YOU WANT NEAR TO BE BLACK AND FAR TO BE WHITE, SWITCH THESE
				unsigned char nearColor = 255;
				unsigned char farColor = 0;

				// HOW FAR BACK WE WANT TO GRADIENT BEFORE HITTING THE MAX DISTANCE.
				// EVERYTHING PAST THE MAX DISTANCE WILL BE CLAMPPED TO 'farColor'
				unsigned int maxDepthLevels = 5001;
				depthLookupTable.resize(maxDepthLevels);

				depthLookupTable[0] = 0;
				for (unsigned int i = 1; i < maxDepthLevels; i++)
				{
					depthLookupTable[i] = ofMap(i, nearClipping, farClipping, nearColor, farColor, true);
				}
			}

			auto & pix = getDepthSource()->getPixelsRef(); //ofShortPixels

			if ( !outputPixels.isAllocated() && pix.getWidth() != 0 && pix.getHeight() != 0 ){
				outputPixels.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
			}


			if ( outputPixels.getWidth() != 0 && outputPixels.getHeight() != 0 ){
				int rawindex = 0;
				int displayindex = 0;
				for (int y = 0; y < pix.getHeight(); y++) {
					for (int x = 0; x < pix.getWidth(); x++) {
						auto c = pix.getColor(x,y).r;
						unsigned char greyValue = depthLookupTable[MIN(depthLookupTable.size() -1, c)];

						for ( int i=0; i<outputPixels.getNumChannels(); i++ ){
							outputPixels.getPixels()[displayindex++] = greyValue;
						}
					}
				}
			}
        }
        
        ofPixelsRef getPixelsRef(){
			return outputPixels;
        }

		unsigned char * getPixels(){
			return getPixelsRef().getPixels();
		}
        
        bool openSource( int width, int height, string etc="" ){
            if (!bDepthInited){
				bDepthInited = true;
				ofxKFW2::Device::open();
				ofxKFW2::Device::initDepthSource();
				bIsOpen = getDepthSource() != nullptr;
			}
            return bIsOpen;
        }
        
        //void update();
        void closeSource(){
			if ( bDepthInited ){
				ofxKFW2::Device::close();
				bDepthInited = false;
			}
        }
		
		// making these publicly accessible to GUI
		float nearClipping, farClipping;

	protected:
		ofPixels outputPixels;
		bool bDepthInited;
    };
}
