package {
	import flash.display.Sprite;
	import flash.events.TransformGestureEvent;
	import org.tuio.TouchEvent;
	
	public class DragRotateScaleMe extends Sprite {
		
		private var curID:int = -1;
		
		public function DragRotateScaleMe(x:Number, y:Number, width:Number, height:Number) {
			this.graphics.beginFill(Math.random() * 0xcccccc);
			this.graphics.drawRect( -width / 2, -height / 2, width, height);
			this.graphics.endFill();
			this.x = x + width / 2;
			this.y = y + height / 2;
			
			this.addEventListener(TransformGestureEvent.GESTURE_PAN, handleDrag);
			this.addEventListener(TransformGestureEvent.GESTURE_ZOOM, handleScale);
			this.addEventListener(TransformGestureEvent.GESTURE_ROTATE, handleRotate);
			this.addEventListener(TouchEvent.TOUCH_DOWN, handleDown);
		}
		
		private function handleScale(e:TransformGestureEvent):void {
			this.scaleX += e.scaleX;
			this.scaleY += e.scaleY;
		}
		
		private function handleRotate(e:TransformGestureEvent):void {
			this.rotation += e.rotation;
		}
		
		private function handleDrag(e:TransformGestureEvent):void {
			this.x += e.offsetX;
			this.y += e.offsetY;
		}
		
		private function handleDown(e:TouchEvent):void {
			if (curID == -1) {
				stage.setChildIndex(this, stage.numChildren - 1);
				this.curID = e.tuioContainer.sessionID;
				stage.addEventListener(TouchEvent.TOUCH_UP, handleUp);
			}
		}
		
		private function handleUp(e:TouchEvent):void {
			if(e.tuioContainer.sessionID == this.curID){
				this.curID = -1;
				stage.removeEventListener(TouchEvent.TOUCH_UP, handleUp);
			}
		}
		
	}
	
}