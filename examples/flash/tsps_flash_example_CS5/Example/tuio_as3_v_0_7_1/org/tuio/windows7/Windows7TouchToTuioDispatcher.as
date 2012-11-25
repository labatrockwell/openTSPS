package org.tuio.windows7
{
	import flash.display.DisplayObject;
	import flash.display.Stage;
	import flash.events.TouchEvent;
	import flash.geom.Point;
	import flash.ui.Multitouch;
	import flash.ui.MultitouchInputMode;
	
	import org.tuio.TuioContainer;
	import org.tuio.TuioCursor;
	import org.tuio.TuioManager;
	import org.tuio.debug.TuioDebug;
	import org.tuio.util.DisplayListHelper;

	public class Windows7TouchToTuioDispatcher{
		
		private var stage:Stage;
		private var useTuioManager:Boolean;
		private var useTuioDebug:Boolean;
		
		public function Windows7TouchToTuioDispatcher(stage:Stage, useTuioManager:Boolean = true, useTuioDebug:Boolean = true){
			this.stage = stage;
			this.useTuioManager = useTuioManager; 
			this.useTuioDebug = useTuioDebug;
			
			Multitouch.inputMode = MultitouchInputMode.TOUCH_POINT;
			stage.addEventListener(TouchEvent.TOUCH_BEGIN, touchBegin);
			stage.addEventListener(TouchEvent.TOUCH_MOVE, dispatchTouchMove);
			stage.addEventListener(TouchEvent.TOUCH_END, dispatchTouchUp);
			
			stage.addEventListener(TouchEvent.TOUCH_TAP, dispatchTap);
		}
		
		private function dispatchTap(event:TouchEvent):void{
			var stagePos:Point = new Point(event.stageX, event.stageY);
			var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
			var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
			
			target.dispatchEvent(new org.tuio.TouchEvent(org.tuio.TouchEvent.TAP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, createTuioContainer(event)));
		}
		
		private function touchBegin(event:TouchEvent):void{
			if(this.useTuioManager){
				TuioManager.getInstance().handleAdd(createTuioContainer(event));
			}else{
				var stagePos:Point = new Point(event.stageX, event.stageY);
				var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
				var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
				
				target.dispatchEvent(new org.tuio.TouchEvent(org.tuio.TouchEvent.TOUCH_DOWN, true, false, local.x, local.y, stagePos.x, stagePos.y, target, createTuioContainer(event)));
			}
			if(this.useTuioDebug){
				TuioDebug.getInstance().addTuioCursor(createTuioCursor(event));
			}
		}
		
		private function dispatchTouchMove(event:TouchEvent):void{
			if(this.useTuioManager){
				TuioManager.getInstance().handleUpdate(createTuioContainer(event));
			}else{
				var stagePos:Point = new Point(event.stageX, event.stageY);
				var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
				var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
				
				target.dispatchEvent(new org.tuio.TouchEvent(org.tuio.TouchEvent.TOUCH_MOVE, true, false, local.x, local.y, stagePos.x, stagePos.y, target, createTuioContainer(event)));
			}
			
			if(this.useTuioDebug){
				TuioDebug.getInstance().updateTuioCursor(createTuioCursor(event));
			}
		}
		
		private function dispatchTouchUp(event:TouchEvent):void{
			if(this.useTuioManager){
				TuioManager.getInstance().handleRemove(createTuioContainer(event));
			}else{
				var stagePos:Point = new Point(event.stageX, event.stageY);
				var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
				var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
				
				target.dispatchEvent(new org.tuio.TouchEvent(org.tuio.TouchEvent.TOUCH_UP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, createTuioContainer(event)));
			}
			
			if(this.useTuioDebug){
				TuioDebug.getInstance().removeTuioCursor(createTuioCursor(event));
			}
		}
		
		private function createTuioContainer(event:TouchEvent):TuioContainer{
			return new TuioContainer("2Dcur",event.touchPointID,event.stageX/stage.stageWidth, event.stageY/stage.stageHeight,0,0,0,0,0,0);
		}
		private function createTuioCursor(event:TouchEvent):TuioCursor{
			return new TuioCursor("2Dcur",event.touchPointID,event.stageX/stage.stageWidth, event.stageY/stage.stageHeight,0,0,0,0,0,0);
		}
	}
}