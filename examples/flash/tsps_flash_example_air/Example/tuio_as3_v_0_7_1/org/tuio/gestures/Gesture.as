package org.tuio.gestures {
	
	import flash.display.DisplayObject;
	import flash.events.EventDispatcher;
	/**
	 * This class is thought as a base class for creating custom gestures for the <code>GestureManager</code>.
	 * To create a custom gesture you have to extend this class and overwrite <code>dispatchGestureEvent</code> function. 
	 * 
	 * Gestures are described by a sequence of steps. If all steps are saturated the <code>dispatchGestureEvent</code> is called.
	 * For more information on gesture steps have a look at the <code>GestureStep</code> asdoc.
	 * 
	 * @author Immanuel Bauer
	 * 
	 */
	public class Gesture extends EventDispatcher {
			
		public static const SATURATED:uint = 1;
		public static const ALIVE:uint = 2;
		public static const PROGRESS:uint = 3;
		public static const DEAD:uint = 4;
		
		private var _steps:GestureStepSequence;
		private var init:Boolean;
		
		public function Gesture() {
			this._steps = new GestureStepSequence();
			this._steps.gesture = this;
		}
		
		/**
		 * Add a <code>GestureStep</code> at the end of the gesture step sequence.
		 * 
		 * @param	s The <code>GestureStep</code> to be added.
		 */
		protected function addStep(s:GestureStep):void {
			this._steps.addStep(s);
		}
		
		/**
		 * Returns the first <code>GestureStep</code> of the gesture's step sequence. 
		 * 
		 * @return The first gesture step of the gesture step sequence.
		 */
		public function get firstStep():GestureStep {
			return this._steps.firstStep;
		}
		
		/**
		 * Returns a copy of the gesture's step sequence.
		 * 
		 * @return A copy of the gesture's step sequence.
		 */
		public function get steps():GestureStepSequence {
			return this._steps.copy();
		}
		
		/**
		 * Is called if all steps of the gesture's step sequence are saturated.
		 * 
		 * @param	target The target <code>InteractiveObject</code>
		 * @param	gsg The <code>GestureStepSequence</code> object storing the aliases and values that have been stored according to the <code>GestureStep</code>'s definitions. 
		 */
		public function dispatchGestureEvent(target:DisplayObject, gsg:GestureStepSequence):void {
			trace("dispatching");
		}
		
	}
	
}