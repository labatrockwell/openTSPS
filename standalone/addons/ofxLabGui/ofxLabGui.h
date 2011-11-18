/*
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

/**********************************************************************************
	This is totally based on Zach + Theo's ofxControlPanel class found in their
	EyeWriter project: http://www.eyewriter.org/
	The change from ofxControlPanel to ofxLabGui is to avoid confusion + because
	there is quite a bit of layout stuff that is different
 
	Things that are different:
	1 - groups:
		- groups are optional, but allow you to group items within panels
			- this gives you the option to change color + hierarchy within panels,
			and activate / deactivate group
		- in the future, groups will be collapsible as well
	2 - tabs
		- tabs are integrated into the guiTypePanel class
		- the name of the panel sits up in the tab
	3 - guiTypeText
		- finished this class (it was in ofxControlPanel, but empty)
		- added passing key events through to objects for typing
		- added string type to guiValue to make this usable
	4 - guiTypeButton
		- button-style gui elements that dispatches an ofEvent on press
		- used these for save / restore / load / save as buttons
	4 - overall spacing + design tweaks
		- lots of little things have been adjusted, i.e. spacing, having the
		text to the right of toggles, not displaying "0" or "1" with toggles, etc
	
	- by Brett Renfer, Joshua Walton - http://lab.rockwellgroup.com
	  + James George (http://www.jamesgeorge.org)
**********************************************************************************/

#pragma once


#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "guiIncludes.h"

//hehe - there is no xml yet!!!
//but we are setting things up for XML

#define borderWidth  10.f
#define topSpacing   20.f
#define topBarHeight 30.f
#define buttonWidth	 55.f
#define buttonHeight 20.f

#define guiButtonWidth 220.f
#define guiButtonHeight 20.f

#include "ofxFileDialog.h"

class xmlAssociation{
    public:
        xmlAssociation(guiBaseObject * objPtr, string xmlNameIn, int numParamsIn){
            guiObj      = objPtr;
            xmlName     = xmlNameIn;
            numParams   = numParamsIn;
        }

        guiBaseObject * guiObj;
        string xmlName;
        int numParams;
};

class ofxLabGui: public guiBaseObject{

	public:

        ofxLabGui();
        ~ofxLabGui();

        void setup(string controlPanelName, float panelX, float panelY, float width, float height);
        void loadFont( string fontName, int fontsize );

		guiTypePanel * addPanel(string panelName, int numColumns, bool locked = false);
		guiTypeGroup * addGroup(string groupName, bool locked = false);

        void setWhichPanel(int whichPanel);
		void setWhichPanel(string panelName);
		void setWhichGroup(int whichGroup);
        void setWhichColumn(int column);
	
		int getSelectedPanel(){  return selectedPanel; };
		string getSelectedPanelName(){	
			if (panels.size() > 0 && selectedPanel < panels.size()) return panels[selectedPanel]->name;
			else return "";
		};
		void setSelectedPanel(int whichPanel);
		void setSelectedPanel(string panelName);
		
		void setDrawBarOnTop( bool drawBarOnTop ){ bDrawBarOnTop = drawBarOnTop; };
        void setSliderWidth(int width);

        guiTypeToggle * addToggle(string name, string xmlName, bool defaultValue);
        guiTypeMultiToggle * addMultiToggle(string name, string xmlName, int defaultBox, vector <string> boxNames);
        guiTypeSlider * addSlider(string sliderName, string xmlName, float value , float min, float max, bool isInt);
        guiType2DSlider * addSlider2D(string sliderName, string xmlName, float valueX, float valueY, float minX, float maxX, float minY, float maxY, bool isInt);
        guiTypeDrawable * addDrawableRect(string name, ofBaseDraws * drawablePtr, int drawW, int drawH);
        guiTypeCustom * addCustomRect(string name, guiCustomImpl * customPtr, int drawW, int drawH);
        guiTypeLogger * addLogger(string name, simpleLogger * logger, int drawW, int drawH);
        guiTypeFileLister * addFileLister(string name, simpleFileLister * lister, int drawW, int drawH);
		guiTypeText * addTextField(string name, string xmlName, string defaultValue, int drawW, int drawH);
		
		//JG now you can add buttons
		guiTypeButton* addButton(string buttonName);

        void setValueB(string xmlName, bool value,  int whichParam = 0);
        void setValueI(string xmlName, int value,  int whichParam = 0);
        void setValueF(string xmlName, float value,  int whichParam = 0);
        bool getValueB(string xmlName, int whichParam = 0);
	
        float getValueF(string xmlName, int whichParam = 0);
		int getValueI(string xmlName, int whichParam = 0);
		string getValueS(string xmlName, int whichParam = 0, string defaultString = "");
            
        //returns a bang if button pressed
        bool getButtonPressed(string buttonName);
        guiBaseObject * getElement( string name );
    
		void setSaveColor( int r, int g, int b );
		void setSaveSelectedColor( int r, int g, int b );
		void setRestoreColor( int r, int g, int b );
		void setRestoreSelectedColor( int r, int g, int b );
		void setLoadColor( int r, int g, int b );
		void setLoadSelectedColor( int r, int g, int b );
		void setSaveAsColor( int r, int g, int b );
		void setSaveAsSelectedColor( int r, int g, int b );
	
		void setGroupActive(string panelName, string groupName, bool bActive);
	
		string getCurrentXMLFile();
        void setIncrementSave(string incrmentalFileBaseName);
        void disableIncrementSave();
		void loadSettings(string xmlFile);
		void reloadSettings();
        void saveSettings(string xmlFile);
        void saveSettings();
		void saveAsSettingsEvent( string & buttonName);
		
		void saveSettingsEvent( string & buttonName);
		void loadSettingsEvent(string & buttonName);
		void reloadSettingsEvent(string & buttonName);
		
		guiTypeButton * getSaveButton();
		guiTypeButton * getRestoreButton();
		guiTypeButton * getLoadButton();
		guiTypeButton * getSaveAsButton();
	
        void setDraggable(bool bDrag);
		void setCollapsible(bool bCollapse);
        void setMinimized(bool bMinimize);
        void show();
        void hide();

        void toggleView();
        void mousePressed(float x, float y, int button);
        void mouseDragged(float x, float y, int button);
        void mouseReleased();
		void keyPressed(int key);
		void keyReleased(int key);

        void updateBoundingBox();
        void update();
        void draw();

	protected:
	
		ofTrueTypeFont guiTTFFont;
		
        vector <xmlAssociation> xmlObjects;
        vector <guiBaseObject *> guiObjects;
		vector <guiTypePanel *> panels;		

		ofxXmlSettings settings;
		string currentXmlFile;
		string settingsDirectory;
	
		//ofxFileDialog fileDialog;
	
		guiTypeButton * saveButton;
		guiTypeButton * restoreButton;
		guiTypeButton * loadButton;
		guiTypeButton * saveAsButton;
		
		//JG internal callback that receives all button presses and
		//sets bools in the map below
		void buttonPressed(string& buttonName);
		//JG simple map structure for pressing buttons
		map<string, bool> pressedButtons;

		ofRectangle topBar;
		ofRectangle minimizeButton;

        string incrementSaveName;

        bool hidden;
        bool usingXml;
        bool bUseTTFFont;
		bool minimize;
		bool saveDown;
		bool incrementSave;
		bool restoreDown;
		bool bDraggable;
		bool bCollapsible;
		bool bDrawBarOnTop;

		int selectedPanel;
        int currentPanel;
		int currentGroup;

        ofPoint prevMouse;

		int sliderWidth;

		ofPoint mouseDownPoint;

		bool dragging;

};


