//
//  VideoFile.h
//  openTSPS
//
//  Created by Brett Renfer on 8/7/12.
//
//

#pragma once

#include "ofVideoPlayer.h"
#include "ofxTSPS/source/Source.h"

namespace ofxTSPS {
    class VideoFile : public Source, public ofVideoPlayer {
    public:
        VideoFile(){
            type = CAMERA_VIDEOFILE;
        }
        
        bool openSource( int width, int height, string etc="" ){
            tspsWidth = width;
            tspsHeight = height;
            customData = etc;
            bool loaded = loadMovie( customData );
            if ( loaded ){
                play();
            }
            return loaded;
        }
        
        void update(){
            ofVideoPlayer::update();
            if ( width != tspsWidth || height != tspsHeight ){
                // is there a better way to do this? probably...
                //resize( tspsWidth, tspsHeight );
            }
        }
        
        void closeSource(){
            stop();
            closeMovie();
            close();
        }
    private:
        
        int tspsWidth, tspsHeight;
        
    };
}
