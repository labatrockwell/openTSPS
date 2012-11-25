package org.tuio.mouse
{
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.InteractiveObject;
	import flash.display.SpreadMethod;
	import flash.display.Sprite;
	import flash.display.Stage;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import org.tuio.*
	import org.tuio.debug.*
	import org.tuio.util.DisplayListHelper;

	/**
	 * Listens on MouseEvents, "translates" them to the analog TouchEvents and dispatches
	 * them on <code>DisplayObject</code>s under the mouse pointer.
	 * 
	 * Additionally, it provides means to simulate multi-touch input with a single mouse.
	 * By pressing the 'Shift' key a touch can be added permanently. By pressing the 'Ctrl'
	 * in Windows or the 'Command' key in Mac OS X a touch will be added to a group. 
	 * 
	 * A group of touches will be moved around together. To rotate a group of touches, press
	 * the 'r' key, while dragging. To make a group disappear after dragging hold the 'Space'
	 * key while dragging. This is very handy, if you want to test physical properties of an
	 * object like inertia around an angle. This is not possible e.g. with the Tuio Simulator
	 * or SimTouch. 
	 * 
	 * 
	 * @author Johannes Luderschmidt
	 * 
	 * @see org.tuio.TouchEvent
	 * 
	 */
	public class MouseToTouchDispatcher
	{
		private var stage:Stage;
		private var useTuioManager:Boolean;
		private var useTuioDebug:Boolean;
		private var touchId:uint;
		private var touchMoveId:Number;
		private var shiftKey:Boolean;
		private var groups:Dictionary;
		
		private var frameId:uint = 0;
		
		private var offsetX:Number;
		private var offsetY:Number;
		private var lastX:Number;
		private var lastY:Number;
		
		private var spaceKey:Boolean;
		private var rKey:Boolean;
		private var rotationMouseX:Number;
		private var rotationMouseY:Number;
		
		/**
		 * initializes MouseToTouchDispatcher by adding appropriate event listeners to it. Basically, MouseToTouchDispatcher
		 * listens on mouse events and translates them to touches. However, additionally keyboard listeners are being added
		 * that listen on keyboard events to control certain actions like rotation of a touches group by holding 'r'.
		 * 
		 * @param stage 
		 * @param useTuioManager call the add, move and remove functions of the TuioManager instead of simply dispatching TouchEvents. You have to initialize TuioManager before.
		 * @param useTuioDebug show the touches as debug cursors. You have to initialize TuioDebug before.
		 * 
		 */
		public function MouseToTouchDispatcher(stage:Stage, useTuioManager:Boolean = true, useTuioDebug:Boolean = true)
		{
			this.stage = stage; 
			stage.addEventListener(MouseEvent.CLICK, dispatchTap);
			stage.addEventListener(MouseEvent.DOUBLE_CLICK, dispatchDoubleTap);
			stage.addEventListener(MouseEvent.MOUSE_DOWN, dispatchTouchDown);
			stage.addEventListener(KeyboardEvent.KEY_DOWN, keyDown);
			stage.addEventListener(KeyboardEvent.KEY_UP, keyUp);
			
			this.useTuioManager = useTuioManager; 
			this.useTuioDebug = useTuioDebug;
			
			//descend through uint from the highest number
			touchId = 0-1;
			touchMoveId = touchId;
			
			offsetX = 0;
			offsetY = 0;
			lastX = 0;
			lastY = 0;
			
			groups = new Dictionary();
			
			spaceKey = false;
			rKey = false;
			rotationMouseX = 0;
			rotationMouseY = 0;
			
		}
		
		/**
		 * dispatches a tap event after the mouse has been clicked.
		 *  
		 * @param event
		 * 
		 */
		private function dispatchTap(event:MouseEvent):void{
			var stagePos:Point = new Point(event.stageX, event.stageY);
			var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
			var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
			
			target.dispatchEvent(new TouchEvent(TouchEvent.TAP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, createTuioContainer(event.stageX, event.stageY, this.touchId, this.frameId++)));
		}
		
		/**
		 * dispatches a double-tap event after the mouse has been double-clicked.
		 *  
		 * @param event
		 * 
		 */
		private function dispatchDoubleTap(event:MouseEvent):void{
			var stagePos:Point = new Point(event.stageX, event.stageY);
			var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
			var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
			
			target.dispatchEvent(new TouchEvent(TouchEvent.DOUBLE_TAP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, createTuioContainer(event.stageX, event.stageY, this.touchId, this.frameId++)));
		}
		
		
		/**
		 * dispatches a touch down event after the mouse key has been pushed down. If there is no existing touch 
		 * under the mouse pointer, a new touch will be added. However, if there already is one it will be marked
		 * for movement and no new touch is being added. If the 'Shift' key is pressed an existing and there is an 
		 * existing touch beneath the mouse cursor this touch will be removed. If the 'Ctrl/Command' key is pressed 
		 * the touch will be added to a group (marked by a dot in the center of a touch) if it does not belong to a 
		 * group already. If it does it will be removed from the group.
		 * 
		 * NOTE: Adding touches permanently does only work if TuioDebug is being used and useTuioDebug is switched on.
		 *  
		 * @param event
		 * 
		 */
		private function dispatchTouchDown(event:MouseEvent):void{
			var cursorUnderPoint:ITuioDebugCursor = getCursorUnderPointer(event.stageX, event.stageY); 
			//add new mouse pointer
			if (cursorUnderPoint == null) {
				var frameId:uint = this.frameId++;	
				var tuioContainer:TuioContainer = createTuioContainer(event.stageX,event.stageY, this.touchId, frameId); 
				//standard
				if(this.useTuioManager){
					TuioManager.getInstance().handleAdd(tuioContainer);
				}else{
					var stagePos:Point = new Point(event.stageX, event.stageY);
					var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
					var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
					
					target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_DOWN, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
				}
				if(this.useTuioDebug){
					var cursor:TuioCursor = createTuioCursor(event.stageX, event.stageY, this.touchId, frameId);
					TuioDebug.getInstance().addTuioCursor(cursor);
				}
				
				stage.addEventListener(MouseEvent.MOUSE_MOVE, dispatchTouchMove);
				stage.addEventListener(MouseEvent.MOUSE_UP, dispatchTouchUp);
				
				//takes care that the cursor will not be removed on mouse up
				this.shiftKey = event.shiftKey;
			}else{
				//update or remove cursor under mouse pointer
				if(event.shiftKey){
					//remove cursor
					removeCursor(event, cursorUnderPoint.sessionId);
					deleteFromGroup(cursorUnderPoint);
				}else if(event.ctrlKey){
					var cursorObject:Object = this.groups[cursorUnderPoint.sessionId];
					
					//add cursor to group
					if(cursorObject == null){
						//add to group
						addToGroup(cursorUnderPoint);
					}else{
						//remove from group
						(cursorObject.cursor as DisplayObjectContainer).removeChild(cursorObject.markerSprite);
//						delete this.groups[cursorUnderPoint.sessionId];
						deleteFromGroup(cursorUnderPoint);
					}
				}else{
					//move cursor
					this.touchMoveId = cursorUnderPoint.sessionId;
					
					//take care that cursor is moved around the middle
					offsetX = event.localX;
					offsetY = event.localY;
					lastX = event.stageX;
					lastY = event.stageY;
					
					stage.addEventListener(MouseEvent.MOUSE_MOVE, dispatchTouchMove);
					stage.addEventListener(MouseEvent.MOUSE_UP, dispatchTouchUp);
				}
			}
		}
		
		/**
		 *adds a cursor to a group.
		 *  
		 * @param cursorUnderPoint the cursor that should be added to the group.  
		 * 
		 */
		private function addToGroup(cursorUnderPoint:ITuioDebugCursor):void{
			var cursorObject:Object = this.groups[cursorUnderPoint.sessionId];
			
			cursorObject = new Object();
			cursorObject.cursor = cursorUnderPoint;
			
			var markerSprite:Sprite = new Sprite();
			markerSprite.graphics.beginFill(0xff0000);
			markerSprite.graphics.drawCircle(0,0,3);
			markerSprite.graphics.endFill();
			(cursorUnderPoint as DisplayObjectContainer).addChild(markerSprite);
			
			cursorObject.markerSprite = markerSprite;
			
			this.groups[cursorUnderPoint.sessionId] = cursorObject;
		}
		
		/**
		 * deletes a cursor from the group dictionary.
		 *  
		 * @param cursorUnderPoint the cursor that should be removed from the group.
		 * 
		 */
		private function deleteFromGroup(cursorUnderPoint:ITuioDebugCursor):void{
			delete this.groups[cursorUnderPoint.sessionId];
		}
		
		/**
		 * moves a touch. 
		 * 
		 * If the 'r' key is being pressed and a touch that is member of a group is being 
		 * moved around the group will be rotated around its berycentric point. To rotate the touches, 
		 * drag the mouse left and right.
		 *  
		 * @param event
		 * 
		 */
		private function dispatchTouchMove(event:MouseEvent):void{
			if(this.groups[this.touchMoveId] != null){
				var xDiff:Number =  lastX - event.stageX;
				var yDiff:Number = lastY - event.stageY;
				
				lastX = event.stageX;
				lastY = event.stageY;
				var cursorObject:Object;
				var cursor:DisplayObjectContainer
				
				var xPos:Number;
				var yPos:Number;
				
				//simply move grouped touches if 'r' key is not pressed
				if(!this.rKey){
					for each(cursorObject in this.groups){
						cursor = cursorObject.cursor as DisplayObjectContainer;
						xPos = cursor.x - xDiff;
						yPos = cursor.y - yDiff;
						moveCursor(xPos, yPos, cursorObject.cursor.sessionId);
					}
				}else{
					//rotate grouped touches if 'r' key is pressed
					for each(cursorObject in this.groups){
						cursor = cursorObject.cursor as DisplayObjectContainer;
						
						var cursorMatrix:Matrix = cursor.transform.matrix;
						cursorMatrix.translate(-this.rotationMouseX, -this.rotationMouseY);
						cursorMatrix.rotate(0.01 * -xDiff);
						cursorMatrix.translate(this.rotationMouseX, this.rotationMouseY);
						xPos = cursorMatrix.tx;
						yPos = cursorMatrix.ty;
						moveCursor(xPos, yPos, cursorObject.cursor.sessionId);
					}
				}
			}else{
				//if no touch from group has been select simply move single touch
				moveCursor(event.stageX-offsetX, event.stageY-offsetY, this.touchMoveId);				
			}
		}
		
		/**
		 * takes care of the touch movement by dispatching an appropriate TouchEvent or using the TuioManager and 
		 * adjusts the display of the touch in TuioDebug.
		 *  
		 * @param stageX the x coordinate of the touch 
		 * @param stageY the y coordinate of the touch 
		 * @param touchId the session id of the touch 
		 * 
		 */
		private function moveCursor(stageX:Number, stageY:Number, touchId:uint):void{
			var frameId:uint = this.frameId++;
			var tuioContainer:TuioContainer = createTuioContainer(stageX, stageY, touchId, frameId);
			
			if(this.useTuioManager){
				TuioManager.getInstance().handleUpdate(tuioContainer);
			}else{
				var stagePos:Point = new Point(stageX, stageY);
				var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
				var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
				
				target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_MOVE, true, false, local.x, local.y, stagePos.x, stagePos.y, target, tuioContainer));
			}
			
			if(this.useTuioDebug){
				var cursor:TuioCursor = createTuioCursor(stageX, stageY, touchId, frameId);
				TuioDebug.getInstance().updateTuioCursor(cursor);
			}
		}
		
		/**
		 * removes the touch that is being dragged around from stage if no key has been pressed.
		 * 
		 * If the 'Shift' key has been pressed the touch will remain on the stage. 
		 * 
		 * If the 'Ctrl/Command' key has been pressed the touch will remain on stage and will be 
		 * added to a group.
		 * 
		 * If the 'Space' key is being pressed and a group of touches is being moved around the 
		 * whole group of touches will be removed.
		 *   
		 * @param event
		 * 
		 */
		private function dispatchTouchUp(event:MouseEvent):void{
			if(this.groups[this.touchMoveId] == null){
				//keep touch if shift key has been pressed
				if(!this.shiftKey && !event.ctrlKey){
					removeCursor(event, touchId);
				}else if(event.ctrlKey){
					var cursorUnderPoint:ITuioDebugCursor = getCursorUnderPointer(event.stageX, event.stageY);
					addToGroup(cursorUnderPoint);
				}
			}else{
				if(this.spaceKey){
					//remove all touches from group if space key is pressed
					for each(var cursorObject:Object in this.groups){
						var cursor:DisplayObjectContainer = cursorObject.cursor as DisplayObjectContainer;
						removeCursor(event, cursorObject.cursor.sessionId);
						deleteFromGroup(cursorObject.cursor);
					}
				}
			}
			
			touchId = touchId-1;
			touchMoveId = touchId;
			
			offsetX = 0;
			offsetY = 0;
			lastX = 0;
			lastY = 0;
			
			stage.removeEventListener(MouseEvent.MOUSE_MOVE, dispatchTouchMove);
			stage.removeEventListener(MouseEvent.MOUSE_UP, dispatchTouchUp);
		}
		
		/**
		 * removes a touch from stage by dispatching an appropriate TouchEvent or using the TuioManager and 
		 * removes the display of the touch in TuioDebug.
		 *  
		 * @param event
		 * @param touchId
		 * 
		 */
		private function removeCursor(event:MouseEvent, touchId:uint):void{
			var frameId:uint = this.frameId++;
			if(this.useTuioManager){
				TuioManager.getInstance().handleRemove(createTuioContainer(event.stageX, event.stageY, touchId, frameId));
			}else{
				var stagePos:Point = new Point(event.stageX, event.stageY);
				var target:DisplayObject = DisplayListHelper.getTopDisplayObjectUnderPoint(stagePos, stage);
				var local:Point = target.globalToLocal(new Point(stagePos.x, stagePos.y));
				
				target.dispatchEvent(new TouchEvent(TouchEvent.TOUCH_UP, true, false, local.x, local.y, stagePos.x, stagePos.y, target, createTuioContainer(event.stageX, event.stageY, touchId, frameId)));
			}
			
			if(this.useTuioDebug){
				TuioDebug.getInstance().removeTuioCursor(createTuioCursor(event.stageX, event.stageY, touchId, frameId));
			}	
		}
		
		
		/**
		 * if the 'Space' key is being pressed spaceKey is set to true in this instance. 
		 * 
		 * If the 'r' key is being pressed rKey is set to true in this instance and the 
		 * barycentric coordinates of the touch group is being calculated. 
		 *  
		 * @param event
		 * 
		 */
		private function keyDown(event:KeyboardEvent):void{
			if(event.keyCode == 32){//space
				this.spaceKey = true;
			}
			if(event.keyCode == 82){//r
				var cursorUnderPoint:ITuioDebugCursor = getCursorUnderPointer(stage.mouseX, stage.mouseY);
				if(cursorUnderPoint != null && this.groups[cursorUnderPoint.sessionId] != null){
					if(!this.rKey){
						this.rKey = true;
						
						//rotate around touched touch
//						this.rotationMouseX = (cursorUnderPoint as DisplayObjectContainer).x;
//						this.rotationMouseY = (cursorUnderPoint as DisplayObjectContainer).y;
						
						//rotate around barycenter of touches
						var xPos:Number;
						var yPos:Number;
						var xPositions:Array = new Array();
						var yPositions:Array = new Array();
						var calcCenterPoint:Point = new Point();
						var touchAmount:Number = 0;
							
						calcCenterPoint.x = 0;
						calcCenterPoint.y = 0;
						
						
						for each(var cursorObject:Object in this.groups){
							var cursor:DisplayObjectContainer = cursorObject.cursor as DisplayObjectContainer;
							xPos = cursor.x;
							yPos = cursor.y;
							xPositions.push(xPos);
							yPositions.push(yPos);
							
							calcCenterPoint.x = calcCenterPoint.x + xPos;
							calcCenterPoint.y = calcCenterPoint.y + yPos;
							
							touchAmount = touchAmount+1;
						}
						this.rotationMouseX = calcCenterPoint.x/touchAmount;
						this.rotationMouseY = calcCenterPoint.y/touchAmount;
					}
				}
			}
		}
		
		/**
		 * sets keyboard variables to false.
		 *  
		 * @param event
		 * 
		 */
		private function keyUp(event:KeyboardEvent):void{
			if(event.keyCode == 32){//space
				this.spaceKey = false;
			}
			if(event.keyCode == 82){//r
				this.rKey = false;
				this.rotationMouseX = 0;
				this.rotationMouseY = 0;
			}
		}
		
		/**
		 * returns the touch under the mouse pointer if there is one. Otherwise null will be returned.
		 * If the mouse pointer is above the red dot of a touch that beloings to a group still the
		 * touch will be returned.
		 *   
		 * @param stageX
		 * @param stageY
		 * @return the touch under the mouse pointer if there is one. Otherwise null will be returned.
		 * 
		 */
		private function getCursorUnderPointer(stageX:Number, stageY:Number):ITuioDebugCursor{
			var cursorUnderPointer:ITuioDebugCursor = null;
			
			var objectsUnderPoint:Array = stage.getObjectsUnderPoint(new Point(stageX, stageY));
			
			if(objectsUnderPoint[objectsUnderPoint.length-1] is ITuioDebugCursor){
				cursorUnderPointer = objectsUnderPoint[objectsUnderPoint.length-1];
			}else if(objectsUnderPoint.length > 1 && objectsUnderPoint[objectsUnderPoint.length-2] is ITuioDebugCursor){
				//if mouse pointer is above marker sprite, return ITuioDebugCursor beneath marker sprite
				cursorUnderPointer = objectsUnderPoint[objectsUnderPoint.length-2];
			}
			
			return cursorUnderPointer; 
		}

		/**
		 * created a TuioContainer instance from the submitted parameters.
		 *  
		 * @param stageX an x coordinate in global coordinates.
		 * @param stageY a y coordinate in global coordinates.
		 * @param touchId the session id of a touch.
		 * 
		 * @return the TuioContainer.
		 * 
		 */
		private function createTuioContainer(stageX:Number, stageY:Number, touchId:uint, frameId:uint):TuioContainer{
			return new TuioContainer("2Dcur",touchId,stageX/stage.stageWidth, stageY/stage.stageHeight,0,0,0,0,0,frameId);
		}
		
		/**
		 * created a TuioCursor instance from the submitted parameters.
		 *  
		 * @param stageX an x coordinate in global coordinates.
		 * @param stageY a y coordinate in global coordinates.
		 * @param touchId the session id of a touch.
		 * 
		 * @return the TuioCursor.
		 * 
		 */
		private function createTuioCursor(stageX:Number, stageY:Number, touchId:uint, frameId:uint):TuioCursor{
			return new TuioCursor("2Dcur",touchId,stageX/stage.stageWidth, stageY/stage.stageHeight,0,0,0,0,0,frameId);
		}
		
	}
}