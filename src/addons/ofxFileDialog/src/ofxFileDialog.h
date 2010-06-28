/*
 *  ofxFileDialog.h
 *  CYAPeopleVision
 *
 *  Created by Brett Renfer on 2/10/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 *	Expands ofxMacFileDialog to mac + win, adds "getStringFromSaveDialog" function
 *
 */

//BASED ON ofxMacFileDialog:

/*
 *  ofxMacFileDialog.h
 *  ofxMacFileDialog
 *
 *  Created by Chris Roby on 12/28/09.
 *  Copyright 2009 Hill Holliday. All rights reserved.
 * 
 */



#include <string>

using namespace std;

#ifndef WIN32
enum kDialogType {
	kDialogFile,
	kDialogFolder
};
#else
#include <windows.h>
#endif

class ofxFileDialog {
public:

#ifndef WIN32
	string getStringFromDialog(kDialogType type, char* windowTitle, char* windowMessage);
	string getStringFromSaveDialog(char* windowTitle, char* windowMessage);
#else
	string getStringFromDialog(char *filter="All Files (*.*)\0*.*\0", HWND owner=NULL);
	string getStringFromSaveDialog(char *filter="All Files (*.*)\0*.*\0", HWND owner=NULL);
#endif
};
