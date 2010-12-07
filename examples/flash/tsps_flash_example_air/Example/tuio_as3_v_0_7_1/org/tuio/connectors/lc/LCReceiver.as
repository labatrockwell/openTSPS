package org.tuio.connectors.lc {
	
	import flash.utils.ByteArray;
	import flash.net.LocalConnection;
	import flash.events.StatusEvent;
	import flash.events.AsyncErrorEvent;
	
	/**
	 * A Class for establishing a receiving LocalConnection
	 * @author Immanuel Bauer
	 */
	public class LCReceiver {

		private var debugListener:Function;
		private var localConnection:LocalConnection;
		private var connectionName:String;
		private var addNumber:Boolean;
		private var lcClient:Object;
		
		/**
		 * Constructor
		 * 
		 * @param	connectionName The name of the LocalConnection 
		 * @param	addNumber If true a number is added to the name to avoid nameconflicts
		 * @param	lcClient An Object that contains a function that will be called via the LocalConnection
		 */
		public function LCReceiver(connectionName:String, lcClient:Object, addNumber:Boolean = true) {

			this.connectionName = connectionName;
			this.addNumber = addNumber;
			this.lcClient = lcClient;
			
			this.localConnection = new LocalConnection();
			this.localConnection.allowDomain('localhost');
			this.localConnection.allowDomain('*');
			this.localConnection.client = this.lcClient;
			
			var lclistener:Object = new Object();
			lclistener.onAsyncError = function(e:AsyncErrorEvent):void {
				debug("error"+e.toString());
			}
			
			this.localConnection.addEventListener(AsyncErrorEvent.ASYNC_ERROR, lclistener.onAsyncError);
		}
		
		/**
		 * Creates a LocalConnection with the name and client Object given in the constructor
		 * 
		 * @return true if connected successfully
		 */
		public function start():Boolean {
			this.stop();
			
			var retry:int = -1;
			var name:String;
			
			while(retry < 7){
				try {
					name = this.connectionName + ((addNumber && retry >= 0)?retry.toString():"");
					
					this.localConnection.connect(name);
					
					debug("connected as: "+name);
					return true;
					
				} catch (e:Error) {
					retry++;
					debug("retry");
				}
			}
			
			return false;
		}
		
		/**
		 * Stops the LocalConnection
		 * 
		 * @return true if the localconnection could be stopped
		 */
		public function stop():Boolean {
			try {
				if (this.localConnection != null) {
					this.localConnection.close();
				}
				return true;
			} catch (e:Error) {
				return false;
			}
			return false;
		}
		
		private function debug(msg:String):void {
			trace(msg);
			if (this.debugListener != null) {
				this.debugListener.call(NaN, msg);
			}
		}
		
		/**
		 * Sets the debuglistener. A Function that is called when the TUIOReceiver needs to trace
		 * a debug message to avoid unwanted trace output.
		 * The function hast to accept a String that contains the message.
		 * 
		 * @param	l The listener Function(params: String).
		 */
		public function setDebugListener(l:Function):void {
			this.debugListener = l;
		}
		
	}
	
}