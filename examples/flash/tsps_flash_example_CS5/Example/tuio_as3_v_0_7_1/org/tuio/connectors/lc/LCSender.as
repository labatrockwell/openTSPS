package org.tuio.connectors.lc {
	
	import flash.net.LocalConnection;

	public class LCSender {

		private var debugListener:Function;
		private var localConnection:LocalConnection;
		private var connectionName:String;
		private var methodName:String;
		
		/**
		 * Constructor
		 * 
		 * @param	connectionName The name of the LocalConnection 
		 */
		public function LCSender(connectionName:String, methodName:String) {

			this.connectionName = connectionName;
			this.methodName = methodName;
			
		}
		
		/**
		 * Creates a sending LocalConnection with the name given in the constructor
		 * 
		 * @param	args The args to be sent
		 * 
		 * @return	true if sent successfully
		 */
		public function send(... args):Boolean {
			var lc:LocalConnection = new LocalConnection();
			args.unshift(this.methodName);
			args.unshift(this.connectionName);
			try{
				lc.send.apply(lc, args);
				return true;
			} catch (e:Error) {
				return false;
			}
			return false;
		}
		
		private function debug(msg:String):void {
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