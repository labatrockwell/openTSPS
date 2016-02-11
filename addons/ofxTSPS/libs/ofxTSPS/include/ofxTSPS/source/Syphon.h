//
//  Syphon.h
//  openTSPS
//
//  Created by BRenfer on 5/23/13.
//
//

#pragma once
#ifdef TARGET_OSX

#include "ofxSyphon.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class Syphon : public Source, public ofxSyphonClient {
    public:
        Syphon() : Source(){
            type = CAMERA_SYPHON;
        }
        
        // core
        bool available(){
            return true;
        }
        
        void update(){
            // read texture to pixels
            if ( bSetup ){
                this->bind(); this->unbind();
                
                // readToPixels doesn't work, as syphon textures are special:
                // http://forum.openframeworks.cc/index.php?topic=7791.0
                // so we're doing a dumb hack
                if ( !hackFBO.isAllocated() || hackFBO.getWidth() != getWidth() ){
                    hackFBO.allocate( getWidth(), getHeight(), GL_RGB );
                }
                hackFBO.begin();
                ofFill();
                ofClear(0);
                ofSetColor(255);
                mTex.draw(0,0);
                hackFBO.end();
                hackFBO.readToPixels(pixels);
                
                if ( bPublishTexture ){
                    publishToSyphon( mTex );
                }
            }
            
            // do we also want to repulish the texture from TSPS? Is that weird?
        }
        
        bool doProcessFrame(){
            return true;
        }
        
        unsigned char * getData(){
            return pixels.getData();
        }
        
        ofPixels & getPixels(){
            return pixels;
        }
        
        // width and height are ignored, but left in because of virtual function override
        bool openSource( int width, int height, string serverName="" ){
            setup();
            bIsOpen = true;
            
            // allocate pixels to avoid errors
            pixels.allocate(width, height, OF_IMAGE_COLOR);
            
            // bind to either first available server OR name specified in serverName parameter
            setServerName("");
            setApplicationName("Simple Server");
            
            return true;
        }
        
        void closeSource(){}
    protected:
        ofPixels    pixels;
        ofFbo       hackFBO;
    };
}

#endif
