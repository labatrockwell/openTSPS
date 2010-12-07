package org.tuio.gestures {
	
	import flash.display.DisplayObject;
	import flash.events.TransformGestureEvent;
	import flash.geom.Point;
	import flash.utils.getTimer;
	import org.tuio.TuioEvent;
	import org.tuio.TouchEvent;
	
	public class ZoomGesture extends TwoFingerMoveGesture {
		
		private var lastDistance:Number;
		
		public function ZoomGesture() {
			super();
		}
		
		public override function dispatchGestureEvent(target:DisplayObject, gsg:GestureStepSequence):void {
			var diffX:Number = gsg.getTuioContainer("A").X * gsg.getTuioContainer("B").X;
			var diffY:Number = gsg.getTuioContainer("A").Y * gsg.getTuioContainer("B").Y;
			if (diffX <= 0 || diffY <= 0) {                           
				var distance:Number = Math.sqrt(Math.pow(gsg.getTuioContainer("A").x - gsg.getTuioContainer("B").x, 2) + Math.pow(gsg.getTuioContainer("A").y - gsg.getTuioContainer("B").y, 2));
				var scale:Number = 0;
				lastDistance = Number(gsg.getValue("lD"));
				
				if (lastDistance != 0) {
					scale = distance - lastDistance;
				}
				
				gsg.getTarget("A").dispatchEvent(new TransformGestureEvent(TransformGestureEvent.GESTURE_ZOOM, true, false, null, 0, 0, scale, scale));
				gsg.storeValue("lD", distance);
			}
		}
		
	}
	
}