package org.tuio {
	
	import flash.events.Event;
	
	/**
	 * The TuioEvent is an event equivalent of the ITuioListener callback functions.
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioEvent extends Event {
		
		/**Triggered if a new object was tracked in the tracking space.*/
		public static const ADD:String = "org.tuio.TuioEvent.add";
		/**Triggered if a tracked object was updated.*/
		public static const UPDATE:String = "org.tuio.TuioEvent.update";
		/**Triggered if a tracked object was removed from the tracking space.*/
		public static const REMOVE:String = "org.tuio.TuioEvent.remove";
		
		/**Triggered if a new object was tracked in the tracking space and was profiled as a TuioObject by the tracker.*/
		public static const ADD_OBJECT:String = "org.tuio.TuioEvent.addObject";
		/**Triggered if a new object was tracked in the tracking space and was profiled as a TuioCursor by the tracker.*/
		public static const ADD_CURSOR:String = "org.tuio.TuioEvent.addCursor";
		/**Triggered if a new object was tracked in the tracking space and was profiled as a TuioBlob by the tracker.*/
		public static const ADD_BLOB:String = "org.tuio.TuioEvent.addBlob";
		
		/**Triggered if a TuioObject was updated.*/
		public static const UPDATE_OBJECT:String = "org.tuio.TuioEvent.updateObject";
		/**Triggered if a TuioCursor was updated.*/
		public static const UPDATE_CURSOR:String = "org.tuio.TuioEvent.updateCursor";
		/**Triggered if a TuioBlob was updated.*/
		public static const UPDATE_BLOB:String = "org.tuio.TuioEvent.updateBlob";
		
		/**Triggered if a TuioObject was removed.*/
		public static const REMOVE_OBJECT:String = "org.tuio.TuioEvent.removeObject";
		/**Triggered if a TuioCursor was removed.*/
		public static const REMOVE_CURSOR:String = "org.tuio.TuioEvent.removeCursor";
		/**Triggered if a TuioBlob was removed.*/
		public static const REMOVE_BLOB:String = "org.tuio.TuioEvent.removeBlob";
		
		/**Triggere if there is a new frameID. The <code>tuioContainer</code> will be <code>null</code> */
		public static const NEW_FRAME:String = "org.tuio.TuioEvent.newFrame";
		
		private var _tuioContainer:TuioContainer;
		
		public function TuioEvent(type:String, tuioContainer:TuioContainer) {
			super(type, false, false);
			this._tuioContainer = tuioContainer;
		}
		
		public function get tuioContainer():TuioContainer {
			return this._tuioContainer;
		}
		
	}
	
}