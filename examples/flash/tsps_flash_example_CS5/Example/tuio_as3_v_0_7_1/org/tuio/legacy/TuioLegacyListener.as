package org.tuio.legacy
{
	import flash.display.DisplayObject;
	import flash.display.Stage;
	import flash.events.EventDispatcher;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.tuio.ITuioListener;
	import org.tuio.TuioBlob;
	import org.tuio.TuioClient;
	import org.tuio.TuioCursor;
	import org.tuio.TuioObject;
	import org.tuio.debug.ITuioDebugBlob;
	import org.tuio.debug.ITuioDebugCursor;
	import org.tuio.debug.ITuioDebugObject;

	/**
	 * Adopts function of <code>TUIO</code> class from Touchlib's Tuio AS3 framework. All functions of
	 * <code>TUIO</code> can be found within <code>TuioLegacyListener</code>. Additionally, <code>TuioLegacyListener</code>
	 * dispatches <code>TouchEvent</code>s Touchlib's Tuio AS3 framework. Thus, existing multitouch software that
	 * has been built with Touchlib's Tuio AS3 framework can be easily ported to the TUIO AS3 framework.
	 * 
	 * All existing calls to <code>TUIO</code> must be replaced with <code>TuioLegacyListener.getInstance()</code> and
	 * the import paths of TouchEvent have to be changed to <code>org.tuio.legacy.TouchEvent</code>. 
	 * 
	 * Following legacy functions have been transfered from TUIO to TuioLegacyListener:
	 * - listenForObject
	 * - getObjectById
	 *  
	 * <code>TuioLegacyListener</code> implements the interface <code>ITuioListener</code>. Thus, it is being called whenever a Tuio
	 * object, cursor or blob is being received. When a cursor (== a touch) is being received in the function 
	 * <code>addTuioCursor</code>, <code>addTuioCursor</code> looks if there is a <code>DisplayObject</code> under the cursor and dispatches a 
	 * <code>TouchEvent.MOUSE_DOWN</code> on this <code>DisplayObject</code>. Additionally, a <code>TouchEvent.MOUSE_DOWN</code> is being dispatched 
	 * on the stage.
	 * 
	 * <b>Use only for the port of existing code to TUIO AS3.</b>
	 * For the current Tuio event implementation see:
	 * @see org.tuio.TuioManager
	 * @see org.tuio.TouchEvent
	 *  
	 * For more information about this legacy Tuio implementation see: 
	 * @see TouchEvent
	 * @see TUIOObject
	 * 
	 * @author Johannes Luderschmidt
	 * 
	 */
	public class TuioLegacyListener extends EventDispatcher implements ITuioListener
	{
		private var stage:Stage;
		private var tuioClient:TuioClient;
		private var listenOnIdsArray:Array;
		
		private static var allowInst:Boolean;
		private static var inst:TuioLegacyListener; 
		
		public var connected:Boolean = true;
		
		
		public function TuioLegacyListener(stage:Stage, tuioClient:TuioClient){
			if (!allowInst) {
	            throw new Error("Error: Instantiation failed: Use TuioLegacyListener.getInstance() instead of new.");
			}else{
				this.stage = stage;
				this.tuioClient = tuioClient;
				this.tuioClient.addListener(this);
				this.listenOnIdsArray = new Array();
			}
			
		}
		
		/**
		 * initializes Singleton instance. Must be called before <code>getInstance()</code>.
		 *  
		 * @param stage
		 * @param tuioClient
		 * 
		 * @return singleton instance of <code>TuioLegacyListener</code>.
		 * 
		 */
		public static function init(stage:Stage, tuioClient:TuioClient):TuioLegacyListener{
			if(inst == null){
				allowInst = true;
				inst = new TuioLegacyListener(stage, tuioClient);
				allowInst = false;
			}
			
			return inst;
		}
		
		/**
		 * 
		 * @return singleton instance of <code>TuioLegacyListener</code>.
		 * 
		 */
		public static function getInstance():TuioLegacyListener{
			if(inst == null){
				throw new Error("Please initialize with method init(...) first!");
			}
			return inst;
		} 
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function addTuioObject(tuioObject:TuioObject):void{
			
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function updateTuioObject(tuioObject:TuioObject):void{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function removeTuioObject(tuioObject:TuioObject):void{
		}
		
		/**
		 * dispatches TouchEvent.MOUSE_DOWN and TouchEvent.MOUSE_OVER events on the DisplayObject under the touch. 
		 * Additionally, the same events are being dispatched on the stage object in order to provide the possibility 
		 * to objects to listen on TouchEvents that are being dispatched on the stage (useful for the implementation of 
		 * dragging and so on).
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function addTuioCursor(tuioCursor:TuioCursor):void{
			var dobj:DisplayObject;
			var stagePoint:Point;					
			var displayObjArray:Array;
			
			stagePoint = new Point((int)(stage.stageWidth*tuioCursor.x), (int)(stage.stageHeight*tuioCursor.y));					
			displayObjArray = this.stage.getObjectsUnderPoint(stagePoint);
//			dobj = null;
			
			if(displayObjArray.length > 0){							
//				dobj = displayObjArray[displayObjArray.length - 1];
				dobj = getTopDisplayObjectUnderPoint(stagePoint);
				var localPoint:Point = dobj.parent.globalToLocal(new Point(stagePoint.x, stagePoint.y));
				dobj.dispatchEvent(new MouseEvent(MouseEvent.CLICK));
				dobj.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_DOWN, true, false, stagePoint.x, stagePoint.y, localPoint.x, localPoint.y, 0, 0, dobj, false,false,false, true, 0,"2Dcur", tuioCursor.sessionID, tuioCursor.sessionID, 0));									
				dobj.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_OVER, true, false, stagePoint.x, stagePoint.y, localPoint.x, localPoint.y, 0, 0, dobj, false,false,false, true, 0,"2Dcur", tuioCursor.sessionID, tuioCursor.sessionID, 0));
			}
			stage.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_DOWN, true, false, stagePoint.x, stagePoint.y, stagePoint.x, stagePoint.y, 0, 0, null, false,false,false, true, 0,"2Dcur", tuioCursor.sessionID, tuioCursor.sessionID, 0));									
			stage.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_OVER, true, false, stagePoint.x, stagePoint.y, stagePoint.x, stagePoint.y, 0, 0, null, false,false,false, true, 0,"2Dcur", tuioCursor.sessionID, tuioCursor.sessionID, 0));
		}
		
		/**
		 * dispatches TouchEvent.MOUSE_MOVE events on the stage whenever a touch is being updated.
		 * 
		 * Additionally, TouchEvent.MOUSE_MOVE events are being dispatched on all receiver objects in
		 * the Array listenOnIdsArray, which listen on the id of the updated touch.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function updateTuioCursor(tuioCursor:TuioCursor):void{
			var stagePoint:Point = new Point((int)(stage.stageWidth*tuioCursor.x), (int)(stage.stageHeight*tuioCursor.y));
			stage.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_MOVE, true, false, stagePoint.x, stagePoint.y, stagePoint.x, stagePoint.y, 0, 0, null, false,false,false, true, 0,"2Dcur", tuioCursor.sessionID, tuioCursor.sessionID, 0));
			
			//legacy listener concept: dispatch MOUSE_MOVE event on objects in listener array
			var localPoint:Point;
			for each(var listeningObject:Object in listenOnIdsArray){
				if(listeningObject.id == tuioCursor.sessionID){
					localPoint = listeningObject.receiver.parent.globalToLocal(new Point(stagePoint.x, stagePoint.y));			
					listeningObject.receiver.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_MOVE, true, false, stagePoint.x, stagePoint.y, localPoint.x, localPoint.y, 0, 0, listeningObject.receiver, false,false,false, true, 0, "2Dcur", tuioCursor.sessionID, -1, 0));
				}	
			}
		}
		
		/**
		 * dispatches TouchEvent.MOUSE_UP events on the stage whenever a touch is being removed.
		 * 
		 * Additionally, TouchEvent.MOUSE_UP events are being dispatched on all receiver objects in
		 * the Array listenOnIdsArray, which listen on the id of the removed touch.
		 * 
		 * @param tuioCursor
		 */
		public function removeTuioCursor(tuioCursor:TuioCursor):void{
			var dobj:DisplayObject;
			var stagePoint:Point;					
			var displayObjArray:Array;
			
			stagePoint = new Point((int)(stage.stageWidth*tuioCursor.x), (int)(stage.stageHeight*tuioCursor.y));					
			displayObjArray = this.stage.getObjectsUnderPoint(stagePoint);
			dobj = null;
			
			if(displayObjArray.length > 0){							
				dobj = displayObjArray[displayObjArray.length - 1];
				
				//don't dispatch TouchEvent on debug cursor, debug object or debug blob but on the actual object
				if((dobj is ITuioDebugCursor || dobj is ITuioDebugBlob || dobj is ITuioDebugObject) && displayObjArray.length > 1){
					dobj = displayObjArray[displayObjArray.length - 2];
				}
				var localPoint2:Point = dobj.parent.globalToLocal(new Point(stagePoint.x, stagePoint.y));				
				dobj.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_UP, true, false, stagePoint.x, stagePoint.y, localPoint2.x, localPoint2.y, 0, 0, dobj, false,false,false, true, 0,"2Dcur", tuioCursor.sessionID, tuioCursor.sessionID, 0));									
			}
			
			stagePoint = new Point((int)(stage.stageWidth*tuioCursor.x), (int)(stage.stageHeight*tuioCursor.y));
			stage.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_UP, true, false, stagePoint.x, stagePoint.y, stagePoint.x, stagePoint.y, 0, 0, null, false,false,false, true, 0,"2Dcur", tuioCursor.sessionID, tuioCursor.sessionID, 0));
			
			//legacy listener concept: dispatch MOUSE_UP event on objects in listener array
			var localPoint:Point;
			for each(var listeningObject:Object in listenOnIdsArray){
				if(listeningObject.id == tuioCursor.sessionID){
					localPoint = listeningObject.receiver.parent.globalToLocal(new Point(stagePoint.x, stagePoint.y));			
					listeningObject.receiver.dispatchEvent(new TouchEvent(TouchEvent.MOUSE_UP, true, false, stagePoint.x, stagePoint.y, localPoint.x, localPoint.y, 0, 0, listeningObject.receiver, false,false,false, true, 0, "2Dcur", tuioCursor.sessionID, -1, 0));
					removeListenForObject(listeningObject.id, listeningObject.receiver);
				}	
			}
		}
		
		private function getTopDisplayObjectUnderPoint(point:Point):DisplayObject {
			var targets:Array =  stage.getObjectsUnderPoint(point);
			var item:DisplayObject = (targets.length > 0) ? targets[targets.length - 1] : stage;
			
			var topmostDisplayObject:DisplayObject;
			
			while(targets.length > 0) {
				item = targets.pop();
				//ignore debug cursor/object/blob and send object under debug cursor/object/blob
				if((item is ITuioDebugCursor || item is ITuioDebugBlob || item is ITuioDebugObject) && targets.length > 1){
					continue;
				}
				topmostDisplayObject = item;
				break;
			}
			if(topmostDisplayObject == null){
				topmostDisplayObject = stage;
			}
			
			return topmostDisplayObject;
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function addTuioBlob(tuioBlob:TuioBlob):void
		{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function updateTuioBlob(tuioBlob:TuioBlob):void
		{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function removeTuioBlob(tuioBlob:TuioBlob):void
		{
		}
		
		/**
		 * adds a receiver object that will be notified whenever
		 * a touch with the id id arrives.
		 *  
		 * @param id session id of touch
		 * @param receiver object 
		 * 
		 */
		public function listenForObject(id:Number, receiver:Object):void{
			var tmpObj:TUIOObject = getObjectById(id);			
			if(tmpObj != null){
				var listeningObject:Object = {id:id, receiver:receiver};
				listenOnIdsArray.push(listeningObject);
			}
		}
		
		/**
		 * removes a receiver object from the notification list.
		 * 
		 * @param id session id of touch
		 * @param receiver object
		 * 
		 */
		public function removeListenForObject(id:Number, receiver:Object):void {
			var i:Number = 0;
			for each(var listeningObject:Object in listenOnIdsArray){
				if(listeningObject.id == id && listeningObject.receiver == receiver){
					listenOnIdsArray.splice(i,1);					
				}
				i = i+1;
			}
		}
		
		/**
		 * takes over the duties of the function <code>getObjectById</code> from <code>TUIO</code>. <code>getObjectById</code> looks in 
		 * the global cursor list for the tuio element with the id id.
		 * If this element is being found it will be repackaged into a (legacy) <code>TUIOObject</code> instance and
		 * returned to the caller.
		 *  
		 * @param id session id of requested blob.
		 * @return TUIOObject with the blob with the id id
		 * 
		 */
		public function getObjectById(id:Number):TUIOObject {
			var tuioObject:TUIOObject;
			//returns mouse cursor as blob
			if(id == 0){
				tuioObject = new TUIOObject("mouse", 0, stage.mouseX, stage.mouseY, 0, 0, 0, 0, 10, 10, null);
			}else{
				//look for blob/cursor in list
				var objectArray:Array = tuioClient.tuioCursors;
				for(var i:int=0; i<objectArray.length; i++)  {
					if(objectArray[i].sessionID == id){
						var stagePoint:Point = new Point((int)(stage.stageWidth*objectArray[i].x), (int)(stage.stageHeight*objectArray[i].y));
						var diffPoint:Point = new Point((int)(stage.stageWidth*objectArray[i].X), (int)(stage.stageHeight*objectArray[i].Y));
						tuioObject = new TUIOObject("2Dcur", id, stagePoint.x, stagePoint.y, diffPoint.x, diffPoint.y,-1,0,0,0, null);
						break;
					}
				}
			}
			return tuioObject;
		}
		
		/**
		 * takes over the duties of the function getObjects from TUIO. getObjects puts all elements from  
		 * the global cursor list (tuioClient.getTuioCursors()) into TUIOObjects and adds them to an array
		 * that is being returned.
		 *  
		 * @return Array with all pravailing tuio cursors and objects. 
		 * 
		 */
		public function getObjects():Array {
			var objects:Array = new Array();
			
			var objectArray:Array = tuioClient.tuioCursors;
			for(var i:int=0; i<objectArray.length; i++)  {
				var tuioObject:TUIOObject;
				var stagePoint:Point = new Point((int)(stage.stageWidth*objectArray[i].x), (int)(stage.stageHeight*objectArray[i].y));
				var diffPoint:Point = new Point((int)(stage.stageWidth*objectArray[i].X), (int)(stage.stageHeight*objectArray[i].Y));
				tuioObject = new TUIOObject("2Dcur", objectArray[i].sessionID, stagePoint.x, stagePoint.y, diffPoint.x, diffPoint.y,-1,0,0,0, null);
				objects.push(tuioObject);
			}
			
			return objects;
		}	
		
		/**
		 * Called if a new frameID is received.
		 * @param	id The new frameID
		 */
		public function newFrame(id:uint):void{}
	}
}