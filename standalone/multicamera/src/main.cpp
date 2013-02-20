#include "ofMain.h"
#include "tspsApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( int argc, char * const argv[] ){
    int     numCameras  = 4;
    
    vector<string> args(argv + 1, argv + argc);
    
	// from: http://stackoverflow.com/questions/441547/most-efficient-way-to-process-arguments-from-the-command-line-in-c
	// because no getopt on windows :(
    
    // Loop over command-line args
    for (vector<string>::iterator i = args.begin(); i != args.end(); ++i) {
        if (*i == "-i") {
            numCameras = ofToInt(*++i);
        }
    }
    
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp( new tspsApp(numCameras) );
    
}
