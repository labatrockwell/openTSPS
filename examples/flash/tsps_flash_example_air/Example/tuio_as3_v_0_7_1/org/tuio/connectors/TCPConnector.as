package org.tuio.connectors {
	
	import flash.net.Socket;
	import flash.utils.ByteArray;
	
	import org.tuio.osc.*;
	import org.tuio.connectors.tcp.OSCSocket;
	
	public class TCPConnector implements IOSCConnector {
		
		private var host:String;
		private var port:int;
		
		private var connection:OSCSocket;
		
		private var listeners:Array;
		
		public function TCPConnector(host:String = "127.0.0.1", port:int = 3333) {
			
			this.listeners = new Array();
			
			this.host = host;
			this.port = port;
			
			this.connection = new OSCSocket();
			this.connection.addEventListener(OSCEvent.OSC_DATA,receiveOscData);
			
			this.connection.connect(host, port);
		}
		
		public function receiveOscData(e:OSCEvent):void {
			var packet:ByteArray = new ByteArray();
			packet.writeBytes(e.data,4);
			packet.position = 0;
			if (packet != null) {
				if (this.listeners.length > 0) {
					//call receive listeners and push the received messages
					for each(var l:IOSCConnectorListener in this.listeners) {
						if (OSCBundle.isBundle(packet)) {
							l.acceptOSCPacket(new OSCBundle(packet));
						} else if (OSCMessage.isMessage(packet)) {
							l.acceptOSCPacket(new OSCMessage(packet));
						} else {
							this.debug("\nreceived: invalid osc packet.");
						}
					}
				}
			}
		}
		
		public function addListener(listener:IOSCConnectorListener):void {
			
			if (this.listeners.indexOf(listener) > -1) return;
			
			this.listeners.push(listener);
			
		}
		
		public function removeListener(listener:IOSCConnectorListener):void {
			
			var tmp:Array = this.listeners.concat();
			var newList:Array = new Array();
			
			var item:Object = tmp.pop();
			while (item != null) {
				if (item != listener) newList.push(item);
			}
			
			this.listeners = newList;
			
		}
		
		public function sendOSCPacket(oscPacket:OSCPacket):void {
			// Not Implemented
		}
		
		private function debug(msg:String):void {
			trace(msg);
		}
		
	}
	
}
	