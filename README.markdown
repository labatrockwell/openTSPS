About
------------
TSPS is a toolkit for sensing people in spaces. It uses openCV to analyze a camera, kinect, or video data, and sends what it finds via OSC, TCP, TUIO, or Websockets. 

Installing:
------------

* Mac OS X
  * web camera, video files, or Kinect: 
    * download and extract openTSPS_1.3_mac.zip
    * run openTSPS/openTSPS.app
  * ASUS Xtion
    * download and extract openTSPS_1.3_openNI_mac.zip
    * install drivers (thanks to James George for this tip):
      * easiest way is to use the package from Sensecast [http://sensecast.com/files/Sensecast.and.Libs.Installer.v.0.02.dmg](http://sensecast.com/files/Sensecast.and.Libs.Installer.v.0.02.dmg)   

* Windows
  * web camera, video files: 
    * download and extract openTSPS_1.3_win.zip
    * run openTSPS.exe
  * Kinect
    * download and extract openTSPS_1.3_win.zip
    * plug in your kinect
    * manually install drivers from openTSPS/drivers/Kinect
    * run openTSPS/openTSPS.exe

* All platforms
  * make sure to keep the "data" folder next to your app if you move it!  

Compiling:
------------
1. clone openframeworks from here: [https://github.com/openframeworks/openFrameworks](https://github.com/openframeworks/openFrameworks )
2. clone the TSPS directory into openframeworks/
3. open the project for your platform (Xcode of vs2010) in openTSPS/standalone/basic
4. compile and run!

Troubleshooting
------------
* If you have trouble compiling, experience crashes, or need any general help, please visit the [TSPS Forum] (http://opentsps.com/forum)

WebSockets
------------
* To turn on WebSocket communication (on by default in 1.1), select the Communication panel in the GUI and edit the WebSockets panel at the very bottom.
* To test your WebSocket, simply compile/run openTSPS, then navigate your browser to localhost:PORT, where PORT is the port that is specified in the GUI. By default it's localhost:7681.
* To have extra fun, open that same URL in a bunch of browser windows.
* To have the most fun, open up another computer to http://YOUR_IP_ADDRESS:PORT
* Making your own JS TSPS clients is easy (and they don't have to live in the bin/data/web directory); the TSPS WebSocket sender sends JSON, so you don't even have to use our JS files if you don't want to.
