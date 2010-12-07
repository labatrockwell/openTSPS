package org.tuio.gestures {
	
	import flash.display.DisplayObject;
	import flash.events.TransformGestureEvent;
	import flash.geom.Point;
	import flash.utils.getTimer;
	import org.tuio.TuioEvent;
	import org.tuio.TouchEvent;
	
	public class DragGesture extends OneFingerMoveGesture {
		
		private var lastPosition:Point;
		
		public function DragGesture() {
			super();
		}
		
		public override function dispatchGestureEvent(target:DisplayObject, gsg:GestureStepSequence):void {                         
			var diffX:Number = 0;
			var diffY:Number = 0;
			lastPosition = gsg.getValue("lP") as Point;
			
			if (lastPosition) {
				diffX = gsg.getTuioContainer("A").x * gsg.getTarget("A").stage.stageWidth - lastPosition.x;
				diffY = gsg.getTuioContainer("A").y * gsg.getTarget("A").stage.stageHeight - lastPosition.y;
			}
			
			gsg.getTarget("A").dispatchEvent(new TransformGestureEvent(TransformGestureEvent.GESTURE_PAN, true, false, null, 0, 0, 1, 1, 0, diffX, diffY));
			gsg.storeValue("lP", new Point(gsg.getTuioContainer("A").x * gsg.getTarget("A").stage.stageWidth, gsg.getTuioContainer("A").y * gsg.getTarget("A").stage.stageHeight) );
		}
		
	}
	
}