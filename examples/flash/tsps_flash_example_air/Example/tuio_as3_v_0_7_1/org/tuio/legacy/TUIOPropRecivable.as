package org.tuio.legacy{
	import flash.display.Sprite;
	
	/**
	 * Legacy TUIOPropRecivable interface from fiducialtuioas3 (http://code.google.com/p/fiducialtuioas3/).
	 * 
	 * For a newer version of a fiducial callback implementation see <code>org.tuio.fiducial.TuioFiducialDispatcher</code> and 
	 * <code>org.tuio.fiducial.ITuioFiducialReceiver</code>.
	 * 
	 * @author Frederic Friess
	 * 
	 */
	public interface TUIOPropRecivable
	{
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