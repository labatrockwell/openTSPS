Release Notes
------------
1.3 (9/10/12)
* Downloadable apps:
	* windows version with Kinect! (finally!)
 	* auto-learn background
 		* now you can have TSPS automatically capture the background on startup
 		* toggle the switch in the "background" tab of the gui
 		* set how many seconds you'd like to wait after startup
   	* flip horizontal / flip vertical / invert
    		* simple camera adjustments that should have been there all along... 
 	* Easy source switching
 		* radio buttons to switch sources under the "Camera" tab 
 		* sources include ofVideoGrabber (default), ofVideoPlayer, and Kinect
	* Video files
 		* use a pre-recorded video file as a source (see above)
 		* place videos you'd like to use in data/videos
	* Faster Haar + optical flow tracking
	 	* thanks to Kyle McDonald's fantastic ofxCv addon
	* ASUS camera support
 		* support for the ASUS Xtion cameras
 		* available (download) as openTSPS_openNI.zip
 		* compile from standalone/openni
	* lots of bug fixes + major refactor
		* now stores last used settings file
			* can be switched in data/settings/defaults.xml
	* Examples updated!
 		* Processing library simplified, friendlier 
	* WebSockets 100% supported
	* new property in WebSockets / OSC: "highest"
 		* represents the brightest point in a blob
 		* with Kinect / ASUS cameras, represents closest point to the camera
 		* allows for fine finger tracking, tracking the tops of people's heads (from above), etc

* Source
	* Major refactor! 
		* Thanks to James George (http://github.com/obviousjim) for tons of help with initial ofxCv refactor
	* New interchangeable "Source" and "Processor" classes
		* Allows you to easily add new Camera sources to TSPS or swap in a completely different Processor (default is ofxCv)
		* Kinect, VideoGrabber, VideoPlayer, and OpenNI Source examples are included
			* OpenNI example (standalone/openni) demonstrates how to add a custom Source
		* ofxCv, ofxOpenCv and (partial) ofxOpenNI Processor examples included in source
			* note: only ofxCv Processor is supported, but other classes show you how to write + swap in your own Processor
	* Much simpler to set up!

1.2.2 (5/10/12 - Mac Only)
* Added loading + playing video files through the GUI
* Using the LAB's ofxLibwebsockets addon to function as a WebSocket server and/or client
* Lots of bug fixes, including camera switching
* Updates to the standalone multi-camera application, which now isn't quite as terrible to use.

1.2 (1/31/12 - Mac Only)

* Updated OSC Message Sending format
* Added leading slash 
* Removed "PersonMoved" message, no only PersonUpdated is sent on every frame
* Added a "Use Legacy OSC" button to revert to old format for backwards compatability

1.1 (11/18/11 - Mac Only)

* WebSocket communication
* Kinect support
* Minimize: run TSPS in lean, mean, mini mode (tiny window, all on-screen drawing skipped)
* OF 007 compatible