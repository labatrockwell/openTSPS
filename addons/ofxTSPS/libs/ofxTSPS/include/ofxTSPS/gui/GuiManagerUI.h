/*
 *  ofxTSPS::GuiManagerUI.h
 *  openTSPS
 *
 *  Created by Brett Renfer on 12/7/09.
 *  Copyright 2014 TSPS. All rights reserved.
 *
 
 Portions of this software are distributed under the GPL and the MIT license.
 
 The MIT License
 
 Copyright (c) 2014 TSPS
 
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

#include "ofxUI.h"
#include "ofxTSPS/gui/Settings.h"

// maybe there's a simpler way to integrate this?
#include "ofxCvCoordWarpingGui.h"

// hm, would like to not include these all...
#include "ofxTSPS/source/Source.h"

#include "ofxTSPS/source/Kinect.h"
#include "ofxTSPS/source/OpenNI2.h"
#include "ofxTSPS/source/Syphon.h"
#include "ofxTSPS/source/VideoFile.h"
#include "ofxTSPS/source/VideoGrabber.h"

namespace ofxTSPS {
    typedef struct  {
        int type;
        string key;
        
        float* f;
        int* i;
        bool* b;
        
    } GUICustomParam;
    
    typedef struct {
        SourceType  type;
        int         index;
    } SourceSelection;
    
    class ofxUIDirectoryDropdown : public ofxUIDropDownList {
    public:
        
        ofxUIDirectoryDropdown( string name, string fileDirectory, vector<string> extensions, bool autoRefresh = true ) :
        ofxUIDropDownList(name, vector<string>() ){
            setAllowMultiple(false);
            for ( auto e : extensions ) directory.allowExt(e);
            currentDirectory = fileDirectory;
            refresh();
            
            selectedIndeces.push_back(0);
            
            if ( autoRefresh ){
                ofAddListener(ofEvents().update, this, &ofxUIDirectoryDropdown::update);
            }
        }
        
        void update( ofEventArgs & e ){
            if ( needToRefresh ){
                refresh();
            }
        }
        
        int refresh(){
            files.clear();
            numFiles = directory.listDir(currentDirectory);
            int i = 0;
            for ( auto & f : directory.getFiles()){
                files.push_back( f.getFileName() );
                i++;
            }
            clearToggles();
            addToggles(files);
            return numFiles;
        }
        
        int getNumFiles(){
            return numFiles;
        }
        
        int listDir( string fileDirectory ){
            currentDirectory = fileDirectory;
            return refresh();
        }
        
        string getSelectedPath(){
            if ( getSelectedIndeces().size() == 0 ){
                return "";
            }
            return directory.getFile( getSelectedIndeces()[0] ).getAbsolutePath();
        }
        
        // public vars for connecting to other guis
        bool needToRefresh;
        
        vector<string> files;
        
    protected:
        ofDirectory directory;
        string currentDirectory;
        int numFiles;
    };
    
    class GuiManagerUI {
        
    public:
        GuiManagerUI();
        ~GuiManagerUI();
        void setup();
        
        void enableEvents();
        void disableEvents();
        
        bool enableGui;
        
        void update(ofEventArgs &e);
        void update();
        void draw();
        
        void enableGroup( string & groupName );
        void enableGroup( string groupName );
        void disableGroup( string groupName );
        vector<string> getGroupNames();
        
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
        
        // refresh current sources
        void refreshSourcePanel();
        map<int,SourceSelection> * getCurrentSources();
        int getSourceSelectionIndex( SourceType type, int deviceIndex);
        
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
        void changeGuiCameraView(bool bCameraView){}
        
        // minimize + maximize (should this be here? hmmm)
        void minimize( string & button );
        void maximize( string & button );
        
        // get settings object
        Settings * getSettings();
        
    protected:
        Settings settings;
        ofxUITabBar * guiController;
        map<string, ofxUITabBar *> guiControllers;
        map<string, map<string, ofxUIScrollableCanvas *> > guis;
        
        // custom widgets
        ofxUIDirectoryDropdown * videoFileDropdown;
        ofxUIDirectoryDropdown * haarFileDropdown;
        
        string enabledPanelGroup;
        
        //an even goofier way to easily add the quad qui
        bool quadGuiSetup;
        ofxCvCoordWarpingGui quadGui;
        int cameraWidth, cameraHeight;
        int currentPanel, lastCurrentPanel;
        
        // switch state of learn BG when we need to
        bool bLastBgState;
        
        // map of source indices to types
        map<int,SourceSelection> currentSources;
        
        void saveEventCatcher( string & buttonName);
        void reloadEventCatcher( string & buttonName);
        void loadEventCatcher( string & buttonName);
        void saveAsEventCatcher( string & buttonName);
        
        // amazing minimize + maximize buttons
        
        //are we using events?
        bool bEventsEnabled;
        
        // add custom panel if a user calls any of the add() functions
        int     customItemCount; // this is dumb, probably a better way to do this
        int     customPanelCount;
        string  currentCustomPanel;
        bool    bHasCustomPanel;
        void    addCustomGui();
    };
}
