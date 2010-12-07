package org.tuio.osc {
	
	/**
	 * An OSCTimetag
	 * This is a helperclass for handling OSC timetags
	 * 
	 * @author Immanuel Bauer
	 */
	public class OSCTimetag {
		
		public var seconds:uint;
		public var picoseconds:uint;
		
		public function OSCTimetag(seconds:uint, picoseconds:uint) {
			this.seconds = seconds;
			this.picoseconds = picoseconds;
		}
		
		public function compareTo(otg:OSCTimetag):int {
			
			if (this.seconds > otg.seconds) return 1;
			else if (this.seconds < otg.seconds) return -1;
			else {
				if (this.picoseconds > otg.picoseconds) return 1;
				else if (this.picoseconds < otg.picoseconds) return -1;
				else return 0;
			}
			
			return 0;
			
		}
		
	}
	
}