#!/bin/bash

# Dependencies for TSPS

#make sure you are in the TSPS folder when you run this.

cd ../addons


PREFIX="git clone https://github.com/"

# cv
${PREFIX}kylemcdonald/ofxCv.git

#cameras
${PREFIX}labatrockwell/ofxNI2.git
#${PREFIX}labatrockwell/ofxOpenNI.git
${PREFIX}robotconscience/ofxKinectV2

# gui
${PREFIX}rezaali/ofxUI.git

# comms
${PREFIX}labatrockwell/ofxLibwebsockets.git
${PREFIX}Spacebrew/ofxSpacebrew.git
${PREFIX}astellato/ofxSyphon.git
${PREFIX}labatrockwell/ofxTSPSReceiver
${PREFIX}emmanuelgeoffray/ofxTuioWrapper