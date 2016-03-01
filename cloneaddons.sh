#!/bin/bash

#make sure you are in the TSPS folder when you run this.

cd ../addons


PREFIX="git clone https://github.com/"

# source
${PREFIX}elliotwoods/ofxKinectForWindows2 --branch 0.8.4
${PREFIX}ofTheo/ofxKinectV2.git
${PREFIX}satoruhiga/ofxOpenGLContextScope.git
${PREFIX}kylemcdonald/ofxCv
ofxMSAInteractiveObject
${PREFIX}labatrockwell/ofxLibwebsockets.git
${PREFIX}memo/ofxMSAInteractiveObject.git
${PREFIX}labatrockwell/ofxNI2.git
${PREFIX}Spacebrew/ofxSpacebrew.git
${PREFIX}astellato/ofxSyphon.git
${PREFIX}labatrockwell/ofxTSPSReceiver.git
${PREFIX}arturoc/ofxTuioWrapper.git
