/*
 *  ofxTSPS::GuiManager.h
 *  openTSPS
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *

 Portions of this software are distributed under the GPL and the MIT license.

 The MIT License

 Copyright (c) 2010 Rockwell Group LLC

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 */

#include "ofxLabGui.h"
#include "ofxTSPS/gui/Settings.h"

// maybe there's a simpler way to integrate this?
#include "ofxCvCoordWarpingGui.h"

namespace ofxTSPS {
    typedef struct  {
        int type;
        string key;
        
        float* f;
        int* i;
        bool* b;
        
    } GUICustomParam;
    
    class GuiManager {
        
    public:
        GuiManager();
        ~GuiManager();
        void setup();
        
        void enableEvents();
        void disableEvents();
        
        bool enableGui;
        void setupQuadGui ( int cameraWidth, int cameraHeight );
        void drawQuadGui();
        void drawQuadGui( int x, int y, int width, int height );
        
        void update(ofEventArgs &e);
        void update();
        void draw();
        
        // set capabilities
        void setHaarEnabled( bool bHaar );
        void setOpticalFlowEnabled( bool bOpticalFlow );
        //void setImageFiltersEnable( bool bEnableFilters);
        
        //forwarded events
        void mousePressed(ofMouseEventArgs &e);
        void mouseDragged(ofMouseEventArgs &e);
        void mouseReleased(ofMouseEventArgs &e);
        void keyPressed(ofKeyEventArgs &e);
        void keyReleased(ofKeyEventArgs &e);
        
        //get + add parameters
        void addSlider(string name, int* value, int min, int max);
        void addSlider(string name, float* value, float min, float max);
        void addToggle(string name, bool* value);
        
        // add elements outside of panels
        guiTypeButton * addButton( string name, ofRectangle dimensions, ofColor color = NULL, ofColor bgcolor = NULL );
        guiTypeButton * getButton( string name );
        
        //get + set panel info
        int getSelectedPanel();
        void setSelectedPanel( int index );
        void setSelectedPanel( string name );
        
        //disable / enable stuff
        void enableElement( string name );
        void disableElement( string name );
        
        // get values
        bool    getValueB( string name );
        int     getValueI( string name );
        float   getValueF( string name );
        string  getValueS( string name );
        
        void    setValueB( string name, bool val );
        void    setValueI( string name, int val );
        void    setValueF( string name, float val );
        void    setValueS( string name, string val );
        
        // load
        void loadSettings ( string xml );
    	
        // Added so the quadGui instance can know when image warping is allowed to occur
        //            (i.e., the image can only get warped when in Camera View).
        void changeGuiCameraView(bool bCameraView);
        
        // minimize + maximize (should this be here? hmmm)
        void minimize( string & button );
        void maximize( string & button );
        
        // get settings object
        Settings * getSettings();
        
    protected:
        Settings settings;
        
        //a little goofy way to keep track of custom params. works but i'm open to suggestions
        ofxLabGui panel;
        vector<GUICustomParam> params;
        map<std::string, guiTypeButton*> customButtons;
        
        //an even goofier way to easily add the quad qui
        bool quadGuiSetup;
        ofxCvCoordWarpingGui quadGui;
        int cameraWidth, cameraHeight;
        int currentPanel, lastCurrentPanel;
        
        void saveEventCatcher( string & buttonName);
        void reloadEventCatcher( string & buttonName);
        void loadEventCatcher( string & buttonName);
        void saveAsEventCatcher( string & buttonName);
        
        // amazing minimize + maximize buttons
        guiTypeButton * minimizeButton;
        guiTypeButton * maximizeButton;
        
        //are we using events?
        bool bEventsEnabled;
        
        simpleFileLister* videoFiles;
        simpleFileLister* haarFiles;
        
    private:
        guiTypeGroup * haarGroup;
        guiTypeGroup * opticalFlowGroup;
    };
}
