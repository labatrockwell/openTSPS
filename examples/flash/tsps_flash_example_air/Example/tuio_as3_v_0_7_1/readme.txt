#-------------------------------------#
#     TUIO ActionScript3 library      #
#-------------------------------------#

WEB: http://bubblebird.at/tuioflash/
SVN: http://code.google.com/p/tuio-as3/

AUTHORS
---------------------------------------
 Immanuel Bauer
 Dean North
 Johannes Luderschmidt
 
CHANGES
---------------------------------------
 - 0.71: ~ Some hotfixes for the 0.7 release
		 ~ Added examples to the package
		 ~ Added DragGesture
 - 0.7: ~ GestureManager -> Gestures based on native GestureEvents + system to easily create new gestures (-> there's already a post on this on the blog). Currently a very simple rotate and scale gesture are included.
		~ Native TouchEvents -> are handled like MouseEvents and are dispatched alongside the original TouchEvents. Because of the namingconflict I'll rename the TouchEvent to TuioTouchEvent
		~ UDPConnector (for AIR2 only)
		~ TCPConnector now supports binary streams without the additional int. Although this could cause problems with very complex/nested OSCBundles
		~ Fiducials (commited by Johannes)
		~ Windows7Touch to Tuio (commited by Johannes) - pretty much native TouchEvents to tuio touchevents
		~ Mouse to Touch (commited by Johannes) - for easier testing without the need of a touch setup
		~ Debug utilities (commited by Johannes) - for easily creating touch indicators 
 - 0.6: TuioManager -> TouchEvents and TuioEvents; Bugfixes; Performance improvements
 - 0.5: initial release 
 
ABOUT
---------------------------------------
 The Tuio as3 library is a framework for receiving and processing
 tracking data within flash to create e.g. multitouch applications.
 It was written completely from scratch with extensibility in mind.
 Antoher focus was backwards compatibilty with already established
 interfaces that were used to handle Tuio data in Flash until now,
 in order to make porting old projects as easy and swift as possible.
 You can find more on the library here -> http://bubblebird.at/tuioflash/tuio-as3-library/
 
 
INSTALLATION
---------------------------------------
 Move the "org" folder to where you keep your ActionScript libraries
 or simply copy it in the same folder as your Flash/as3 project.
 
 
USE
---------------------------------------
 The easiest way to use the library is by using the TuioManager class as
 explained in this article -> http://bubblebird.at/tuioflash/guides/using-the-tuiomanager/
 
 ... or if you want more control on what to do with the received tracking data
 and have the best possible performance you can implement the ITuioListener interface
 like it is explained in this article -> http://bubblebird.at/tuioflash/guides/creating-a-project-using-callbacks/
 
 ... or you can use the legacy module to quickly port on off you old projects
 like it is explained here -> http://bubblebird.at/tuioflash/guides/porting-an-old-tcpxml-project/
 
 The Tuio tracker you use will have to support either TCP or
 LocalConnection or you can use a bridge/gateway for either method.
 First place to look for Tuio software -> http://www.tuio.org/?software
 
DEMO
---------------------------------------
 Check out the demos folder. Simply copy the files from the folders into a Flash or Flex project and use the class named
 like the containing folder either as main or add it to the stage.
 

 
If you have any questions or suggestions please do not hesitate to 
contact us via our website or the google repository.