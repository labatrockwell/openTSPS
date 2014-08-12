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
    
    class ofxUIDynamicRadio {
    public:
        
        ofxUIDynamicRadio( string _name, string fileDirectory, vector<string> extensions, bool autoRefresh = true ){
            name = _name;
            radio = NULL;
            
            for ( auto e : extensions ) directory.allowExt(e);
            currentDirectory = fileDirectory;
            needToRefresh = true;
            refresh();
            
            if ( autoRefresh ){
                ofAddListener(ofEvents().update, this, &ofxUIDynamicRadio::update, OF_EVENT_ORDER_BEFORE_APP);
            }
        }
        
        void update( ofEventArgs & e ){
            if ( needToRefresh ){
                needToRefresh = false;
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
            
            ofxUICanvas * parent = NULL;
            if ( radio != NULL ){
                parent = (ofxUICanvas *) radio->getParent();
                
                if ( parent != NULL && radio != NULL ){
                    parent->removeWidget(radio);
                }
            }
            
            vector<string> fileNames;
            
            for ( auto tname : files ){
                fileNames.push_back(tname);
            }
            
            if ( parent != NULL ){
                radio = parent->addRadio(name, fileNames);
            } else {
                radio = new ofxUIRadio(name, fileNames, OFX_UI_ORIENTATION_VERTICAL, 0, 0);
            }
            return numFiles;
        }
        
        ofxUIRadio * getWidget(){
            return radio;
        }
        
        int getNumFiles(){
            return numFiles;
        }
        
        int listDir( string fileDirectory ){
            currentDirectory = fileDirectory;
            return refresh();
        }
        
        string getSelectedPath(){
            if ( numFiles == 0 || ( radio!= NULL && radio->getValue() == -1) || radio == NULL ) return "";
            return directory.getFile( radio->getValue() ).getAbsolutePath();
        }
        
        // public vars for connecting to other guis
        bool needToRefresh;
        
        vector<string> files;
        
    protected:
        ofDirectory directory;
        string currentDirectory;
        int numFiles;
        
        ofxUIRadio * radio;
       string name;
    };
    
    typedef ofxUICanvas GUICanvas;
    
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
        
        // quad gui stuff
        void setupQuadGui ( int cameraWidth, int cameraHeight );
        void drawQuadGui();
        void drawQuadGui( int x, int y, int width, int height );
        
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
        void changeGuiCameraView(bool bCameraView);
        
        // minimize + maximize (should this be here? hmmm)
        void minimize( string & button );
        void maximize( string & button );
        
        // get settings object
        Settings * getSettings();
        
    protected:
        Settings settings;
        ofxUITabBar * guiController;
        map<string, ofxUITabBar *> guiControllers;
        map<string, map<string, GUICanvas *> > guis;
        vector<string> guiNames; //hack-y way to keep an ordered list
        vector<string> guiTypes; //hack-y way to keep an ordered list
        
        // custom widgets
        ofxUIDynamicRadio * videoFileDropdown;
        ofxUIDynamicRadio * haarFileDropdown;
        
        // setup methods
        GUICanvas * setupGUI( string name, string type );
        GUICanvas * setupGUI( string longName, string name, string type );
        
        // quad gui stuff
        bool quadGuiSetup;
        ofxCvCoordWarpingGui quadGui;
        
        void showHideWidgetChildren( GUICanvas * group, string name, bool bShow );
        
        string enabledPanelGroup;
        
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
