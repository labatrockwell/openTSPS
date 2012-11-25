package org.tuio.osc {
	
    public interface IOSCConnector {
		
		/**
		 * Adds a listener for incoming data to a private list. 
		 * @param	listener A listener for incoming data
		 */
		function addListener(listener:IOSCConnectorListener):void;
		
		/**
		 * Removes a listener for incoming data to a private list. 
		 * @param	listener A listener for incoming data
		 */
		function removeListener(listener:IOSCConnectorListener):void;
		
		/**
		 * Sends an OSCPacket via the connection type implemented by this IOSCConnector.
		 * @param	oscPacket The OSCPacket to be sent via this connection.
		 */
		function sendOSCPacket(oscPacket:OSCPacket):void;
		
    }
	
}