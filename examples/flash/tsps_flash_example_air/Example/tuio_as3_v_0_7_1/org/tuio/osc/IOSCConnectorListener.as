package org.tuio.osc {
	
    public interface IOSCConnectorListener {
		
		function acceptOSCPacket(oscPacket:OSCPacket):void;
		
    }
	
}