/*
NOTE: Creative Commons Attribution 3.0 License (http://creativecommons.org/licenses/by/3.0/)
You are free: to Share — to copy, distribute and transmit the work
You are free: to Remix — to adapt the work
Under the following conditions: 
Attribution. You must attribute the work in the manner specified by the author or licensor 
(but not in any way that suggests that they endorse you or your use of the work).
Feel free to download and modify the code for any use as long as our name is in there somewhere.
You may also use it in comercial projects without any licensing fees.
*/
package org.tuio.legacy{
	import flash.display.Sprite;
	
	 /**
	 * @class TUIOPropInterface.as
	 * @namespace org.tuio.legacy
	 * @author Frederic Friess and Johannes Luderschmidt (http://johannesluderschmidt.de/tuio-actionscript-3-open-source-fiducial-object-interaction-implementation/552/)
	 * 
	 * interfaces all possible events that can be dispatched for a fiducial. This interface should
	 * be implemented by a Tuio object handler (e.g. TestFiducialHandler).
	 * 
	 * Legacy PropObject from fiducialtuioas3 (http://code.google.com/p/fiducialtuioas3/).
	 * 
	 * For a newer version of a fiducial callback implementation see <code>org.tuio.fiducial.TuioFiducialDispatcher</code> and 
	 * <code>org.tuio.fiducial.ITuioFiducialReceiver</code>. 
	 */
	public interface TUIOPropInterface{
		
		/**
		 * will be executed everytime a fiducial to which this handler is applied is being
		 * added on the table/simulator.
		 */
		function onAdd(evt:PropEvent):void;
		
		/**
		 * will be executed everytime a fiducial to which this handler is applied is being
		 * removed from the table/simulator.
		 */
		function onRemove(evt:PropEvent):void;
		
		/**
		 * will be executed everytime a fiducial to which this handler is applied is being
		 * moved on the table / in the simulator.
		 */
		function onMove(evt:PropEvent):void;
		
		/**
		 * will be executed everytime a fiducial to which this handler is applied is being
		 * rotated on the table / in the simulator.
		 */
		function onRotate(evt:PropEvent):void;
		
		/**
		 * represents a standard Tuio property, added for completeness. Not used so far.
		 */
		function onMoveVelocety(evt:PropEvent):void;
		
		/**
		 * represents a standard Tuio property, added for completeness. Not used so far.
		 */
		function onRotateVelocety(evt:PropEvent):void;
		
		/**
		 * represents a standard Tuio property, added for completeness. Not used so far.
		 */
		function onMoveAccel(evt:PropEvent):void;
		
		/**
		 * represents a standard Tuio property, added for completeness. Not used so far.
		 */
		function onRotateAccel(evt:PropEvent):void;
	}	
}