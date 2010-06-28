/*
 *  ofxMacFileDialog.cpp
 *  ofxMacFileDialog
 *
 *  Created by Chris Roby on 12/28/09.
 *  Copyright 2009 Hill Holliday. All rights reserved.
 *
 *
 *	Some code from http://www.openframeworks.cc/forum/viewtopic.php?f=10&t=955
 *  and other pieces from Zach Lieberman http://snipt.org/lVl
 *  and even more from http://free.abracode.com/cyclone/sources/New%20Sources/NavDialogs.cp
 *  so......basically I just pieced this together
 */

// Brett Renfer: added getStringFromSaveDialog function... totally based off of the above-mentioned
// http://free.abracode.com/cyclone/sources/New%20Sources/NavDialogs.cp

#include "ofxFileDialog.h"

#ifndef WIN32

CFURLRef GetOpenDialogForUser(kDialogType type, char* title, char* message)
{
	NavDialogCreationOptions dialogOptions;
	NavDialogRef dialog = NULL;
	NavReplyRecord replyRecord;
	CFURLRef fileAsCFURLRef = NULL;
	FSRef fileAsFSRef;
	OSStatus status;

	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions(&dialogOptions);
	require_noerr( status, CantGetNavOptions );

	dialogOptions.optionFlags = kNavNoTypePopup + kNavSupportPackages + kNavAllowOpenPackages;

	CFAllocatorRef alloc_default = kCFAllocatorDefault;  // = NULL;

	if (title != NULL) {
		CFStringRef cftitle = CFStringCreateWithCString(alloc_default,title,kCFStringEncodingMacRoman);
		dialogOptions.windowTitle = cftitle;
	}

	if (message != NULL) {
		CFStringRef cfmessage = CFStringCreateWithCString(alloc_default,message,kCFStringEncodingMacRoman);
		dialogOptions.message = cfmessage;
	}


	// Make the window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;

	// Create the dialog
	if (type == kDialogFile) {
		status = NavCreateGetFileDialog(&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
	} else if (type == kDialogFolder) {
		status = NavCreateChooseFolderDialog(&dialogOptions, NULL, NULL, NULL, &dialog);
	}
	require_noerr( status, CantCreateDialog );

	// Show it
	status = NavDialogRun(dialog);
	require_noerr( status, CantRunDialog );

	// Get the reply
	status = NavDialogGetReply(dialog, &replyRecord);
	require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );

	// If the user clicked "Cancel", just bail
	if ( status == userCanceledErr ) goto UserCanceled;

	// Get the file
	status = AEGetNthPtr(&(replyRecord.selection), 1, typeFSRef, NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
	require_noerr( status, CantExtractFSRef );

	// Convert it to a CFURL
	fileAsCFURLRef = CFURLCreateFromFSRef(NULL, &fileAsFSRef);

	// Cleanup
CantExtractFSRef:
UserCanceled:
	verify_noerr( NavDisposeReply(&replyRecord) );
CantGetReply:
CantRunDialog:
	NavDialogDispose(dialog);
CantCreateDialog:
CantGetNavOptions:
    return fileAsCFURLRef;
}

CFURLRef GetSaveDialogForUser(char* title, char* message)
{
	NavDialogCreationOptions dialogOptions;
	FSRef output_file;
	CFURLRef fileAsCFURLRef = NULL;
	OSStatus status;

	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions( &dialogOptions );
	require_noerr( status, CantGetNavOptions );

	dialogOptions.optionFlags = kNavNoTypePopup + kNavSupportPackages + kNavAllowOpenPackages;

	CFAllocatorRef alloc_default = kCFAllocatorDefault;  // = NULL;

	if (title != NULL) {
		CFStringRef cftitle = CFStringCreateWithCString(alloc_default,title,kCFStringEncodingMacRoman);
		dialogOptions.windowTitle = cftitle;
	}

	if (message != NULL) {
		CFStringRef cfmessage = CFStringCreateWithCString(alloc_default,message,kCFStringEncodingMacRoman);
		dialogOptions.message = cfmessage;
	}
	// Make the window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;

	NavDialogRef dialog;
	status = NavCreatePutFileDialog ( &dialogOptions, NULL, NULL, NULL, NULL, &dialog);
	require_noerr( status, CantCreateDialog );

	status = NavDialogRun(dialog);
	require_noerr( status, CantRunDialog );

	// get dialog reply
	NavReplyRecord reply;
	status = NavDialogGetReply(dialog, &reply);
	require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );

	//get file directory
	AEKeyword keyword;
	DescType actual_type;
	Size actual_size;
	FSRef output_dir;
	status = AEGetNthPtr(&(reply.selection), 1, typeFSRef, &keyword, &actual_type,
						 &output_dir, sizeof(output_file), &actual_size);
	require_noerr( status, CantExtractFSRef );

	UInt8 output_dir_name[1024];
	FSRefMakePath(&output_dir, output_dir_name, 1024 );

	// now get filename
	CFIndex len = CFStringGetLength(reply.saveFileName);
	if (len > 255)
		len = 255;
	UniChar output_filename[255];
	CFStringGetCharacters(reply.saveFileName, CFRangeMake(0, len), output_filename);

	// need to unlink the old file
	if ( reply.replacing )
	{
		FSRef oldfile;

		status = FSMakeFSRefUnicode(&output_dir, len, output_filename,
								 kTextEncodingUnicodeDefault,
								 &oldfile);
		if (status == noErr) status = FSDeleteObject(&oldfile);
		//overwrite failed!
		require_noerr( status, UserCanceled );
	}

	//create fsref again to new file (NOTE: this actually makes a file...)
	status = FSCreateFileUnicode( &output_dir, len, output_filename, kFSCatInfoNone,
								 NULL, &output_file, NULL );
	require_noerr( status, CantExtractFSRef );

	// Convert it to a CFURL
	fileAsCFURLRef = CFURLCreateFromFSRef(NULL, &output_file);

CantExtractFSRef:
UserCanceled:
	verify_noerr( NavDisposeReply(&reply) );
CantGetReply:
CantRunDialog:
	// cleanup dialog
	NavDialogDispose(dialog);
CantCreateDialog:
CantGetNavOptions:
    return fileAsCFURLRef;
}


//--------------------------------------------------------------
string ofxFileDialog::getStringFromDialog(kDialogType type, char* windowTitle, char* windowMessage) {
	CFURLRef cfUrl = NULL;

	cfUrl = GetOpenDialogForUser(type, windowTitle, windowMessage);

	CFStringRef cfString = NULL;

	string fileNameToLoad = "";

	if ( cfUrl != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );

		// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
		const int kBufferSize = 255;

		char folderURL[kBufferSize];
		Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);


		string url1 = folderURL;
		string url2 = "";
		string finalURL = url1 + "/" + url2;
		fileNameToLoad = finalURL;

		if (fileNameToLoad.length() > 0)
			fileNameToLoad.erase(fileNameToLoad.length()-1);
	}

	return fileNameToLoad;
}

//--------------------------------------------------------------
string ofxFileDialog::getStringFromSaveDialog (char* windowTitle, char* windowMessage){
	CFURLRef cfUrl = NULL;

	cfUrl = GetSaveDialogForUser(windowTitle, windowMessage);

	CFStringRef cfString = NULL;

	string fileNameToSave = "";

	if ( cfUrl != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );

		// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
		const int kBufferSize = 255;

		char folderURL[kBufferSize];
		Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);


		string url1 = folderURL;
		string url2 = "";
		string finalURL = url1 + "/" + url2;
		fileNameToSave = finalURL;

		if (fileNameToSave.length() > 0) fileNameToSave.erase(fileNameToSave.length()-1);
	}
	return fileNameToSave;

}

#else

//based in part at code found here: http://www.daniweb.com/forums/thread159753.html
//NOTE: using this in code::blocks, you have to make it a GUI application
// (Properties / Build Options > Type), which makes it not have a console.
// if you  want to use the console to trace stuff, comment out the if statement
	string ofxFileDialog::getStringFromDialog(char *filter, HWND owner){
		char fileName[MAX_PATH] = "";
		string fileNameString;

		OPENFILENAME openFileDialog;
		ZeroMemory(&openFileDialog, sizeof(openFileDialog));
		openFileDialog.lStructSize = sizeof(openFileDialog);
		openFileDialog.hwndOwner = owner;
		openFileDialog.lpstrFilter = filter;
		openFileDialog.lpstrFile = fileName;
		openFileDialog.nMaxFile = MAX_PATH;
		openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		openFileDialog.lpstrDefExt = "";

		if(GetOpenFileName(&openFileDialog) == TRUE) {
		    fileNameString = fileName;
		}
		return fileNameString;
	};

    string ofxFileDialog::getStringFromSaveDialog(char *filter, HWND owner){
		char fileName[MAX_PATH] = "";
		string fileNameString;

		OPENFILENAME saveFileDialog;
		ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));
		saveFileDialog.lStructSize = sizeof(saveFileDialog);
		saveFileDialog.hwndOwner = owner;
		saveFileDialog.lpstrFilter = filter;
		saveFileDialog.lpstrFile = fileName;
		saveFileDialog.nMaxFile = MAX_PATH;
		saveFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		saveFileDialog.lpstrDefExt = "";

		if(GetSaveFileName(&saveFileDialog) == TRUE) {
		    fileNameString = fileName;
		}
		return fileNameString;
	};


#endif
