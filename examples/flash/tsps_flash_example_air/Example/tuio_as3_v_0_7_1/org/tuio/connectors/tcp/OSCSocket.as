package org.tuio.connectors.tcp
{
	import flash.events.DataEvent;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.events.SecurityErrorEvent;
	import flash.net.Socket;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	import org.tuio.osc.OSCEvent;
	import org.tuio.osc.OSCBundle;
	
	public class OSCSocket extends Socket
	{
		private var Debug:Boolean = true;
		private var Buffer:ByteArray = new ByteArray();
		private var PartialRecord:Boolean = false;
		private var isBundle:Boolean = false;
    	
		public function OSCSocket(){
			configureListeners();
		}

		private function configureListeners():void {
	        addEventListener(Event.CLOSE, closeHandler);
	        addEventListener(Event.CONNECT, connectHandler);
	        addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
	        addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler);
	        addEventListener(ProgressEvent.SOCKET_DATA, socketDataHandler);
	    }
	    
	    private function socketDataHandler(event:ProgressEvent):void {
	    
    		var data:ByteArray = new ByteArray();
    		if(PartialRecord){
    			Buffer.readBytes(data,0,Buffer.length);
    			PartialRecord = false;
    		}

        	super.readBytes(data,data.length,super.bytesAvailable);
   			
			var Length:int;
			
    		// While we have data to read
			while(data.position < data.length){
				
				isBundle = OSCBundle.isBundle(data);
				
				if (isBundle) { //check if the bytes are already a OSCBundle
					if (data.bytesAvailable > 20) { //there should be size information
						data.position += 16;
						if (data.readUTFBytes(1) != "#") {
							data.position -= 1;
							Length = data.readInt() + 20;
							data.position -= 20;
						} else {
							data.position -= 17;
							Length = 16;
						}
					} else { 
						Length = data.length+1;
					}
				} else {
					Length = data.readInt() + 4;
					data.position -= 4;
				}
					
				// If we have enough data to form a full packet.
				if(Length <= (data.length - data.position)){
					var packet:ByteArray = new ByteArray();
					if (isBundle) packet.writeInt(Length);
		    		data.readBytes(packet,packet.position,Length);
		    		packet.position = 0;
					this.dispatchEvent(new OSCEvent(packet));
		   		} else {
					// Read the partial packet
					Buffer = new ByteArray();
					data.readBytes(Buffer,0,data.length - data.position);
					PartialRecord = true;
		   		}
	    		
			}

	    }
	    
	    private function closeHandler(event:Event):void {
	        if(Debug)trace("Connection Closed");
	    }
	
	    private function connectHandler(event:Event):void {
	        if(Debug)trace("Connected");
	    }
	
	    private function ioErrorHandler(event:IOErrorEvent):void {
	        if(Debug)trace("ioErrorHandler: " + event);
	    }
	
	    private function securityErrorHandler(event:SecurityErrorEvent):void {
	        if(Debug)trace("securityErrorHandler: " + event);
	    }

	}
}