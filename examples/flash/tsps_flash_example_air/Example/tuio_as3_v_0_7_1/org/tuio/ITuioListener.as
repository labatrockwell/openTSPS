package org.tuio {
	
	/**
	 * This Interface defines callback functions that will be called by the TuioClient if the 
	 * implementing class was added as a listener to the according TuioClient.
	 */
    public interface ITuioListener {
		
		/**
		 * Called if a new object was tracked.
		 * @param	tuioObject The values of the received /tuio/**Dobj.
		 */
		function addTuioObject(tuioObject:TuioObject):void;
		
		/**
		 * Called if a tracked object was updated.
		 * @param	tuioObject The values of the received /tuio/**Dobj.
		 */
		function updateTuioObject(tuioObject:TuioObject):void;
		
		/**
		 * Called if a tracked object was removed.
		 * @param	tuioObject The values of the received /tuio/**Dobj.
		 */
		function removeTuioObject(tuioObject:TuioObject):void;

		
		/**
		 * Called if a new cursor was tracked.
		 * @param	tuioObject The values of the received /tuio/**Dcur.
		 */
		function addTuioCursor(tuioCursor:TuioCursor):void;
		
		/**
		 * Called if a tracked cursor was updated.
		 * @param	tuioCursor The values of the received /tuio/**Dcur.
		 */
		function updateTuioCursor(tuioCursor:TuioCursor):void;
		
		/**
		 * Called if a tracked cursor was removed.
		 * @param	tuioCursor The values of the received /tuio/**Dcur.
		 */
		function removeTuioCursor(tuioCursor:TuioCursor):void;
		
		
		/**
		 * Called if a new blob was tracked.
		 * @param	tuioBlob The values of the received /tuio/**Dblb.
		 */
		function addTuioBlob(tuioBlob:TuioBlob):void;

		/**
		 * Called if a tracked blob was updated.
		 * @param	tuioBlob The values of the received /tuio/**Dblb.
		 */
		function updateTuioBlob(tuioBlob:TuioBlob):void;
		
		/**
		 * Called if a tracked blob was removed.
		 * @param	tuioBlob The values of the received /tuio/**Dblb.
		 */
		function removeTuioBlob(tuioBlob:TuioBlob):void;
		
		/**
		 * Called if a new frameID is received.
		 * @param	id The new frameID
		 */
		function newFrame(id:uint):void;
		
    }
	
}