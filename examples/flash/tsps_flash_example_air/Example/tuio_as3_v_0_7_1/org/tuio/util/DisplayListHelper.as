package org.tuio.util
{
	import flash.display.DisplayObject;
	import flash.display.InteractiveObject;
	import flash.display.Stage;
	import flash.geom.Point;
	
	import org.tuio.debug.ITuioDebugBlob;
	import org.tuio.debug.ITuioDebugCursor;
	import org.tuio.debug.ITuioDebugObject;
	import org.tuio.debug.ITuioDebugTextSprite;

	public class DisplayListHelper
	{
		
		public static function getTopDisplayObjectUnderPoint(point:Point, stage:Stage):DisplayObject {
			var targets:Array =  stage.getObjectsUnderPoint(point);
			var item:DisplayObject = (targets.length > 0) ? targets[targets.length - 1] : stage;
			
			while(targets.length > 0) {
				item = targets.pop() as DisplayObject;
				//ignore debug cursor/object/blob and send object under debug cursor/object/blob
				if((item is ITuioDebugCursor || item is ITuioDebugBlob || item is ITuioDebugObject || item is ITuioDebugTextSprite) && targets.length > 1){
					continue;
				}
				if (item.parent != null && !(item is InteractiveObject)) item = item.parent;
				if (item is InteractiveObject) {
					if ((item as InteractiveObject).mouseEnabled) return item;
				}
			}
			item = stage;
			
			return item;
		}
		
		public static function bubbleListCheck(obj:DisplayObject):Boolean {
			if (obj.parent != null) {
				return bubbleListCheck(obj.parent);
			} else {
				return false;
			}
		}
	}
}