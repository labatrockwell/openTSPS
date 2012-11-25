package org.tuio.osc {
	
    public interface IOSCListener {
		
		function acceptOSCMessage(oscmsg:OSCMessage):void;
		
    }
	
}