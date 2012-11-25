package org.tuio.connectors
{
	import flash.utils.ByteArray;
	
	import org.tuio.connectors.udp.OSCDatagramSocket;
	import org.tuio.osc.IOSCConnector;
	import org.tuio.osc.IOSCConnectorListener;
	import org.tuio.osc.OSCBundle;
	import org.tuio.osc.OSCEvent;
	import org.tuio.osc.OSCMessage;
	import org.tuio.osc.OSCPacket;

	/**
	 * <p>Currently UDP in Adobe Air is only supported in Air 2 beta. Additionally, this connector 
	 * only works under Windows and with Mac OS X.</p>
	 * 
	 * <p>I tested it with Air 2 beta 2 and it did NOT work under Mac OS X 10.5 with Flex Builder 3 but
	 * only wit Flash Builder 4 beta 2. With Flex Builder 3 I regularly 
	 * got a "VerifyError: Error #1014: Class flash.net::DatagramSocket could not be found."
	 * in adl.</p>  
	 * 
	 * <p>This function is currently only supported in Adobe Air 2 beta. Thus, as long as
	 * Air 2 is still beta following arrangements have to be made in your app to use 
	 * UDPConnector:
	 * <pre>
	 * 1. Download Adobe Air 2 Beta from Adobe: http://labs.adobe.com/technologies/air2/ 
	 * 2. Install it: http://labs.adobe.com/wiki/index.php/AIR_2:Release_Notes#How_to_overlay_the_Adobe_AIR_SDK_for_use_with_the_Flex_SDK
	 * 3. Set the SDK of your project to the SDK that supports Air 2.
	 * 4. If your updating an existing App don't forget to adjust the namespace in your app descriptor to "http://ns.adobe.com/air/application/2.0beta2" otherwise you will
	 * get the VerifyError as well under Windows.</pre></p>
	 * 
	 
	 * 
	 * @author Johannes Luderschmidt
	 * 
	 */
	public class UDPConnector implements IOSCConnector
	{
		private var connection:OSCDatagramSocket;
		private var listeners:Array;
		
		/**
		 * 
		 * @example The following code shows three approaches to initialize UDPConnector. Use only one of them:
		 * <listing version="3.0">
		 * //tracker runs on localhost on default port 3333
		 * var tuio:TuioClient = new TuioClient(new UDPConnector());
		 * //or 
		 * //tracker runs on 192.0.0.5 on default port 3333 
		 * var tuio:TuioClient = new TuioClient(new UDPConnector("192.0.0.5"));
		 * //or
		 * //tracker runs on 192.0.0.5 on port 3334
		 * var tuio:TuioClient = new TuioClient(new UDPConnector("192.0.0.5",3334));
		 * </listing>
		 * 
		 * @param host ip of the tracker resp. tuio message producer.
		 * @param port of the tracker resp. tuio message producer.
		 * @param connect If true the <code>UDPConnector</code> will try to bind the given IP:port and to receive packets. If false the <code>UDPConnector</code> connect to the given IP:port and will wait for calls of <code>UDPConnector.sendOSCPacket()</code>
		 *
		 */
		public function UDPConnector(host:String = "127.0.0.1", port:int = 3333, bind:Boolean = true)
		{
			this.listeners = new Array();
			
			this.connection = new OSCDatagramSocket(host, port, bind);
			this.connection.addEventListener(OSCEvent.OSC_DATA,receiveOscData);
		}
		
		/**
		 * parses an incoming OSC message.
		 * 
		 * @param e
		 * 
		 */
		public function receiveOscData(e:OSCEvent):void {
			var packet:ByteArray = new ByteArray();
			packet.writeBytes(e.data);
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
							//this.debug("\nreceived: invalid osc packet.");
						}
					}
				}
			}
			
			packet = null;
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function addListener(listener:IOSCConnectorListener):void
		{
			if (this.listeners.indexOf(listener) > -1) return;
			
			this.listeners.push(listener);
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function removeListener(listener:IOSCConnectorListener):void
		{
			var tmp:Array = this.listeners.concat();
			var newList:Array = new Array();
			
			var item:Object = tmp.pop();
			while (item != null) {
				if (item != listener) newList.push(item);
			}
			
			this.listeners = newList;
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function sendOSCPacket(oscPacket:OSCPacket):void
		{
			if (this.connection.connected) this.connection.send(oscPacket.getBytes());
			else throw new Error("Can't send if not connected.");
		}
		
	}
}