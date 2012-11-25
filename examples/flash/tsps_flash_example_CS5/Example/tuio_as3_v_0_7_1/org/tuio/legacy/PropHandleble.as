package org.tuio.legacy{
	 /**
	 * Legacy PropHandleble from fiducialtuioas3 (http://code.google.com/p/fiducialtuioas3/).
	 * 
	 * For a newer version of a fiducial callback implementation see: 
	 * @see org.tuio.fiducial.TuioFiducialDispatcher
	 * @see org.tuio.fiducial.ITuioFiducialReceiver
	 * 
	 * @author Frederic Friess
	 * 
	 */
	public interface PropHandleble{
		function onAdd(evt:PropEvent):void;
		function onRemove(evt:PropEvent):void;
		function onMove(evt:PropEvent):void;
		function onRotate(evt:PropEvent):void;
		function onMoveVelocety(evt:PropEvent):void;
		function onRotateVelocety(evt:PropEvent):void;
		function onMoveAccel(evt:PropEvent):void;
		function onRotateAccel(evt:PropEvent):void;	
	}
}