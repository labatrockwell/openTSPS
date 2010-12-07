package org.tuio.gestures {
	import flash.events.Event;
	
	public class GestureStepEvent extends Event {
		
		public static const SATURATED:String = "saturated";
		public static const DEAD:String = "dead";
		
		private var _step:uint;
		private var _group:GestureStepSequence;
		
		function GestureStepEvent(type:String, step:uint, group:GestureStepSequence) {
			super(type, false, false);
			this._step = step;
			this._group = group;
		}
		
		public function get step():uint {
			return this._step;
		}
		
		public function get group():GestureStepSequence {
			return this._group;
		}
		
	}
	
}