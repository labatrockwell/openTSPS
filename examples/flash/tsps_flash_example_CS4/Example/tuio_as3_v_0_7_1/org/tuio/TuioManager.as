package org.tuio {
	
	import flash.display.DisplayObject;
	import flash.display.InteractiveObject;
	import flash.display.Stage;
	import flash.events.EventDispatcher;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	import flash.utils.getTimer;
	
	import org.tuio.debug.ITuioDebugBlob;
	import org.tuio.debug.ITuioDebugCursor;
	import org.tuio.debug.ITuioDebugObject;
	import org.tuio.debug.ITuioDebugTextSprite;
	import org.tuio.fiducial.*;
	
	/**@eventType org.tuio.TuioEvent.ADD*/
	[Event(name = "org.tuio.TuioEvent.add", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.UPDATE*/
	[Event(name = "org.tuio.TuioEvent.update", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.REMOVE*/
	[Event(name = "org.tuio.TuioEvent.remove", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.ADD_OBJECT*/
	[Event(name = "org.tuio.TuioEvent.addObject", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.UPDATE_OBJECT*/
	[Event(name = "org.tuio.TuioEvent.updateObject", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.REMOVE_OBJECT*/
	[Event(name = "org.tuio.TuioEvent.removeObject", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.ADD_CURSOR*/
	[Event(name = "org.tuio.TuioEvent.addCursor", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.UPDATE_CURSOR*/
	[Event(name = "org.tuio.TuioEvent.updateCursor", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.REMOVE_CURSOR*/
	[Event(name = "org.tuio.TuioEvent.removeCursor", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.ADD_BLOB*/
	[Event(name = "org.tuio.TuioEvent.addBlob", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.UPDATE_BLOB*/
	[Event(name = "org.tuio.TuioEvent.updateBlob", type = "org.tuio.TuioEvent")]
	/**@eventType org.tuio.TuioEvent.REMOVE_BLOB*/
	[Event(name = "org.tuio.TuioEvent.removeBlob", type = "org.tuio.TuioEvent")]
	
	/**
	 * The TuioManager class implements the ITuioListener interface and triggers events 
	 * into Flash's event flow according to the called callback functions.
	 * <p><b>Triggered events</b></p>
	 * <ul>
	 * 	<li>TuioEvent: Is triggered on the TuioManager itself.</li>
	 * 	<li>TouchEvent: Is triggered on DisplayObjects under the tracked point according to the TuioManager's settings.</li>
	 * </ul>
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioManager extends EventDispatcher implements ITuioListener {
		
		/**The number of milliseconds within two subsequent taps trigger a double tap.*/
		public var doubleTapTimeout:int = 300;
		
		/**The maximum distance between two subsequent taps on the x/y axis to be counted as double tap in px*/
		public var doubleTapDistance:Number = 10;
		 
		/**The time between a touch down event and a hold event in ms*/
		public var holdTimeout:int = 500;
		
		/**If set true a TouchEvent is triggered if a TuioObject is received. The default is false.*/
		public var triggerTouchOnObject:Boolean = false;
		
		/**If set true a TouchEvent is triggered if a TuioBlob is received. The default is false.*/
		public var triggerTouchOnBlob:Boolean = false;	
		
		/**Sets the method how to discover the TouchEvent's target object. The default is TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED.*/
		public var touchTargetDiscoveryMode:uint = TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED;
		
		//the possible touch target discovery modes.
		/**The events will be triggered on the top object under the tracked point. Fastest method. Works for DisplayObject and subclasses.*/
		public static const TOUCH_TARGET_DISCOVERY_NONE:Number = 0;
		/**The InteractiveObject's mouseEnabled parameter is used to determine whether a TouchEvent is triggered on an InteractiveObject under the tracked point. Works only for InteractiveObject and subclasses.*/
		public static const TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED:Number = 1;
		/**An ignore list is used to determin whether a TouchEvent is triggered on an InteractiveObject under the tracked point. Works for DisplayObject and subclasses.*/
		public static const TOUCH_TARGET_DISCOVERY_IGNORELIST:Number = 2;
		
		//if true MouseEvents are dispatched alongside the TouchEvents
		private var _dispatchMouseEvents:Boolean = false;
		
		//if true native TouchEvents are dispatched alongside the org.tuio.TouchEvents
		private var _dispatchNativeTouchEvents:Boolean = false;
		
		private var _tuioClient:TuioClient;
		private var lastTarget:Array;
		private var firstTarget:Array;
		private var tapped:Array;
		private var hold:Array;
		
		private var ignoreList:Array;
		
		private var stage:Stage;

		private var touchReceiversDict:Dictionary;
		
		private static var allowInst:Boolean;
		private static var inst:TuioManager;
		
		/**
		 * Creates a new TuioManager instance which processes the Tuio tracking data received by the given TuioClient.
		 * 
		 * @param	stage The Stage object of the Flashmovie.
		 * @param	tuioClient A TuioClient instance that receives Tuio tracking data from a tracker.
		 */
		public function TuioManager(stage:Stage, tuioClient:TuioClient) {
			if (!allowInst) {
				throw new Error("Error: Instantiation failed: Use TuioManager.getInstance() instead of new.");
			}else{
				this._tuioClient = tuioClient;
				this._tuioClient.addListener(this);
				this.stage = stage;
				this.lastTarget = new Array();
				this.firstTarget = new Array();
				this.tapped = new Array();
				this.hold = new Array();
				this.ignoreList = new Array();
				this.touchReceiversDict = new Dictionary();
			}
		}
		
		/**
		 * initializes Singleton instance.
		 * 
		 * @param	stage The Stage object of the Flashmovie.
		 * @param	tuioClient A TuioClient instance that receives Tuio tracking data from a tracker.
		 */
		public static function init(stage:Stage, tuioClient:TuioClient):TuioManager{
			if(inst == null){
				allowInst = true;
				inst = new TuioManager(stage, tuioClient);
				allowInst = false;
			}
			
			return inst;
		}
		
		/**
		 * gets Singleton instance
		 */
		public static function getInstance():TuioManager{
			if(inst == null){
				throw new Error("Please initialize with method TuioManager.init(...) first!");
			}
			return inst;
		}
		
		public function handleAdd(tuioContainer:TuioContainer):void {
			var stagePos:Point = new Point(stage.stageWidth * tuioContainer.x, stage.stageHeight * tuioContainer.y);
			var target:DisplayObject = getTopDisplayObjectUnderPoint(stagePos);
			var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
			
			firstTarget[tuioContainer.sessionID] = target;
			lastTarget[tuioContainer.sessionID] = target;
			hold[tuioContainer.sessionID] = getTimer();
			
			//target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_OVER, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			//target.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OVER, false, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_DOWN, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			this.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_DOWN, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			
			if (_dispatchMouseEvents) {
				//target.dispatchEvent(new MouseEvent(MouseEvent.MOUSE_OVER, true, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
				//target.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OVER, false, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
				target.dispatchEvent(new MouseEvent(MouseEvent.MOUSE_DOWN, true, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
			}
			
			if (_dispatchNativeTouchEvents) {
				target.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_BEGIN, true, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, target as InteractiveObject));
			}
			
		}
		
		public function handleUpdate(tuioContainer:TuioContainer):void {
			var stagePos:Point = new Point(stage.stageWidth * tuioContainer.x, stage.stageHeight * tuioContainer.y);
			var target:DisplayObject = getTopDisplayObjectUnderPoint(stagePos);
			var targetDict:Dictionary = createDict(stage.getObjectsUnderPoint(stagePos));
			var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
			var last:DisplayObject = lastTarget[tuioContainer.sessionID];
			
			//mouse move or hold
			if (Math.abs(tuioContainer.X) > 0.001 || Math.abs(tuioContainer.Y) > 0.001 || Math.abs(tuioContainer.Z) > 0.001) {
				hold[tuioContainer.sessionID] = getTimer();
				target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_MOVE, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
				this.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_MOVE, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
				if (_dispatchMouseEvents) {
					target.dispatchEvent(new MouseEvent(MouseEvent.MOUSE_MOVE, true, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
				}
				if (_dispatchNativeTouchEvents) {
					target.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_MOVE, true, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, target as InteractiveObject));
				}
			} else if (hold[tuioContainer.sessionID] < getTimer() - holdTimeout) {
				hold[tuioContainer.sessionID] = getTimer();
				target.dispatchEvent(new TouchEvent(TouchEvent.HOLD, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
				this.dispatchEvent(new TouchEvent(TouchEvent.HOLD, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			}
			
			//mouse out/over
			if (target != last) {
				var lastLocal:Point = last.globalToLocal(new Point(stagePos.x, stagePos.y));
				var lastAncestors:Array = createAncestorList(last);
				var ancestors:Array = createAncestorList(target);
				var lastAncestorLocal:Point = new Point(lastLocal.x+last.x, lastLocal.y+last.y);
				var ancestorLocal:Point = new Point(local.x+target.x, local.y+target.y);
				var la:DisplayObject = lastAncestors.pop();
				while (la != null && la == ancestors.pop()) {
					la = lastAncestors.pop();
				}
				if (la != null) lastAncestors.push(la);
				
				if (_dispatchMouseEvents) {
					target.dispatchEvent(new MouseEvent(MouseEvent.MOUSE_OVER, true, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
					target.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OVER, false, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
				}
				if (_dispatchNativeTouchEvents) {
					target.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_OVER, true, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, target as InteractiveObject));
					target.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_ROLL_OVER, false, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, target as InteractiveObject));
				}
				
				if (ancestors.indexOf(last) < 0) {
					if (_dispatchMouseEvents) {
						last.dispatchEvent(new MouseEvent(MouseEvent.MOUSE_OUT, true, false, local.x, local.y, (last as InteractiveObject), false, false, false, false, 0));
						last.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OUT, false, false, local.x, local.y, (last as InteractiveObject), false, false, false, false, 0));
					}
					if (_dispatchNativeTouchEvents) {
						last.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_OUT, true, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, last as InteractiveObject));
						last.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_ROLL_OUT, false, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, last as InteractiveObject));
					}
					last.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_OUT, true, false, lastLocal.x, lastLocal.y, stagePos.x, stagePos.y, last, tuioContainer));
					last.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OUT, false, false, local.x, local.y, stagePos.x, stagePos.y, last, tuioContainer));
					this.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_OUT, true, false, lastLocal.x, lastLocal.y, stagePos.x, stagePos.y, last, tuioContainer));
					this.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OUT, false, false, local.x, local.y, stagePos.x, stagePos.y, last, tuioContainer));
					
					for each(var a:InteractiveObject in lastAncestors) {
						if(a != target){
							a.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OUT, false, false, lastAncestorLocal.x, lastAncestorLocal.y, stagePos.x, stagePos.y, a, tuioContainer));
							this.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OUT, false, false, lastAncestorLocal.x, lastAncestorLocal.y, stagePos.x, stagePos.y, a, tuioContainer));
							if (_dispatchMouseEvents) {
								a.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OUT, false, false, lastAncestorLocal.x, lastAncestorLocal.y, a, false, false, false, false, 0));
							}
							if (_dispatchNativeTouchEvents) {
								a.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_ROLL_OUT, false, false, tuioContainer.sessionID, false, lastAncestorLocal.x, lastAncestorLocal.y, 0, 0, 0, a));
							}
						}
						lastAncestorLocal.x += a.x;
						lastAncestorLocal.y += a.y;
					}
					
				} else {
					var ta:InteractiveObject = ancestors.pop();
					while (last != ta && ta != null) {
						ta = ancestors.pop();
					}
				}
				
				if (lastAncestors.indexOf(target) < 0) {
					target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_OVER, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
					target.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OVER, false, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
					for each(var b:InteractiveObject in ancestors) {
						b.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OVER, false, false, ancestorLocal.x, ancestorLocal.y, stagePos.x, stagePos.y, b, tuioContainer));
						this.dispatchEvent(new TouchEvent(TouchEvent.ROLL_OVER, false, false, ancestorLocal.x, ancestorLocal.y, stagePos.x, stagePos.y, b, tuioContainer));
						if (_dispatchMouseEvents) {
							b.dispatchEvent(new MouseEvent(MouseEvent.ROLL_OVER, false, false, ancestorLocal.x, ancestorLocal.y, b, false, false, false, false, 0));
						}
						if (_dispatchNativeTouchEvents) {
							b.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_ROLL_OVER, false, false, tuioContainer.sessionID, false, ancestorLocal.x, ancestorLocal.y, 0, 0, 0, b));
						}
						ancestorLocal.x += b.x;
						ancestorLocal.y += b.y;
					}
				}
			}
			
			lastTarget[tuioContainer.sessionID] = target;
			
			//handle updates on receivers: call updateTouch from each receiver that listens on sessionID
			if(this.touchReceiversDict[tuioContainer.sessionID]){
				for each(var receiver:ITuioTouchReceiver in this.touchReceiversDict[tuioContainer.sessionID]){
					receiver.updateTouch(new TouchEvent(TouchEvent.TOUCH_MOVE, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
				}
			}
		}
		
		/**
		 * adds a touch callback class to TuioManager that listens on a certain sessionId.
		 * 
		 * @param receiver callback class.
		 * @param sessionId of the cursor/blob to listen to.
		 * 
		 */
		public function registerReceiver(receiver:ITuioTouchReceiver, sessionId:Number):void{
			if(!this.touchReceiversDict[sessionId]){
				this.touchReceiversDict[sessionId] = new Array();
			}
			this.touchReceiversDict[sessionId].push(receiver);	
		}
		
		public function removeReceiver(receiver:ITuioTouchReceiver, sessionId:Number):void{
//			var i:Number = 0;
//			for each(var receiverObject:Object in receivers){
//				if(receiverObject.receiver == receiver && receiverObject.classID == fiducialId){
//					receivers.splice(i,1);
//					break;
//				}
//				i = i+1;
//			}
			
		}
		
		private function subtractDicts(dict1:Dictionary, dict2:Dictionary):Array{
			var diffArray:Array = new Array();
			
			for (var key:Object in dict1){
				var isIn:Object = dict2[key];
				if(isIn == null){
					diffArray.push(key);
				}
			}
			
			return diffArray;
		}
		private function createDict(objectsUnderPoint:Array):Dictionary{
			var objectsDict:Dictionary = new Dictionary();
			for each(var displayObject:DisplayObject in objectsUnderPoint){
				objectsDict[displayObject] = "";
			}
			return objectsDict;
		}
		
		public function handleRemove(tuioContainer:TuioContainer):void {
			var stagePos:Point = new Point(stage.stageWidth * tuioContainer.x, stage.stageHeight * tuioContainer.y);
			var target:DisplayObject = getTopDisplayObjectUnderPoint(stagePos);
			var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
			
			target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_UP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			this.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_UP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			if (_dispatchMouseEvents) {
				target.dispatchEvent(new MouseEvent(MouseEvent.MOUSE_UP, true, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
			}
			if (_dispatchNativeTouchEvents) {
				target.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_END, true, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, target as InteractiveObject));
			}
			
			
			//handle receivers
			if(this.touchReceiversDict[tuioContainer.sessionID]){
				//call removeTouch from each receiver that listens on sessionID
				for each(var receiver:ITuioTouchReceiver in this.touchReceiversDict[tuioContainer.sessionID]){
					receiver.removeTouch(new TouchEvent(TouchEvent.TOUCH_UP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
				}
				
				//delete receivers from dictionary
				delete this.touchReceiversDict[tuioContainer.sessionID];
			}
			
			//tap
			if (target == firstTarget[tuioContainer.sessionID]) {
				var double:Boolean = false;
				var tmpArray:Array = new Array();
				var item:DoubleTapStore;
				while (tapped.length > 0) {
					item = tapped.pop() as DoubleTapStore;
					if (item.check(this.doubleTapTimeout)) {
						if (item.target == target && Math.abs(stagePos.x-item.x) < this.doubleTapDistance && Math.abs(stagePos.y - item.y) < this.doubleTapDistance ) double = true;
						else tmpArray.push(item);
					}
				}
				tapped = tmpArray.concat();
				
				if (double) {
					target.dispatchEvent(new TouchEvent(TouchEvent.DOUBLE_TAP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
					this.dispatchEvent(new TouchEvent(TouchEvent.DOUBLE_TAP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
					if (_dispatchMouseEvents) {
						target.dispatchEvent(new MouseEvent(MouseEvent.DOUBLE_CLICK, true, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
					}
				} else {
					tapped.push(new DoubleTapStore(target, getTimer(), stagePos.x, stagePos.y));
					target.dispatchEvent(new TouchEvent(TouchEvent.TAP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
					this.dispatchEvent(new TouchEvent(TouchEvent.TAP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
					if (_dispatchMouseEvents) {
						target.dispatchEvent(new MouseEvent(MouseEvent.CLICK, true, false, local.x, local.y, (target as InteractiveObject), false, false, false, false, 0));
					}
					if (_dispatchNativeTouchEvents) {
						target.dispatchEvent(new flash.events.TouchEvent(flash.events.TouchEvent.TOUCH_TAP, true, false, tuioContainer.sessionID, false, local.x, local.y, 0, 0, 0, target as InteractiveObject));
					}
				}
			}
			
			lastTarget[tuioContainer.sessionID] = null;
			firstTarget[tuioContainer.sessionID] = null;
			hold[tuioContainer.sessionID] = null;
		}
		
		private function getTopDisplayObjectUnderPoint(point:Point):DisplayObject {
			var targets:Array =  stage.getObjectsUnderPoint(point);
			var item:DisplayObject = (targets.length > 0) ? targets[targets.length - 1] : stage;
			
			if(this.touchTargetDiscoveryMode == TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED){
				while(targets.length > 0) {
					item = targets.pop() as DisplayObject;
					//ignore debug cursor/object/blob and send object under debug cursor/object/blob
					if((item is ITuioDebugCursor || item is ITuioDebugBlob || item is ITuioDebugObject || item is ITuioDebugTextSprite) && targets.length > 0){
						continue;
					}
					if (item.parent != null && !(item is InteractiveObject)) item = item.parent;
					if (item is InteractiveObject) {
						if ((item as InteractiveObject).mouseEnabled) return item;
					}
				}
				item = stage;
			} else if (this.touchTargetDiscoveryMode == TOUCH_TARGET_DISCOVERY_IGNORELIST) {
				while(targets.length > 0) {
					item = targets.pop();
					//ignore debug cursor/object/blob and send object under debug cursor/object/blob
					if((item is ITuioDebugCursor || item is ITuioDebugBlob || item is ITuioDebugObject || item is ITuioDebugTextSprite) && targets.length > 0){
						continue;
					}
					if (!bubbleListCheck(item)) return item;
				}
				item = stage;
			}
			
			return item;
		}
		
		/**
		 * Checks if a DisplayObject or its parents are in the ignoreList.
		 * 
		 * @param	obj The DisplayObject that has to be checked.
		 * @return Is true if the DisplayObject or one of its parents is in the ignoreList.
		 */
		private function bubbleListCheck(obj:DisplayObject):Boolean {
			if (ignoreList.indexOf(obj) < 0){
				if (obj.parent != null) {
					return bubbleListCheck(obj.parent);
				} else {
					return false;
				}
			} else {
				return true;
			}
		}
		
		/**
		 * Creates a list of all ancestors for the given <code>DisplayObject</code> from 
		 * the <code>DisplayObject</code>'s parent to the stage.
		 * 
		 * @param	item The <code>DisplayObject</code> of which the list will be created.
		 * @return The ancestor list of the given <code>DisplayObject</code>
		 */
		private function createAncestorList(item:DisplayObject):Array {
			var list:Array = new Array();
			var stage:Stage = item.stage;
			while (item != stage) {
				list.push(item.parent);
				item = item.parent;
			}
			return list;
		}
		
		/**
		 * Adds the given DisplayObject to an internal list of DisplayObjects that won't receive TouchEvents.
		 * If a DisplayobjectContainer is added to the list its children can still receive TouchEvents.
		 * The touchTargetDiscoveryMode is automatically set to TOUCH_TARGET_DISCOVERY_IGNORELIST.
		 * 
		 * @param	item The DisplayObject that should be ignored by TouchEvents.
		 */
		public function addToIgnoreList(item:DisplayObject):void {
			this.touchTargetDiscoveryMode = TOUCH_TARGET_DISCOVERY_IGNORELIST;
			if(ignoreList.indexOf(item) < 0) ignoreList.push(item);
		}
		
		/**
		 * Removes the given DisplayObject from the internal list of DisplayObjects that won't receive TouchEvents.
		 * 
		 * @param	item The DisplayObject that should be ignored by TouchEvents.
		 */
		public function removeFromIgnoreList(item:DisplayObject):void {
			var tmpList:Array = new Array();
			var listItem:Object;
			while (ignoreList.length > 0) {
				listItem = ignoreList.pop();
				if (listItem != item) tmpList.push(listItem);
			}
			ignoreList = tmpList.concat();
		}
		
		/**
		 * If set true MouseEvents are dispatched alongside the TouchEvents also the touchTargetDicoveryMode
		 * is automatically set to TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED.
		 * The default value is false.
		 */
		public function set dispatchMouseEvents(value:Boolean):void {
			if (value) this.touchTargetDiscoveryMode = TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED;
			this._dispatchMouseEvents = value;
		}
		
		public function get dispatchMouseEvents():Boolean {
			return this._dispatchMouseEvents;
		}
		
		/**
		 * If set true native TouchEvents (since Flash 10.1 and Air2.0) are dispatched alongside the org.tuio.TouchEvents also the touchTargetDicoveryMode
		 * is automatically set to TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED.
		 * The default value is false.
		 */
		public function set dispatchNativeTouchEvents(value:Boolean):void {
			if (value) this.touchTargetDiscoveryMode = TOUCH_TARGET_DISCOVERY_MOUSE_ENABLED;
			this._dispatchNativeTouchEvents = value;
		}
		
		public function get dispatchNativeTouchEvents():Boolean {
			return this._dispatchNativeTouchEvents;
		}
		
		public function addTuioObject(tuioObject:TuioObject):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.ADD_OBJECT, tuioObject));
			this.dispatchEvent(new TuioEvent(TuioEvent.ADD, tuioObject));
			if(triggerTouchOnObject) this.handleAdd(tuioObject);
		}
		
		public function updateTuioObject(tuioObject:TuioObject):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.UPDATE_OBJECT, tuioObject));
			this.dispatchEvent(new TuioEvent(TuioEvent.UPDATE, tuioObject));
			if(triggerTouchOnObject) this.handleUpdate(tuioObject);
		}
		
		public function removeTuioObject(tuioObject:TuioObject):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.REMOVE_OBJECT, tuioObject));
			this.dispatchEvent(new TuioEvent(TuioEvent.REMOVE, tuioObject));
			if(triggerTouchOnObject) this.handleRemove(tuioObject);
		}

		public function addTuioCursor(tuioCursor:TuioCursor):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.ADD_CURSOR, tuioCursor));
			this.dispatchEvent(new TuioEvent(TuioEvent.ADD, tuioCursor));
			this.handleAdd(tuioCursor);
		}
		
		public function updateTuioCursor(tuioCursor:TuioCursor):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.UPDATE_CURSOR, tuioCursor));
			this.dispatchEvent(new TuioEvent(TuioEvent.UPDATE, tuioCursor));
			this.handleUpdate(tuioCursor);
		}
		
		public function removeTuioCursor(tuioCursor:TuioCursor):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.REMOVE_CURSOR, tuioCursor));
			this.dispatchEvent(new TuioEvent(TuioEvent.REMOVE, tuioCursor));
			this.handleRemove(tuioCursor);
		}
		
		public function addTuioBlob(tuioBlob:TuioBlob):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.ADD_BLOB, tuioBlob));
			this.dispatchEvent(new TuioEvent(TuioEvent.ADD, tuioBlob));
			if(triggerTouchOnBlob) this.handleAdd(tuioBlob);
		}

		public function updateTuioBlob(tuioBlob:TuioBlob):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.UPDATE_BLOB, tuioBlob));
			this.dispatchEvent(new TuioEvent(TuioEvent.UPDATE, tuioBlob));
			if(triggerTouchOnBlob) this.handleUpdate(tuioBlob);
		}
		
		public function removeTuioBlob(tuioBlob:TuioBlob):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.REMOVE_BLOB, tuioBlob));
			this.dispatchEvent(new TuioEvent(TuioEvent.REMOVE, tuioBlob));
			if(triggerTouchOnBlob) this.handleRemove(tuioBlob);
		}
		
		public function newFrame(id:uint):void {
			this.dispatchEvent(new TuioEvent(TuioEvent.NEW_FRAME, null));
		}
		
		public function get tuioClient():TuioClient{
			return _tuioClient;
		}
	}
	
}

import flash.display.DisplayObject;
import flash.utils.getTimer;

internal class DoubleTapStore {
	
	internal var target:DisplayObject;
	internal var time:int;
	internal var x:Number;
	internal var y:Number;
	
	function DoubleTapStore(target:DisplayObject, time:int, x:Number, y:Number) {
		this.target = target;
		this.time = time;
		this.x = x;
		this.y = y;
	}
	
	internal function check(timeout:int):Boolean {
		if (time > getTimer() - timeout) return true;
		else return false;
	}
	
}