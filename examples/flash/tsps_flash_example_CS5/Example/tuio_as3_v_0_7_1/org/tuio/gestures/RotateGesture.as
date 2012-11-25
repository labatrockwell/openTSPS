package org.tuio.gestures {
	
	import flash.display.DisplayObject;
	import flash.events.TransformGestureEvent;
	import flash.geom.Point;
	import flash.utils.getTimer;
	import org.tuio.TuioEvent;
	import org.tuio.TouchEvent;
	
	public class RotateGesture extends TwoFingerMoveGesture {
		
		private var lastAngle:Number;
		
		public function RotateGesture() {
			super();
		}
		
		public override function dispatchGestureEvent(target:DisplayObject, gsg:GestureStepSequence):void {
			var center:Point = new Point((gsg.getTuioContainer("B").x + gsg.getTuioContainer("A").x)/2, (gsg.getTuioContainer("B").y + gsg.getTuioContainer("A").y)/2);
			var vector:Point;
			if (gsg.getTuioContainer("A").y > gsg.getTuioContainer("B").y) vector = new Point(gsg.getTuioContainer("A").x - gsg.getTuioContainer("B").x, gsg.getTuioContainer("A").y - gsg.getTuioContainer("B").y);
			else  vector = new Point(gsg.getTuioContainer("B").x - gsg.getTuioContainer("A").x, gsg.getTuioContainer("B").y - gsg.getTuioContainer("A").y);
			var length:Number = Math.sqrt(Math.pow(vector.x, 2) + Math.pow(vector.y, 2));
			var angle:Number = Math.acos( vector.x / length);
			lastAngle = Number(gsg.getValue("lA"));
			
			var rotation:Number = 0;
			
			if (lastAngle != 0) {
				rotation = 180*(angle - lastAngle)/Math.PI;
			} 
			gsg.storeValue("lA", angle);
			gsg.getTarget("A").dispatchEvent(new TransformGestureEvent(TransformGestureEvent.GESTURE_ROTATE, true, false, null, 0, 0, 0, 0, rotation)); ;
		}
		
	}
	
}