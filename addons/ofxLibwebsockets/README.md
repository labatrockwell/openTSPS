ofxLibwebsockets
------------
* openframeworks wrapper for libwebsockets (http://git.warmcat.com/cgi-bin/cgit/libwebsockets/) 
* implements both client and server functionality of libwebsockets
* based on Paul Reimer's work on ofxWebUI (https://github.com/paulreimer/ofxWebUI) and jason vancleave's ofxLibWebSockets (https://github.com/jvcleave/ofxLibWebSockets)
* NOTE: uses our clone of libwebsockets https://github.com/labatrockwell/libwebsockets

STATUS
------------
* Server example currently works with Chrome, and Safari (need to test FF)
* Server and Client both support SSL, message sending/receiving, broadcasting, protocols and channels
* Client example currently tested with server example, echo.websockets.org (with and without SSL), local Autobahn ws server

TO-DO
------------
* Add functionality to examples
* More fun examples
* Try to add binary sending/receiving
* More testing of JSON parsing with jsoncpp (seems to be working)
* Compile for linux
