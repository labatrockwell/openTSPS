package org.tuio.osc {
	
	public class OSCManager implements IOSCConnectorListener {
		
		private var _connectorIn:IOSCConnector;
		private var _connectorOut:IOSCConnector;
		
		private var _currentPacket:OSCPacket;
		
		private var msgListener:Array;
		private var oscMethods:Array;
		private var oscAddressSpace:OSCAddressSpace;
		
		private var running:Boolean;
		
		public var usePatternMatching:Boolean = false;
		
		public function OSCManager(connectorIn:IOSCConnector = null, connectorOut:IOSCConnector = null, autoStart:Boolean = true) {
			
			this.msgListener = new Array();
			this.oscMethods = new Array();
			
			this._connectorIn = connectorIn;
			if(this._connectorIn != null) this._connectorIn.addListener(this);
			this._connectorOut = connectorOut;
			
			this.running = autoStart;
			
		}
		
		public function start():void {
			this.running = true;
		}

		public function stop():void {
			this.running = false;
		}
		
		public function set connectorIn(conn:IOSCConnector):void {
			if (this._connectorIn != null) {
				this._connectorIn.removeListener(this);
			}
			this._connectorIn = conn;
			this._connectorIn.addListener(this);
		}
		
		public function get connectorIn():IOSCConnector {
			return this._connectorIn;
		}
		
		public function set connectorOut(conn:IOSCConnector):void {
			this._connectorOut = conn;
		}
		
		public function get connectorOut():IOSCConnector {
			return this._connectorOut;
		}
		
		public function sendOSCPacket(oscPacket:OSCPacket):void {
			if(this._connectorOut){
				this._connectorOut.sendOSCPacket(oscPacket);
			}
		}
		
		public function get currentPacket():OSCPacket {
			return this._currentPacket;
		}
		
		public function acceptOSCPacket(oscPacket:OSCPacket):void {
			if (running) {
				this._currentPacket = oscPacket;
				this.distributeOSCPacket(this._currentPacket);
				oscPacket = null;
			}
		}
		
		private function distributeOSCPacket(packet:OSCPacket):void {
			if (packet is OSCMessage) {
				this.distributeOSCMessage(packet as OSCMessage);
			} else if (packet is OSCBundle) {
				var cont:Array = (packet as OSCBundle).subPackets;
				for each(var p:OSCPacket in cont) {
					this.distributeOSCPacket(p);
				}
			}
		}
		
		private function distributeOSCMessage(msg:OSCMessage):void {

			for each(var l:IOSCListener in this.msgListener) {
				l.acceptOSCMessage(msg);
			}
			
			if(this.oscMethods.length > 0){
				
				var oscMethod:IOSCListener;
				var oscMethods:Array;
				
				if (this.usePatternMatching) {
					oscMethods = this.oscAddressSpace.getMethods(msg.address);
					for each(l in oscMethods) {
						l.acceptOSCMessage(msg);
					}
				} else {
					oscMethod = this.oscMethods[msg.address];
					if (oscMethod != null) oscMethod.acceptOSCMessage(msg);
				}
			}
			
		}
		
		public function addMethod(address:String, listener:IOSCListener):void {
			this.oscMethods[address] = listener;
			this.oscAddressSpace.addMethod(address, listener);
		}
		
		public function removeMethod(address:String):void {
			this.oscMethods[address] = null;
			this.oscAddressSpace.removeMethod(address);
		}
		
		public function addMsgListener(listener:IOSCListener):void {
			if (this.msgListener.indexOf(listener) > -1) return;
			this.msgListener.push(listener);
		}
		
		public function removeMsgListener(listener:IOSCListener):void {
			var temp:Array = new Array();
			for each(var l:IOSCListener in this.msgListener) {
				if (l != listener) temp.push(l);
			}
			this.msgListener = temp.concat();
		}
		
	}
	
}