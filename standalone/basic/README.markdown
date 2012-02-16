Notes
------------
1. implementation of libwebsockets in OF is from https://github.com/paulreimer/ofxWebUI, but the version of libwebsockets is newer, so it will work in Chrome 15+ and browsers that use the newer WebSocket spec

WebSockets
------------
To make WebSockets work (they're on by default right now), select the 
Communication panel in the GUI and edit the WebSockets panel at the
very bottom. 

To test your WebSocket, simply compile + run openTSPS, then navigate
your browser to localhost:PORT, where PORT is the port that is specified
in the GUI. By default it's localhost:7681.

To have extra fun, open that same URL in a bunch of browser windows. 

By default right now, the html/js/css that is served is in bin/data/web.
You'll see some super barebones TSPS javascript stuff in there. Hack
away at it, please.