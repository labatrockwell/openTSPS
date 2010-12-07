package org.tuio.osc
{
	import flash.events.Event;
	import flash.utils.ByteArray;
	
	public class OSCEvent extends Event
	{
		public static var OSC_DATA:String = "OSCData";	
		public var data:ByteArray;
		public function OSCEvent(data:ByteArray)
		{
			super(OSC_DATA);
			this.data = data;
		}

	}
}