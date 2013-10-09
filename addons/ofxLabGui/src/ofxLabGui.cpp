#include "ofxLabGui.h"

//----------------------------
ofxLabGui::ofxLabGui(){
    dragging        = false;
    minimize        = false;
    selectedPanel   = 0;
    currentPanel    = 0;
	currentGroup	= 0;
    bUseTTFFont     = false;
    usingXml        = false;
    saveDown        = false;
    restoreDown     = false;
    incrementSave   = false;
    hidden          = false;
    bDraggable      = true;
    bCollapsible    = true;

    currentXmlFile = "";
    incrementSaveName = "";
    xmlObjects.clear();

}

ofxLabGui::~ofxLabGui(){
    for(int i = 0; i < guiObjects.size(); i++){
        delete guiObjects[i];
    }
}

//-----------------------------
void ofxLabGui::setup(string controlPanelName, float panelX, float panelY, float width, float height){
	name = controlPanelName;

	setPosition(panelX, panelY);
	setDimensions(width, height);
	setShowText(true);

	bDrawBarOnTop = true;

	//setup save + restore + load

	int buttonBaseX = 0;
	int buttonBaseY = 0;

	saveButton = new guiTypeButton();
	saveButton->setup("save", buttonWidth, buttonHeight);
	if (bCollapsible){
		saveButton->setPosition(minimizeButton.x - (buttonWidth), boundingBox.y + borderWidth);
	} else {
		saveButton->setPosition(boundingBox.x + boundingBox.width - (buttonWidth) - borderWidth, boundingBox.y + borderWidth);
	}
	ofAddListener(saveButton->buttonPressed, this, &ofxLabGui::saveSettingsEvent);

	restoreButton = new guiTypeButton();
	restoreButton->setup("restore", buttonWidth, buttonHeight);
	restoreButton->setPosition(saveButton->getPosX() - (buttonWidth + borderWidth), saveButton->getPosY());
	ofAddListener(restoreButton->buttonPressed, this, &ofxLabGui::reloadSettingsEvent);

	loadButton = new guiTypeButton();
	loadButton->setup("load", buttonWidth, buttonHeight);
	loadButton->setPosition(restoreButton->getPosX() - (buttonWidth + borderWidth), restoreButton->getPosY());
	ofAddListener(loadButton->buttonPressed, this, &ofxLabGui::loadSettingsEvent);
	
	saveAsButton = new guiTypeButton();
	saveAsButton->setup("save as", buttonWidth, buttonHeight);
	saveAsButton->setPosition(loadButton->getPosX() - (buttonWidth + borderWidth), loadButton->getPosY());
	ofAddListener(saveAsButton->buttonPressed, this, &ofxLabGui::saveAsSettingsEvent);
}

//-----------------------------
void ofxLabGui::loadFont( string fontName, int fontsize ){
    guiTTFFont.loadFont(fontName, fontsize);
    bool okay = guiTTFFont.isLoaded();

    if(okay){
        bUseTTFFont = true;
        for(int i = 0; i < guiObjects.size(); i++){
            guiObjects[i]->setFont(&guiTTFFont);
        }
		saveButton->setFont(&guiTTFFont);
		restoreButton->setFont(&guiTTFFont);
		loadButton->setFont(&guiTTFFont);
		saveAsButton->setFont(&guiTTFFont);
		setFont(&guiTTFFont);
    }
}

//---------------------------------------------
guiTypePanel * ofxLabGui::addPanel(string panelName, int numColumns, bool locked){
    guiTypePanel * panelPtr = new guiTypePanel();

    if( bUseTTFFont ){
        panelPtr->setFont(&guiTTFFont);
    }

    panelPtr->setup(panelName);
	panelPtr->setDimensions(boundingBox.width, boundingBox.height - topBarHeight - borderWidth*3 );
	if (panels.size() > 0){
		ofRectangle tabRect = panels[panels.size()-1]->getTabRect();
		panelPtr->setTabPosition(tabRect.x + tabRect.width, tabRect.y);
	} else {
		panelPtr->setTabPosition(0,0);
		panelPtr->bSelected = true;
	}
	ofRectangle tabRect = panelPtr->getTabRect();
    panelPtr->setPosition(0, (int)tabRect.y+tabRect.height);

    if( locked )panelPtr->lock();
    else panelPtr->unlock();
    
    if( numColumns > 1 ){
        for(int i = 1; i < numColumns; i++){
            panelPtr->addColumn(30);
        }
    }
    
    panels.push_back(panelPtr);
    guiObjects.push_back(panelPtr);

    
    // recalc all positions if we have to
    if ( tabRect.x + tabRect.width > boundingBox.width ){
        float diff = (tabRect.x + tabRect.width) - boundingBox.width;
		panelPtr->setTabPosition(tabRect.x - diff, tabRect.y);
        //diff /= (float) panels.size();
        float w = boundingBox.width - diff;
        w /= panels.size()-1;
        for ( int i=0; i<panels.size()-1; i++){
            ofRectangle tabRect = panels[i]->getTabRect();
            float dest = w * i;
            if ( i > 0 ){
                dest = min( (float) panels[i-1]->getTabRect().x + panels[i-1]->getTabRect().width, (float) w * i );
                if ( dest + tabRect.width < panels[i-1]->getTabRect().x + panels[i-1]->getTabRect().width ){
                    dest = panels[i-1]->getTabRect().x + panels[i-1]->getTabRect().width;
                }
            }
            panels[i]->setTabPosition(dest,  tabRect.y);
        }
    }
    
    return panelPtr;
}

//---------------------------------------------
guiTypeGroup * ofxLabGui::addGroup(string groupName, bool locked){
    guiTypeGroup * groupPtr = new guiTypeGroup();
    groupPtr->setup(groupName);
    groupPtr->setDimensions(panels[currentPanel]->boundingBox.width - borderWidth*4, 10);
    if( locked )groupPtr->lock();
    else groupPtr->unlock();

    panels[currentPanel]->addElement( groupPtr );
	panels[currentPanel]->numGroups++;
	panels[currentPanel]->groups.push_back(groupPtr);
	setWhichGroup(panels[currentPanel]->numGroups-1);

    guiObjects.push_back(groupPtr);
    if( bUseTTFFont ){
        groupPtr->setFont(&guiTTFFont);
    }

    return groupPtr;
}

// ############################################################## //
// ##
// ##      Control where things are added to
// ##
// ############################################################## //


//---------------------------------------------
void ofxLabGui::setWhichPanel(int whichPanel){
    if( whichPanel < 0 || whichPanel >= panels.size() )return;
    currentPanel = whichPanel;
    setWhichColumn(0);
}

//---------------------------------------------
void ofxLabGui::setWhichPanel(string panelName){
    for(int i = 0; i < panels.size(); i++){
        if( panels[i]->name == panelName){
            setWhichPanel(i);
            setWhichColumn(0);
            return;
        }
    }
}

//---------------------------------------------
void ofxLabGui::setWhichGroup(int whichGroup){
	if( panels[currentPanel]->groups.size() <= 0 || whichGroup >= panels[currentPanel]->groups.size() )return;
    currentGroup = whichGroup;
	panels[currentPanel]->currentGroup = whichGroup;
    setWhichColumn(0);
}

//---------------------------------------------
void ofxLabGui::setWhichColumn(int column){
    if( currentPanel < 0 || currentPanel >= panels.size() )return;
    panels[currentPanel]->selectColumn(column);
}

// ############################################################## //
// ##
// ##      Manually activate a panel
// ##
// ############################################################## //

//----------------------------------------------
void ofxLabGui::setSelectedPanel( int whichPanel ){
	if (whichPanel < 0 || whichPanel >= panels.size()) return;
	selectedPanel = whichPanel;
	panels[whichPanel]->bSelected = true;

	for(int i = 0; i < panels.size(); i++){
		if (i != whichPanel){
			panels[i]->bSelected = false;
		}
	}
}

//---------------------------------------------
void ofxLabGui::setSelectedPanel(string panelName){
    for(int i = 0; i < panels.size(); i++){
        if( panels[i]->name == panelName){
            setSelectedPanel(i);
            return;
        }
    }
}


// ############################################################## //
// ##
// ##       Gui elements
// ##
// ############################################################## //

//-------------------------------
void ofxLabGui::setSliderWidth(int width){

}

//---------------------------------------------
guiTypeToggle * ofxLabGui::addToggle(string name, string xmlName, bool defaultValue){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new slider to our list
    guiTypeToggle * tmp = new guiTypeToggle();

    //setup and dimensions
    tmp->setup(name, (bool)defaultValue);
    tmp->setDimensions(14, 14);
    tmp->setPosition(0,0);
    tmp->setTypeBool();
    tmp->xmlName = xmlName;

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 1) );

	//either add to panel or group (if there are groups)
	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		tmp->fgColor = group->fgColor;
		tmp->bgColor = group->bgColor;
		group->addElement( tmp );
	} else {
		panels[currentPanel]->addElement( tmp );
	}
    guiObjects.push_back(tmp);
    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

    return tmp;
}

//---------------------------------------------
guiTypeButton* ofxLabGui::addButton(string buttonName){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
	
    //add a new slider to our list
    guiTypeButton* tmp = new guiTypeButton();
	
	tmp->setup(buttonName, guiButtonWidth, guiButtonHeight);
	tmp->setPosition(0, 0);
	ofAddListener(tmp->buttonPressed, this, &ofxLabGui::buttonPressed);
	
    //setup and dimensions
 //   tmp->setup(name, (bool)defaultValue);
  //  tmp->setDimensions(14, 200);
//    tmp->setPosition(0,0);
   // tmp->setTypeBool();
  //  tmp->xmlName = xmlName;
	
    //xmlObjects.push_back( xmlAssociation(tmp, xmlName, 1) );
	
	//either add to panel or group (if there are groups)
	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		tmp->fgColor = group->fgColor;
		tmp->bgColor = group->bgColor;
		group->addElement( tmp );
	} else {
		panels[currentPanel]->addElement( tmp );
	}
    guiObjects.push_back(tmp);
    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }
	
    return tmp;
}


//---------------------------------------------
guiTypeMultiToggle * ofxLabGui::addMultiToggle(string name, string xmlName, int defaultBox, vector <string> boxNames){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new multi toggle to our list
    guiTypeMultiToggle * tmp = new guiTypeMultiToggle();

    //setup and dimensions
    tmp->setup(name, defaultBox, boxNames);
    tmp->setDimensions(180, 60);
    tmp->xmlName = xmlName;

    //we can say we want to an int or a float!
    tmp->setTypeInt();

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 1) );
    guiObjects.push_back(tmp);

    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

	//either add to panel or group (if there are groups)
	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		tmp->fgColor = group->fgColor;
		tmp->bgColor = group->bgColor;
		group->addElement( tmp );
	} else {
		panels[currentPanel]->addElement( tmp );
	}

    return tmp;
}

//-------------------------------
guiTypeSlider * ofxLabGui::addSlider(string sliderName, string xmlName, float value , float min, float max, bool isInt){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new slider to our list
    guiTypeSlider * tmp = new guiTypeSlider();

    //setup and dimensions
    tmp->setup(sliderName, value, min, max);
    tmp->setDimensions(270, 10);
    tmp->xmlName = xmlName;

    //we can say we want to an int or a float!
    if(isInt){
        tmp->setTypeInt();
    }else{
        tmp->setTypeFloat();
    }

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 1) );
    guiObjects.push_back(tmp);
    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

	//either add to panel or group (if there are groups)
	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		tmp->fgColor = group->fgColor;
		tmp->bgColor = group->bgColor;
		group->addElement( tmp );
	} else {
		panels[currentPanel]->addElement( tmp );
	}

    return tmp;
}

//-------------------------------
guiType2DSlider * ofxLabGui::addSlider2D(string sliderName, string xmlName, float valueX, float valueY, float minX, float maxX, float minY, float maxY, bool isInt){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;

    //add a new slider to our list
    guiType2DSlider * tmp = new guiType2DSlider();

    //setup and dimensions
    tmp->setup(sliderName, valueX, minX, maxX, valueY, minY, maxY);
    tmp->setDimensions(180, 80);
    tmp->xmlName = xmlName;

    //we can say we want to an int or a float!
    if(isInt){
        tmp->setTypeInt();
    }else{
        tmp->setTypeFloat();
    }

    xmlObjects.push_back( xmlAssociation(tmp, xmlName, 2) );
    guiObjects.push_back(tmp);

    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

	//either add to panel or group (if there are groups)
	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		tmp->fgColor = group->fgColor;
		tmp->bgColor = group->bgColor;
		group->addElement( tmp );
	} else {
		panels[currentPanel]->addElement( tmp );
	}

    return tmp;
}



//---------------------------------------------
guiTypeDrawable * ofxLabGui::addDrawableRect(string name, ofBaseDraws * drawablePtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeDrawable * tmp = new guiTypeDrawable();

    tmp->setup(name, drawablePtr, drawW, drawH);

	//either add to panel or group (if there are groups)
	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		tmp->fgColor = group->fgColor;
		tmp->bgColor = group->bgColor;
		group->addElement( tmp );
	} else {
		panels[currentPanel]->addElement( tmp );
	}

    guiObjects.push_back(tmp);
    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }


    return tmp;
}

//---------------------------------------------
guiTypeCustom * ofxLabGui::addCustomRect(string name, guiCustomImpl * customPtr, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeCustom * tmp = new guiTypeCustom();

    tmp->setup(name, customPtr, drawW, drawH);

	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		tmp->fgColor = group->fgColor;
		tmp->bgColor = group->bgColor;
		group->addElement( tmp );
	} else {
		panels[currentPanel]->addElement( tmp );
	}

    guiObjects.push_back(tmp);

    if( bUseTTFFont ){
        tmp->setFont(&guiTTFFont);
    }

    return tmp;
}

//---------------------------------------------
guiTypeLogger * ofxLabGui::addLogger(string name, simpleLogger * logger, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeLogger * loggerType = new guiTypeLogger();

    loggerType->setup(name, logger, drawW, drawH);

	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		loggerType->fgColor = group->fgColor;
		loggerType->bgColor = group->bgColor;
		group->addElement( loggerType );
	} else {
		panels[currentPanel]->addElement( loggerType );
	}

    guiObjects.push_back(loggerType);

    if( bUseTTFFont ){
        loggerType->setFont(&guiTTFFont);
    }

    return loggerType;
}

//---------------------------------------------
guiTypeFileLister * ofxLabGui::addFileLister(string name, simpleFileLister * lister, int drawW, int drawH){
    if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeFileLister * listerType = new guiTypeFileLister();

    listerType->setup(name, lister, drawW, drawH);

	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		listerType->fgColor = group->fgColor;
		listerType->bgColor = group->bgColor;
		group->addElement( listerType );
	} else {
		panels[currentPanel]->addElement( listerType );
	}

    guiObjects.push_back(listerType);

    if( bUseTTFFont ){
        listerType->setFont(&guiTTFFont);
    }

    return listerType;
}

//---------------------------------------------
guiTypeText * ofxLabGui::addTextField(string name, string xmlName, string defaultValue, int drawW, int drawH){
	if( currentPanel < 0 || currentPanel >= panels.size() )return NULL;
    guiTypeText * guiTextField = new guiTypeText();

    guiTextField->setup(name, defaultValue);
    guiTextField->xmlName = xmlName;
    guiTextField->setDimensions(drawW, drawH);
    guiTextField->setTypeString();

	if (panels[currentPanel]->groups.size() > 0){
		guiTypeGroup * group = panels[currentPanel]->groups[currentGroup];
		guiTextField->fgColor = group->fgColor;
		guiTextField->bgColor = group->bgColor;
		group->addElement( guiTextField );
	} else {
		panels[currentPanel]->addElement( guiTextField );
	}

    guiObjects.push_back(guiTextField);
    xmlObjects.push_back( xmlAssociation(guiTextField, xmlName, 1) );

    if( bUseTTFFont ){
        guiTextField->setFont(&guiTTFFont);
    }

    return guiTextField;
};

// ############################################################## //
// ##
// ##       get and set values
// ##
// ############################################################## //

//---------------------------------------------
void ofxLabGui::setValueB(string xmlName, bool value,  int whichParam){
    for(int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
void ofxLabGui::setValueI(string xmlName, int value,  int whichParam){
    for(int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
void ofxLabGui::setValueF(string xmlName, float value,  int whichParam){
    for(int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValue(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
void ofxLabGui::setValueS(string xmlName, string value, int whichParam){
    for(int i = 0; i < guiObjects.size(); i++){
        if( guiObjects[i]->xmlName == xmlName){
            if( whichParam >= 0  ){
                guiObjects[i]->value.setValueS(value, whichParam);
                return;
            }
        }
    }
}

//---------------------------------------------
bool ofxLabGui::getValueB(string xmlName, int whichParam){
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueB(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxLabGui - paramater requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}

//---------------------------------------------
string ofxLabGui::getValueS(string xmlName, int whichParam, string defaultString){
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueS(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxLabGui - paramater requested %s doesn't exist - returning default value", xmlName.c_str());
    return defaultString;
}

//---------------------------------------------
float ofxLabGui::getValueF(string xmlName, int whichParam){
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueF(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxLabGui - paramater requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}

//---------------------------------------------
int ofxLabGui::getValueI(string xmlName, int whichParam){
    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL && xmlName == xmlObjects[i].xmlName ){
            if( whichParam >= 0 && whichParam < xmlObjects[i].numParams ){
                return xmlObjects[i].guiObj->value.getValueI(whichParam);
            }
        }
    }
    ofLog(OF_LOG_WARNING, "ofxLabGui - paramater requested %s doesn't exist - returning 0", xmlName.c_str());
    return 0;
}

//---------------------------------------------
void ofxLabGui::setSaveColor( int r, int g, int b ){
	saveButton->setBackgroundColor(r,g,b,255);
}

//---------------------------------------------
void ofxLabGui::setSaveSelectedColor( int r, int g, int b ){
	saveButton->setBackgroundSelectColor(r,g,b,255);
}

//---------------------------------------------
void ofxLabGui::setRestoreColor( int r, int g, int b ){
	restoreButton->setBackgroundColor(r,g,b,255);
}

//---------------------------------------------
void ofxLabGui::setRestoreSelectedColor( int r, int g, int b ){
	restoreButton->setBackgroundSelectColor(r,g,b,255);
}

//---------------------------------------------
void ofxLabGui::setLoadColor( int r, int g, int b ){
    loadButton->setBackgroundColor(r,g,b,255);
}

//---------------------------------------------
void ofxLabGui::setLoadSelectedColor( int r, int g, int b ){
	loadButton->setBackgroundSelectColor(r,g,b,255);
}

//---------------------------------------------
void ofxLabGui::setSaveAsColor( int r, int g, int b ){
	saveAsButton->setBackgroundColor(r,g,b,255);
}

//---------------------------------------------
void ofxLabGui::setSaveAsSelectedColor( int r, int g, int b ){
	saveAsButton->setBackgroundSelectColor(r,g,b,255);
}


// ############################################################## //
// ##
// ##       Groups: access + update
// ##
// ############################################################## //

//---------------------------------------------
void ofxLabGui::setGroupActive(string panelName, string groupName, bool bActive){
	for(int i = 0; i < panels.size(); i++){
		if( panels[i]->name == panelName){
			for (int j=0; j<panels[i]->groups.size(); j++){
				if( panels[i]->groups[j]->name == groupName){
					panels[i]->groups[j]->setActive(bActive);
				}
			}
		}
    }
}


// ############################################################## //
// ##
// ##       Settings to/from xml
// ##
// ############################################################## //

string ofxLabGui::getCurrentXMLFile(){
	return currentXmlFile;
}

//-----------------------------
void ofxLabGui::setIncrementSave(string incrmentalFileBaseName){
    incrementSaveName = incrmentalFileBaseName;
    incrementSave = true;
}
//-----------------------------
void ofxLabGui::disableIncrementSave(){
    incrementSave = false;
}

//-----------------------------
void ofxLabGui::loadSettings(string xmlFile){
	cout<<"LOADING "<<xmlFile<<endl;
    for(int i = 0; i < guiObjects.size(); i++)guiObjects[i]->loadSettings(xmlFile);

    currentXmlFile = xmlFile;

    settingsDirectory = currentXmlFile;

    int posLastSlash = settingsDirectory.rfind("/");
    if( posLastSlash > 0) settingsDirectory.erase(settingsDirectory.begin()+ posLastSlash+1, settingsDirectory.end()  );
    else settingsDirectory = "";

    //bool loaded = settings.loadFile(ofToDataPath(currentXmlFile, true));
	bool loaded = settings.loadFile(currentXmlFile);
	if (!loaded) ofLog(OF_LOG_WARNING, "xml load failed!");
    usingXml = true;

    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
				if ( xmlObjects[i].guiObj->dataType == SG_TYPE_STRING){
					string val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueS(j));
					xmlObjects[i].guiObj->value.setValueS(val, j);
				} else {
					float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));
					xmlObjects[i].guiObj->value.setValue(val, j);
				}
            }
        }
    }
}

//-----------------------------
void ofxLabGui::loadSettingsEvent( string & buttonName){
	ofFileDialogResult r = ofSystemLoadDialog("Load XML File", false);
	if(r.bSuccess){
		loadSettings(r.getPath());
	}
}

//-----------------------------
void ofxLabGui::reloadSettings(){
    for(int i = 0; i < guiObjects.size(); i++)guiObjects[i]->reloadSettings();

    if( currentXmlFile != "" ){

        settings.loadFile(currentXmlFile);
        usingXml = true;

        for(int i = 0; i < xmlObjects.size(); i++){
            if( xmlObjects[i].guiObj != NULL ){
                int numParams = xmlObjects[i].numParams;

                for(int j = 0; j < numParams; j++){
                    string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                    if ( xmlObjects[i].guiObj->dataType == SG_TYPE_STRING){
						string val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueS(j));
						xmlObjects[i].guiObj->value.setValueS(val, j);
					} else {
						float val = settings.getValue(str, xmlObjects[i].guiObj->value.getValueF(j));
						xmlObjects[i].guiObj->value.setValue(val, j);
					}
                }
            }
        }

    }
}

//-----------------------------
void ofxLabGui::reloadSettingsEvent( string & buttonName){
	reloadSettings();
}

//-------------------------------
void ofxLabGui::saveSettings(string xmlFile){
    for(int i = 0; i < guiObjects.size(); i++)guiObjects[i]->saveSettings(xmlFile);

    for(int i = 0; i < xmlObjects.size(); i++){
        if( xmlObjects[i].guiObj != NULL ){
            int numParams = xmlObjects[i].numParams;

            for(int j = 0; j < numParams; j++){
                string str = xmlObjects[i].xmlName+":val_"+ofToString(j);
                if ( xmlObjects[i].guiObj->dataType == SG_TYPE_STRING){
					settings.setValue(str, xmlObjects[i].guiObj->value.getValueS(j));
				} else {
					settings.setValue(str, xmlObjects[i].guiObj->value.getValueF(j));
				}
            }
        }
    }

    if( incrementSave ){
        string xmlName = incrementSaveName;
        xmlName += ofToString(ofGetYear()) +"-"+ ofToString(ofGetMonth()) +"-"+ ofToString(ofGetDay()) +"-"+ ofToString(ofGetHours()) +"-"+ ofToString(ofGetMinutes())+"-"+ ofToString(ofGetSeconds());
        xmlName += ".xml";
        xmlName = settingsDirectory + xmlName;

        settings.saveFile(xmlName);
    }
	cout<<"SAVING "<<xmlFile;
    settings.saveFile(xmlFile);
    currentXmlFile = xmlFile;
    usingXml = true;
}

//-----------------------------
void ofxLabGui::saveSettings(){
    saveSettings(currentXmlFile);
}

//-----------------------------
void ofxLabGui::saveSettingsEvent( string & buttonName){
	saveSettings();
}

//-----------------------------
void ofxLabGui::saveAsSettingsEvent( string & buttonName){
	ofFileDialogResult r = ofSystemSaveDialog("Save XML File", "Save XML File");
	string testPath = r.getPath();
	if(r.bSuccess){
		size_t found = testPath.find(".xml");
		if (found == string::npos) testPath += ".xml";
		saveSettings(testPath);
	}
}

//JG 12/21/10 now you can add buttons.  This will get called for every button so switch based on the button name
void ofxLabGui::buttonPressed( string & buttonName){
	pressedButtons[buttonName] = true;
}

//---------------------------------------------------------------
bool ofxLabGui::getButtonPressed(string buttonName)
{
	bool hasBeenPressed = pressedButtons[buttonName];
	pressedButtons[buttonName] = false;
	return hasBeenPressed;
}

//---------------------------------------------------------------
guiBaseObject * ofxLabGui::getElement( string name ){
    int len = guiObjects.size();
    for (int i=0; i<len; i++){
        if (guiObjects[i]->name == name){
            return guiObjects[i];
        }
    }
    return NULL;
}

//---------------------------------------------------------------
guiTypePanel * ofxLabGui::getSelectedPanel(){
    if ( selectedPanel >= panels.size() ) return NULL;
    return panels[selectedPanel];
}

//---------------------------------------------------------------
int ofxLabGui::getSelectedPanelIndex(){
    return selectedPanel;
}

// ############################################################## //
// ##
// ##       GET BUTTONS (for hooking into their events)
// ##
// ############################################################## //

//-----------------------------
guiTypeButton * ofxLabGui::getSaveButton(){
	return saveButton;
}

//-----------------------------
guiTypeButton * ofxLabGui::getRestoreButton(){
	return restoreButton;
}

//-----------------------------
guiTypeButton * ofxLabGui::getLoadButton(){
	return loadButton;
}

//-----------------------------
guiTypeButton * ofxLabGui::getSaveAsButton(){
	return saveAsButton;
};

// ############################################################## //
// ##
// ##       Visibilty
// ##
// ############################################################## //


//---------------------------
void ofxLabGui::setMinimized(bool bMinimize){
    minimize = bMinimize;
}

//---------------------------
void ofxLabGui::setDraggable(bool bDrag){
    bDraggable = bDrag;
}

//---------------------------
void ofxLabGui::setCollapsible(bool bCollapse){
    bCollapsible = bCollapse;
}

//-------------------------------
void ofxLabGui::show(){
    hidden = false;
}

//-------------------------------
void ofxLabGui::hide(){
    hidden = true;
}

//-------------------------------
void ofxLabGui::toggleView(){
    hidden = !hidden;
}

// ############################################################## //
// ##
// ##       Mouse Events
// ##
// ############################################################## //


//-------------------------------
void ofxLabGui::mousePressed(float x, float y, int button){
    if( hidden ) return;

    bool tabButtonPressed = false;

	if( bCollapsible && isInsideRect(x, y, minimizeButton)){
		if( bCollapsible ) minimize = !minimize;
	}else if( isInsideRect(x, y, topBar) && bDraggable){
        dragging = true;
        mouseDownPoint.set(x - boundingBox.x, y-boundingBox.y, 0);
    }else if(!minimize){
		bool bPanelClicked = false;
		bool bTextEnterSet = false;
		//check panel tabs
        for(int i = 0; i < panels.size(); i++){
            if ( !panels[i]->enabled ) continue;
			ofRectangle scaledTabRect = panels[i]->getTabRect();
			scaledTabRect.x += panels[i]->getPosX();
			scaledTabRect.y += getPosY();

            if( isInsideRect(x, y, scaledTabRect) && !bPanelClicked){
                selectedPanel = i;
				panels[i]->bSelected = true;
                tabButtonPressed = true;
				bPanelClicked = true;
                //break;
            } else {
				panels[i]->bSelected = false;
			}
        }
		if (!bPanelClicked)	panels[selectedPanel]->bSelected = true;
    }

    if(minimize == false && tabButtonPressed == false && isInsideRect(x, y, boundingBox) ){
        for(int i = 0; i < panels.size(); i++){
             if( i == selectedPanel )panels[i]->checkHit( x - hitArea.x, y - hitArea.y, button);
        }

		saveAsButton->checkHit(x, y, button);
		saveAsButton->updateGui(x, y, false, true);
		loadButton->checkHit(x, y, button);
		loadButton->updateGui(x, y, false, true);
		saveButton->checkHit(x, y, button);
		saveButton->updateGui(x, y, false, true);
		restoreButton->checkHit(x, y, button);
		restoreButton->updateGui(x, y, false, true);
    }

    prevMouse.set(x, y);
}


//-------------------------------
void ofxLabGui::mouseDragged(float x, float y, int button){
    if( hidden ) return;

    if(dragging)setPosition( MAX(0, x - mouseDownPoint.x), MAX(0, y -mouseDownPoint.y));
    else if(!minimize){
        for(int i = 0; i < panels.size(); i++){
            if( i == selectedPanel ){

                if(button){
                    panels[i]->updateGui( x - prevMouse.x, y - prevMouse.y, false, true);
                }else{
                    panels[i]->updateGui( x - hitArea.x, y - hitArea.y, false, false);
                }
            }
        }
    }

    prevMouse.set(x, y);
}

//-------------------------------
void ofxLabGui::mouseReleased(){
    if( hidden ) return;

    for(int i = 0; i < panels.size(); i++){
        panels[i]->release();
    }
	
	saveAsButton->release();
	loadButton->release();
	saveButton->release();
	restoreButton->release();

    dragging        = false;
    saveDown        = false;
    restoreDown     = false;
}

// ############################################################## //
// ##
// ##       Key Events
// ##
// ############################################################## //

//-------------------------------
void ofxLabGui::keyPressed(int key)
{
	for(int i = 0; i < panels.size(); i++){
		if( i == selectedPanel )panels[i]->keyPressed(key);
	}
};

//-------------------------------
void ofxLabGui::keyReleased(int key)
{
};

// ############################################################## //
// ##
// ##       Updater
// ##
// ############################################################## //

//-------------------------------
void ofxLabGui::update(){
    guiBaseObject::update();

    topBar           = ofRectangle(boundingBox.x, boundingBox.y, boundingBox.width, 20);
    minimizeButton   = ofRectangle(boundingBox.x + boundingBox.width - 24, boundingBox.y + 4, 20, 10 );

	// update buttons, move to corners
	if (bDrawBarOnTop){
		if (bCollapsible) saveButton->setPosition(boundingBox.x + borderWidth, boundingBox.y + borderWidth);
		else saveButton->setPosition(boundingBox.x + borderWidth, boundingBox.y + borderWidth);
	} else {
		if (bCollapsible) saveButton->setPosition(boundingBox.x + borderWidth, boundingBox.y + boundingBox.height - buttonHeight);
		else saveButton->setPosition(boundingBox.x + borderWidth, boundingBox.y + boundingBox.height - buttonHeight - borderWidth);
	}

	restoreButton->setPosition(saveButton->getPosX() + (buttonWidth + borderWidth), saveButton->getPosY());
	loadButton->setPosition(restoreButton->getPosX() + (buttonWidth + borderWidth), restoreButton->getPosY());
	saveAsButton->setPosition(loadButton->getPosX() + (buttonWidth + borderWidth), loadButton->getPosY());

    for(int i = 0; i < panels.size(); i++){
        panels[i]->update();
    }
}

//---------------------------------------------
void ofxLabGui::updateBoundingBox(){
    if(bShowText){
        //we need to update out hitArea because the text will have moved the gui down
        hitArea.y = boundingBox.y + topSpacing;
        //boundingBox.height = hitArea.height + displayText.getTextSingleLineHeight() + titleSpacing; //don't override the gui set height!
        boundingBox.width  = MAX( hitArea.width, displayText.getTextWidth() );
    }else{
         //we need to update out hitArea because the text will have moved the gui down
        hitArea.y = boundingBox.y;
        boundingBox.height = hitArea.height;
        boundingBox.width  = hitArea.width;
    }
}


// ############################################################## //
// ##
// ##       Drawing
// ##
// ############################################################## //

//-------------------------------
void ofxLabGui::draw(){
    if( hidden ) return;

    ofPushStyle();
    ofEnableAlphaBlending();

        float panelH = boundingBox.height;
        if( minimize ){
            panelH = 20;
        }

		if (bDrawBarOnTop || panels.size() == 0){
			glPushMatrix();
				glTranslatef(boundingBox.x, boundingBox.y, 0);
				//draw the background
				ofFill();
				ofSetColor(bgColor.getColor().r,bgColor.getColor().g,bgColor.getColor().b,bgColor.getColor().a);
				ofRect(0, 0, boundingBox.width, panelH);

				//draw the outline
				ofNoFill();
				ofSetColor(outlineColor.getColor().r,outlineColor.getColor().g,outlineColor.getColor().b, outlineColor.getColor().a);
				ofRect(0, 0, boundingBox.width, panelH);
				ofLine(0, 20, boundingBox.width, 20);
			glPopMatrix();
		} else {
			glPushMatrix();
				glTranslatef(boundingBox.x, (int) boundingBox.y + panels[0]->getPosY(), 0);
					//draw the background
				ofFill();
				ofSetColor(bgColor.getColor().r,bgColor.getColor().g, bgColor.getColor().b, bgColor.getColor().a);
				ofRect(0, 0, boundingBox.width, (int) panelH - panels[0]->getPosY());

				//draw the outline
				ofNoFill();
				ofSetColor(outlineColor.getColor().r,outlineColor.getColor().b,outlineColor.getColor().g,outlineColor.getColor().a);
				ofRect(0, 0, boundingBox.width, (int) panelH - panels[0]->getPosY());
				ofLine(0, 20, boundingBox.width, 20);
			glPopMatrix();
		}

		if (bCollapsible) ofRect(minimizeButton.x, minimizeButton.y, minimizeButton.width, minimizeButton.height);
		saveButton->render();
		restoreButton->render();
		loadButton->render();
		saveAsButton->render();
        ofPushMatrix();
            ofTranslate(2,0,0);
            ofSetColor(textColor.getColor().r,textColor.getColor().g, textColor.getColor().b, textColor.getColor().a);
            guiBaseObject::renderText();
        ofPopMatrix();

        if( !minimize ){

            //don't let gui elements go out of their panels
            //glEnable(GL_SCISSOR_TEST);
            //glScissor(boundingBox.x, ofGetHeight() - ( boundingBox.y + boundingBox.height - (-2 + topSpacing) ), boundingBox.width - borderWidth , boundingBox.height);
			//glScissor(boundingBox.x-1, ofGetHeight() - ( boundingBox.y + boundingBox.height+1 ), boundingBox.width+2, boundingBox.height+2);
                /*for(int i = 0; i < panelTabs.size(); i++){
                    if( i == selectedPanel){
                        ofPushStyle();
                            ofFill();
                            ofSetColor(fgColor.getSelectedColorF());
							ofBeginShape();
							ofVertex(panelTabs[i].x, panelTabs[i].y+panelTabs[i].height);
							ofVertex(panelTabs[i].x, panelTabs[i].y);
							ofVertex(panelTabs[i].x+panelTabs[i].width, panelTabs[i].y);
							ofVertex(panelTabs[i].x+panelTabs[i].width, panelTabs[i].y + panelTabs[i].height);
							ofEndShape(false);
                            //ofRect(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                            ofSetColor(outlineColor.getColorF());
                        ofPopStyle();
                    }
                    ofSetColor(outlineColor.getColorF());
                    ofNoFill();
					ofBeginShape();
					ofVertex(panelTabs[i].x, panelTabs[i].y+panelTabs[i].height);
					ofVertex(panelTabs[i].x, panelTabs[i].y);
					ofVertex(panelTabs[i].x+panelTabs[i].width, panelTabs[i].y);
					ofVertex(panelTabs[i].x+panelTabs[i].width, panelTabs[i].y + panelTabs[i].height);
					ofEndShape(false);
					//ofRect(panelTabs[i].x, panelTabs[i].y, panelTabs[i].width, panelTabs[i].height);
                }*/

            ofPushMatrix();{
                ofTranslate( hitArea.x, hitArea.y );
                
                int numEnabled = 0;
                for(int i = 0; i < panels.size(); i++){
                    if ( !panels[i]->enabled ) continue;
                    numEnabled++;
                }
                
                if ( numEnabled == 0 ) return;
                
                float w = boundingBox.width;
                w /= numEnabled;
                
                int index = 0;
                int lastfound = 0;
                
                for(int i = 0; i < panels.size(); i++){
                    if ( !panels[i]->enabled ) continue;
                    
                    ofRectangle tabRect = panels[i]->getTabRect();
                    float dest = w * index;
                    if ( lastfound > 0 ){
                        dest = min( (float) panels[lastfound]->getTabRect().x + panels[lastfound]->getTabRect().width, dest );
                        if ( dest + tabRect.width < panels[lastfound]->getTabRect().x + panels[lastfound]->getTabRect().width ){
                            dest = panels[lastfound]->getTabRect().x + panels[lastfound]->getTabRect().width;
                        }
                    }
                    index++;
                    lastfound = i;
                    panels[i]->setTabPosition(dest,  tabRect.y);
                    
                    if( i == selectedPanel ) panels[i]->render();
                    else panels[i]->renderTab();

                }
            } ofPopMatrix();

            //glDisable(GL_SCISSOR_TEST);
        }

    ofPopStyle();
}



