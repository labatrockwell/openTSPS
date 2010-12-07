package org.tuio.fiducial
{
	import flash.display.DisplayObject;
	import flash.display.Stage;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.geom.Point;
	import flash.utils.*;
	
	import org.tuio.ITuioListener;
	import org.tuio.TuioBlob;
	import org.tuio.TuioCursor;
	import org.tuio.TuioObject;
	import org.tuio.debug.*;
	
	/**
	 * <p>The TuioFiducialDispatcher class offers an easy to use global listener concept for the use
	 * of fiducials in an application.</p>  
	 * 
	 * <p>Receivers can register themselves with <code>TuioFiducialDispatcher</code> in order to receive callbacks for a certain fiducial
	 * id. A receiver must implement <code>ITuioFiducialReceiver</code>.</p> 
	 * 
	 * @see ITuioFiducialReceiver
	 * @see FiducialEvent
	 * 
	 * @author Johannes Luderschmidt
	 */
	public class TuioFiducialDispatcher extends EventDispatcher implements ITuioListener
	{
		private static var allowInst:Boolean;
		private static var inst:TuioFiducialDispatcher;
		
		private var stage:Stage;
		
		private var receivers:Array;
		private var removalTimes:Array;
		
		private var ROTATION_MINIMUM:Number = 0.05;
		private var MOVEMENT_MINIMUM:Number = 3;
		
		private var _timeoutTime:Number;
		private var _rotationShift:Number;
		private var _invertRotation:Boolean;
		
		/**
		 * As TuioFiducialDispatcher has been implemented as a Singleton class this constructor cannot be 
		 * called directly. Call <code>init(...)</code> instead. 
		 * 
		 * @param stage
		 * @param timeoutTime
		 * @param target
		 * 
		 * @see init
		 * 
		 */
		public function TuioFiducialDispatcher(stage:Stage, timeoutTime:Number, rotationShift:Number, target:IEventDispatcher=null)
		{
			if (!allowInst) {
	            throw new Error("Error: Instantiation failed: Use TuioFiducialDispatcher.getInstance() instead of new.");
			}else{
				this.stage = stage;
				this._rotationShift = rotationShift;
				this._timeoutTime = timeoutTime;
				
				receivers = new Array();
				removalTimes = new Array();
			}
		}
		
		/**
		 * initializes Singleton instance of TuioFiducialDispatcher. Must be called before <code>getInstance()</code>.
		 * 
		 * @param stage
		 * @param timeoutTime time, which TuioFiducialDispatcher should wait until it calls the onRemove callback function
		 * of a receiver object after a tuio object has been removed from stage.
		 * @param rotationShift is a fixed degree value, which should be added to each fiducial rotation
		 * @return 
		 * 
		 */
		public static function init(stage:Stage, timeoutTime:Number = 1000, rotationShift:Number = 0):TuioFiducialDispatcher{
			if(inst == null){
				allowInst = true;
				inst = new TuioFiducialDispatcher(stage, timeoutTime, rotationShift);
				allowInst = false;
			}
			
			return inst;
		}
		
		/**
		 * 
		 * @return Singleton instance of TuioFiducialDispatcher.
		 * 
		 */
		public static function getInstance():TuioFiducialDispatcher{
			if(inst == null){
				throw new Error("Error: Not initialized: Use TuioFiducialDispatcher.init(...) first.");
			}
			return inst;
		}
		
		private function addObject(tuioObject:TuioObject):void{
			for each(var receiverObject:Object in receivers){
				if(receiverObject.classID == tuioObject.classID){
					if(receiverObject.tuioObject != null){
						//object is still existing because it had been lost while tracking
						//update object and stop timer
						(receiverObject.receiver as ITuioFiducialReceiver).onNotifyReturned(createFiducialEvent(
																								FiducialEvent.NOTIFY_RETURNED, 
																								tuioObject)
																							);
						//stop return timeout for this fiducial
						for(var i:Number = 0; i <  removalTimes.length; i++){
							var removalTimeObject:Object = removalTimes[i]; 
							if(removalTimeObject.receiverObject.classId == receiverObject.classId){
								clearTimeout(removalTimeObject.timeoutId);
								removalTimes.splice(i,1);
							}
						}
						callUpdateMethods(receiverObject, receiverObject.tuioObject, tuioObject);
					}else{
						//object does not exist yet. call add, move and rotation method callbacks.
						receiverObject.tuioObject = tuioObject.clone();
						(receiverObject.receiver as ITuioFiducialReceiver).onAdd(createFiducialEvent(
																					FiducialEvent.ADD, 
																					tuioObject)
																				);
						receiverObject.receiver.onMove(createFiducialEvent(FiducialEvent.MOVE, tuioObject));
						receiverObject.receiver.onRotate(createFiducialEvent(FiducialEvent.ROTATE,tuioObject));
					}
				}
			}
			
			//dispatch ADD event on DisplayObject under fiducial
			var stagePos:Point = new Point(stage.stageWidth * tuioObject.x, stage.stageHeight * tuioObject.y);
			getTopDisplayObjectUnderPoint(stagePos).dispatchEvent(createFiducialEvent(
				FiducialEvent.ADD, 
				tuioObject));
		}
		
		private function updateObject(tuioObject:TuioObject):void{
			for each(var receiverObject:Object in receivers){
				if(receiverObject.classID == tuioObject.classID){
					//compare rotation, movement and so on and call according callback methods
					callUpdateMethods(receiverObject, receiverObject.tuioObject, tuioObject);
				}
			}
			dispatchUpdateEvents(tuioObject);
		}
		
		private function dispatchUpdateEvents(newTuioObject:TuioObject):void{
			//dispatch MOVE event on DisplayObject under fiducial
			var stagePos:Point = new Point(stage.stageWidth * newTuioObject.x, stage.stageHeight * newTuioObject.y);
			getTopDisplayObjectUnderPoint(stagePos).dispatchEvent(createFiducialEvent(
				FiducialEvent.MOVE, 
				newTuioObject));
			//dispatch ROTATE event on DisplayObject under fiducial
			getTopDisplayObjectUnderPoint(stagePos).dispatchEvent(createFiducialEvent(
				FiducialEvent.ROTATE, 
				newTuioObject));
		}
		
		private function callUpdateMethods(receiverObject:Object, oldTuioObject:TuioObject, newTuioObject:TuioObject):void{
			if(oldTuioObject){
				//check for movement and rotation
				var distX:Number = oldTuioObject.x*stage.stageWidth-newTuioObject.x*stage.stageWidth;
				var distY:Number = oldTuioObject.y*stage.stageHeight-newTuioObject.y*stage.stageHeight;
				var dist:Number = Math.sqrt(distX*distX+distY*distY);
				var angleDist:Number = Math.abs(oldTuioObject.a - newTuioObject.a);
				
				//to prevent flickering of fiducial graphics updates are 
				//only triggered if movement or rotation exceed threshold
				if(dist > MOVEMENT_MINIMUM || angleDist > ROTATION_MINIMUM){
					if(dist > 0){
						receiverObject.receiver.onMove(createFiducialEvent(FiducialEvent.MOVE, newTuioObject));
					}
					if(angleDist > 0){
						receiverObject.receiver.onRotate(createFiducialEvent(FiducialEvent.ROTATE, newTuioObject));
					}
					receiverObject.tuioObject = newTuioObject.clone();
				}
			}else{
				//call move callback function
				receiverObject.receiver.onMove(createFiducialEvent(FiducialEvent.MOVE, newTuioObject));
				//call rotate callback function
				receiverObject.receiver.onRotate(createFiducialEvent(FiducialEvent.ROTATE, newTuioObject));
				//update tuioObject to be able to check for movement and rotation
				receiverObject.tuioObject = newTuioObject.clone();
			}
			
		}
		
		private function removeObject(tuioObject:TuioObject):void{
			for each(var receiverObject:Object in receivers){
				if(receiverObject.classID == tuioObject.classID){
					(receiverObject.receiver as ITuioFiducialReceiver).onNotifyRemoved(
																			createFiducialEvent(FiducialEvent.NOTIFY_REMOVED, tuioObject), 
																			_timeoutTime);
					var timeoutId:Number = setTimeout(checkTimeouts, _timeoutTime);
					var removalObject:Object = new Object();
					removalObject.timeout = getTimer()+_timeoutTime;
					removalObject.receiverObject = receiverObject;
					removalObject.timeoutId = timeoutId;
					removalTimes.push(removalObject);
					break;
				}
			}
			
			//dispatch REMOVED event on DisplayObject under fiducial
			var stagePos:Point = new Point(stage.stageWidth * tuioObject.x, stage.stageHeight * tuioObject.y);
			getTopDisplayObjectUnderPoint(stagePos).dispatchEvent(createFiducialEvent(
				FiducialEvent.REMOVED, 
				tuioObject));
		}
		
		private function checkTimeouts():void{
			var firstTimeout:Number = removalTimes[0].timeout;
			
			if(firstTimeout <= getTimer()){
				var removalTimeObject:Object = removalTimes.shift(); 
				(removalTimeObject.receiverObject.receiver as ITuioFiducialReceiver).onRemove(null);
				for(var i:Number = 0; i < receivers.length; i++){
					if(removalTimeObject.receiverObject.classID == receivers[i].classID){
						receivers[i].tuioObject = null;
					}
				}
			}
			if(removalTimes.length == 0){
				removeEventListener(Event.ENTER_FRAME, checkTimeouts);
			}
		}
		
		/**
		 * adds a callback object to TuioFiducialDispatcher.
		 * 
		 * @param receiver object that should be notified if a fiducial with the id fiducialId is changed.
		 * @param fiducialId fiducial id on which TuioFiducialDispatcher should listen
		 * 
		 */
		public function registerReceiver(receiver:ITuioFiducialReceiver, fiducialId:Number):void{
			var receiverObject:Object = new Object();
			receiverObject.receiver = receiver;
			receiverObject.classID = fiducialId;
			receivers.push(receiverObject);
		}
		
		/**
		 * removes a callback object from TuioFiducialDispatcher.
		 * 
		 * @param receiver object that should be notified if a fiducial with the id fiducialId is changed.
		 * @param fiducialId fiducial id on which TuioFiducialDispatcher should listen.
		 * 
		 */
		public function removeReceiver(receiver:ITuioFiducialReceiver, fiducialId:Number):void{
			var i:Number = 0;
			for each(var receiverObject:Object in receivers){
				if(receiverObject.receiver == receiver && receiverObject.classID == fiducialId){
					receivers.splice(i,1);
					break;
				}
				i = i+1;
			}
			
		}
		private function createFiducialEvent(type:String, tuioObject:TuioObject):FiducialEvent{
			var fiducialEvent:FiducialEvent = new FiducialEvent(type, true, true);
			fiducialEvent.x = tuioObject.x*stage.stageWidth;
			fiducialEvent.y = tuioObject.y*stage.stageHeight;
			fiducialEvent.fiducialId = tuioObject.classID;
			fiducialEvent.tuioObject = tuioObject;
			
			//calculate rotation
			if(!_invertRotation){
				fiducialEvent.rotation = tuioObject.a * 180 / Math.PI+_rotationShift;
			}else{
				var invertedValue:Number = 2*Math.PI - tuioObject.a; 
				fiducialEvent.rotation = invertedValue * 180 / Math.PI+_rotationShift;
			}
			
			//calculate local position
			var stagePos:Point = new Point(fiducialEvent.x, fiducialEvent.y);
			var localPos:Point = getTopDisplayObjectUnderPoint(stagePos).globalToLocal(stagePos);
			fiducialEvent.localX = localPos.x;
			fiducialEvent.localY = localPos.y;
			return fiducialEvent;
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
		 * time, which TuioFiducialDispatcher should wait until it calls the onRemove callback function
		 * of a receiver object after a tuio object has been removed from stage.
		 * 
		 * @return timeout time
		 * 
		 * @see ITuioFiducialReceiver
		 * 
		 */
		public function get timeoutTime():Number{
			return _timeoutTime;
		}
		public function set timeoutTime(timeoutTime:Number):void{
			_timeoutTime = timeoutTime;
		}
		
		
		/**
		 * Fixed degree value, which is added to the rotation value. Trackers behave differently in how they calculate
		 * the rotation of a fiducial on their surface. Thus, rotationShift can be set according to a 
		 * tracker's properties. The simulator does not need any shift.
		 * 
		 * @return rotationShift as degree value.
		 * 
		 */
		public function get rotationShift():Number{
			return _timeoutTime;
		}
		public function set rotationShift(rotationShift:Number):void{
			_rotationShift = rotationShift;
		}
		
		/**
		 * Some trackers invert the rotation of a fiducial. Thus, by setting invertRotation true
		 * the rotation of a fiducial will be inverted.
		 *  
		 * @return invertRotation
		 * 
		 */
		public function get invertRotation():Boolean{
			return _invertRotation;
		}
		public function set invertRotation(invertRotation:Boolean):void{
			_invertRotation = invertRotation;
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function addTuioObject(tuioObject:TuioObject):void {
			addObject(tuioObject);
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function updateTuioObject(tuioObject:TuioObject):void {
			updateObject(tuioObject);
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function removeTuioObject(tuioObject:TuioObject):void {
			removeObject(tuioObject);
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function addTuioCursor(tuioCursor:TuioCursor):void {
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function updateTuioCursor(tuioCursor:TuioCursor):void {
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function removeTuioCursor(tuioCursor:TuioCursor):void {
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function addTuioBlob(tuioBlob:TuioBlob):void {
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function updateTuioBlob(tuioBlob:TuioBlob):void {
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function removeTuioBlob(tuioBlob:TuioBlob):void {
		}
		
		/**
		 * @inheritDoc 
		 * 
		 */
		public function newFrame(id:uint):void {
		}
	}
}