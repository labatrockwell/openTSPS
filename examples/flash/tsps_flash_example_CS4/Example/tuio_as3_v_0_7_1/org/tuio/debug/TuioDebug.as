package org.tuio.debug
{
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.display.Stage;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	import org.tuio.ITuioListener;
	import org.tuio.TuioBlob;
	import org.tuio.TuioClient;
	import org.tuio.TuioCursor;
	import org.tuio.TuioObject;

	/**
	 * 
	 * <p>implements the interface <code>ITuioListener</code> to show debug information about all tuio cursors and objects 
	 * that are prevailing in the application.</p>
	 * 
	 * <p>The appearance of the cursors and objects is controlled by the classes <code>TuioDebugCursor</code> and 
	 * <code>TuioDebugObject</code>. Their appearance can be tweaked with multiple settings. Additionally, custom debug cursor 
	 * and object implementations can be set via the functions <code>customCursorSprite</code> and <code>customObjectClass</code>.</p> 
	 * 
	 * @see org.tuio.ITuioListener
	 * @see TuioDebugCursor
	 * @see ITuioDebugCursor
	 * @see TuioDebugObject
	 * @see ITuioDebugObject
	 * 
	 * @author Johannes Luderschmidt
	 * 
	 */
	public class TuioDebug implements ITuioListener{
		
		private var stage:Stage;
		private var tuioClient:TuioClient;
		
		private var cursors:Array;
		private var objects:Array;
		private var blobs:Array;
		
		private var _showCursors:Boolean = true;
		private var _showObjects:Boolean = true;
		private var _showBlobs:Boolean = true;
		
		private var _showDebugText:Boolean = true;
		
		private var _cursorRadius:Number = 13;
		private var _cursorColor:Number = 0x0;
		private var _cursorAlpha:Number = 0;
		private var _cursorLineThickness:Number = 3;
		private var _cursorLineColor:Number = 0x0;
		private var _cursorLineAlpha:Number = 0.5;
		private var _customCursorSprite:Class;
		
		private var _objectWidth:Number = 80;
		private var _objectHeight:Number = 80;
		private var _objectColor:Number = 0x0;
		private var _objectAlpha:Number = 0.5;
		private var _objectLineThickness:Number = 0;
		private var _objectLineColor:Number = 0x0;
		private var _objectLineAlpha:Number = 0;
		private var _customObjectClass:Class;
		
		private static var allowInst:Boolean;
		private static var inst:TuioDebug;
		
		public function TuioDebug(stage:Stage){
			if (!allowInst) {
	            throw new Error("Error: Instantiation failed: Use TuioLegacyDebug.getInstance() instead of new.");
			}else{
				this.stage = stage;
				cursors = new Array();
				objects = new Array();
				blobs = new Array();
				
				_customObjectClass = TuioDebugObject;
				_customCursorSprite = TuioDebugCursor;
			}
		}
		
		/**
		 * initializes Singleton instance of TuioDebug. Must be called before <code>getInstance()</code> 
		 * can be called.
		 *  
		 * @param stage
		 * @return Singleton instance of TuioDebug.
		 * 
		 */
		public static function init(stage:Stage):TuioDebug{
			if(inst == null){
				allowInst = true;
				inst = new TuioDebug(stage);
				allowInst = false;
			}
			
			return inst;
		}
		
		/**
		 * Singleton instance of TuioDebug.
		 * 
		 * @return Singleton instance of TuioDebug.
		 * 
		 */
		public static function getInstance():TuioDebug{
			if(inst == null){
				throw new Error("Please initialize with method init(...) first!");
			}
			return inst;
		}
		
		private function addCursorEventListeners(cursor:TuioDebugCursor):void{
			cursor.addEventListener(org.tuio.TouchEvent.TAP, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.DOUBLE_TAP, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.TOUCH_DOWN, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.TOUCH_MOVE, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.TOUCH_OUT, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.TOUCH_OVER, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.TOUCH_UP, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.ROLL_OUT, propagateEvent);
			cursor.addEventListener(org.tuio.TouchEvent.ROLL_OVER, propagateEvent);
		}
		
		private function propagateEvent(event:org.tuio.TouchEvent):void{
			var targets:Array =  stage.getObjectsUnderPoint(new Point(event.stageX, event.stageY));
			var target:DisplayObject = (targets.length > 1) ? targets[targets.length-2] : stage;
		}

		/**
		 * Called if a new object was tracked.
		 * @param	tuioObject the received /tuio/2Dobj.
		 */
		public function addTuioObject(tuioObject:TuioObject):void{
			var objectSprite:Sprite;
			
			if(_customObjectClass == TuioDebugObject){
				objectSprite = new TuioDebugObject(tuioObject.classID, _objectWidth, _objectHeight, _objectColor, _objectAlpha,_objectLineThickness, _objectLineColor, _objectLineAlpha);
			}else{
				objectSprite = new _customObjectClass();
				if(!(objectSprite is ITuioDebugObject)){
					throw new Error("Custom Tuio Object class must implement ITuioDebugObject.");
				}
			}
			var objectObject:Object = new Object();
			if(_showObjects){
				objectSprite.x = tuioObject.x*stage.stageWidth;
				objectSprite.y = tuioObject.y*stage.stageHeight;
		        
				objectSprite.rotation = tuioObject.a/Math.PI*180;
				objectObject.object = objectSprite;
				objectObject.sessionID = tuioObject.sessionID;
				objects.push(objectObject);
				stage.addChild(objectSprite);
				
				if(_showDebugText){
					var label:TextField = new TextField();
		            label.autoSize = TextFieldAutoSize.LEFT;
		            label.background = false;
		            label.border = false;
					label.text = generateObjectLabelText(objectSprite.x, objectSprite.y, tuioObject.classID, tuioObject.sessionID);
		
		            label.defaultTextFormat = debugTextFormat();
		            label.setTextFormat(debugTextFormat());
					
					var textSprite:TuioDebugTextSprite = new TuioDebugTextSprite();
					objectSprite.addChild(textSprite);
					drawObjectString(label, textSprite);
					textSprite.x  = _objectWidth;
										
		            objectObject.label = label;
		            objectObject.textSprite = textSprite;
				}
			}
		}
		
		/**
		 * Called if a tracked object was updated.
		 * @param	tuioObject The values of the received /tuio/2Dobj.
		 */
		public function updateTuioObject(tuioObject:TuioObject):void{
			for each(var object:Object in objects){
				if(object.sessionID == tuioObject.sessionID){
					object.object.x = tuioObject.x*stage.stageWidth;
					object.object.y = tuioObject.y*stage.stageHeight;
					object.object.rotation = tuioObject.a/Math.PI*180;
					if(_showDebugText){
						object.label.text = generateObjectLabelText(object.object.x, object.object.y, tuioObject.classID, tuioObject.sessionID);
						drawObjectString(object.label, object.textSprite);
					}
					break;
				}
			}
		}
		
		/**
		 * Called if a tracked object was removed.
		 * 
		 * @param	tuioObject The values of the received /tuio/2Dobj.
		 */
		public function removeTuioObject(tuioObject:TuioObject):void{
			var i:Number = 0;
			for each(var object:Object in objects){
				if(object.sessionID == tuioObject.sessionID){
					stage.removeChild(object.object);
					objects.splice(i,1);
					break;
				}
				i=i+1;
			}
		}

		/**
		 * width of debug object rectangle.
		 * 
		 */
		public function get objectWidth():Number{
			return _objectWidth;
		}
		public function set objectWidth(objectWidth:Number):void{
			_objectWidth = objectWidth;
		}
		
		
		/**
		 * height of debug object rectangle. 
		 * 
		 */
		public function get objectHeight():Number{
			return _objectHeight;
		}
		public function set objectHeight(objectHeight:Number):void{
			_objectHeight = objectHeight;
		}
		
		
		/**
		 * color of the filling of debug object rectangle.
		 *  
		 */
		public function get objectColor():Number{
			return _objectColor;
		}
		public function set objectColor(objectColor:Number):void{
			_objectColor = objectColor;
		}
		
		/**
		 * alpha of the filling of debug object rectangle. 
		 * 
		 */
		public function get objectAlpha():Number{
			return _objectAlpha;
		}
		public function set objectAlpha(objectAlpha:Number):void{
			_objectAlpha = objectAlpha;
		}
		
		/**
		 * thickness of the line around a debug object rectangle.
		 *  
		 */	
		public function get objectLineThickness():Number{
			return _objectLineThickness;
		}
		public function set objectLineThickness(objectLineThickness:Number):void{
			_objectLineThickness = objectLineThickness;
		}
		
		/**
		 * color of the line around a debug object rectangle.
		 *  
		 */
		public function get objectLineColor():Number{
			return _objectLineColor;
		}
		public function set objectLineColor(objectLineColor:Number):void{
			_objectLineColor = objectLineColor;
		}
		
		/**
		 * alpha of the line around a debug object rectangle.
		 *  
		 */
		public function get objectLineAlpha():Number{
			return _objectLineAlpha;
		}
		public function set objectLineAlpha(objectLineAlpha:Number):void{
			_objectLineAlpha = objectLineAlpha;
		}
		
		/**
		 * sets base class for the Sprite that should be drawn on screen when a new
		 * object is added via a Tuio message.
		 *  
		 */
		public function get customObjectClass():Class{
			return _customObjectClass;
		}
		public function set customObjectClass (customObjectClass:Class):void{
			_customObjectClass = customObjectClass;
		}
		
		/**
		 * Called if a new cursor was tracked.
		 * @param	tuioObject The values of the received /tuio/**Dcur.
		 */
		public function addTuioCursor(tuioCursor:TuioCursor):void{
			var cursorSprite:Sprite;
			
			if(_customCursorSprite == TuioDebugCursor){
				cursorSprite = new TuioDebugCursor(_cursorRadius,_cursorColor, _cursorAlpha, _cursorLineThickness, _cursorLineColor, _cursorLineAlpha);
			}else{
				cursorSprite = new _customCursorSprite();
				if(!(cursorSprite is ITuioDebugCursor)){
					throw new Error("Custom Tuio Debug Cursor class must implement ITuioDebugCursor.");
				}
			}
			(cursorSprite as ITuioDebugCursor).sessionId = tuioCursor.sessionID;
			
			var cursorObject:Object = new Object();
			
			if(_showCursors){
				cursorSprite.x = tuioCursor.x*stage.stageWidth;
				cursorSprite.y = tuioCursor.y*stage.stageHeight;
				if(_customCursorSprite == TuioDebugCursor){
					addCursorEventListeners(cursorSprite as TuioDebugCursor);
				}
				cursorObject.cursor = cursorSprite;
				cursorObject.sessionID = tuioCursor.sessionID;
				cursors.push(cursorObject);
				stage.addChild(cursorSprite);
				
				if(_showDebugText){
					var label:TextField = new TextField();
		            label.autoSize = TextFieldAutoSize.LEFT;
		            label.background = false;
		            label.border = false;
					label.text = generateCursorLabelText(cursorSprite.x, cursorSprite.y, tuioCursor.sessionID);
		
		            label.defaultTextFormat = debugTextFormat();
		            label.setTextFormat(debugTextFormat());
					
					var textSprite:TuioDebugTextSprite = new TuioDebugTextSprite();
					drawCursorString(label, textSprite);
					cursorSprite.addChild(textSprite);
					textSprite.x  = _cursorRadius;
					textSprite.y  = 2*_cursorRadius;
					
		            cursorObject.label = label;
					cursorObject.textSprite = textSprite;
				}
			}
		}
		
		/**
		 * Called if a tracked cursor was updated.
		 * @param	tuioCursor The values of the received /tuio/2Dcur.
		 */
		public function updateTuioCursor(tuioCursor:TuioCursor):void{
			for each(var cursor:Object in cursors){
				if(cursor.sessionID == tuioCursor.sessionID){
					cursor.cursor.x = tuioCursor.x*stage.stageWidth;
					cursor.cursor.y = tuioCursor.y*stage.stageHeight;
					
					if(_showDebugText){
						cursor.label.text = generateCursorLabelText(cursor.cursor.x, cursor.cursor.y, tuioCursor.sessionID);
						drawCursorString(cursor.label, cursor.textSprite);
					}
					break;
				}
			}
		}
		
		/**
		 * Called if a tracked cursor was removed.
		 * @param	tuioCursor The values of the received /tuio/2Dcur.
		 */
		public function removeTuioCursor(tuioCursor:TuioCursor):void{
			var i:Number = 0;
			for each(var cursor:Object in cursors){
				if(cursor.sessionID == tuioCursor.sessionID){
					stage.removeChild(cursor.cursor);
					cursors.splice(i,1);
					break;
				}
				i=i+1;
			}
		}
		
		private function generateCursorLabelText(xVal:Number, yVal:Number, id:Number):String{
			return "x: " + xVal + "\ny: " + yVal + "\nid: " + id;
		}
		
		private function debugTextFormat():TextFormat{
			var format:TextFormat = new TextFormat();
	            format.font = "Arial";
	            format.color = 0x0;
	            format.size = 11;
	            format.underline = false;
	            
        	return format;
		}
		
		/**
		 * radius of the debug cursor circle.
		 *  
		 */
		public function get cursorRadius():Number{
			return _cursorRadius;
		}
		public function set cursorRadius(cursorRadius:Number):void{
			_cursorRadius = cursorRadius;
		}
		
		/**
		 * color of the filling of the debug cursor circle.
		 *  
		 */
		public function get cursorColor():Number{
			return _cursorColor;
		}
		public function set cursorColor(cursorColor:Number):void{
			_cursorColor = cursorColor;
		}
		
		/**
		 * alpha of the filling of the debug cursor circle.
		 *  
		 */
		public function get cursorAlpha():Number{
			return _cursorAlpha;
		}
		public function set cursorAlpha(cursorAlpha:Number):void{
			_cursorAlpha = cursorAlpha;
		}
		
		/**
		 * thickness of the line around a debug cursor circle.
		 * 
		 */
		public function get cursorLineThickness():Number{
			return _cursorLineThickness;
		}
		public function set cursorLineThickness(cursorLineThickness:Number):void{
			_cursorLineThickness = cursorLineThickness;
		}
		
		/**
		 * color of the line around a debug cursor circle.
		 *  
		 */
		public function get cursorLineColor():Number{
			return _cursorLineColor;
		}
		public function set cursorLineColor(cursorLineColor:Number):void{
			_cursorLineColor = cursorLineColor;
		}
		
		/**
		 * alpha of the line around a debug cursor circle.
		 *  
		 */
		public function get cursorLineAlpha():Number{
			return _cursorLineAlpha;
		}
		public function set cursorLineAlpha(cursorLineAlpha:Number):void{
			_cursorLineAlpha = cursorLineAlpha;
		}
		
		/**
		 * controls whether debug text (session id, x position, y position and fiducial id) should be shown next to
		 * a debug cursor or debug object.
		 *   
		 * @param showDebugText 
		 * 
		 */
		public function set showDebugText(showDebugText:Boolean):void{
			_showDebugText = showDebugText;
		}
		
		/**
		 * sets base class for the Sprite that should be drawn on screen when a new
		 * cursor is added via a Tuio message.
		 *  
		 * @param customCursorSprite class name of class that should be used as debug cursor information.
		 * 
		 */
		public function set customCursorSprite (customCursorSprite:Class):void{
			_customCursorSprite = customCursorSprite;
		}
		
		/**
		 * returns base class of the Sprite that is being drawn on screen when a new
		 * cursor is added via a Tuio message.
		 *  
		 * @return class of debug cursor sprite. 
		 * 
		 */
		public function get customCursorSprite():Class{
			return _customCursorSprite;
		}
		
		/**
		 * Called if a new blob was tracked.
		 * @param	tuioBlob The values of the received /tuio/**Dblb.
		 */
		public function addTuioBlob(tuioBlob:TuioBlob):void{
			trace(this, "addTuioBlob");
			if(_showBlobs){
				_showCursors = true;
				addTuioCursor(new TuioCursor("2dcur", tuioBlob.sessionID, tuioBlob.x, tuioBlob.y, tuioBlob.z,tuioBlob.X, tuioBlob.Y, tuioBlob.Z, tuioBlob.m, tuioBlob.frameID));
			}
		}

		/**
		 * Called if a tracked blob was updated.
		 * @param	tuioBlob The values of the received /tuio/**Dblb.
		 */
		public function updateTuioBlob(tuioBlob:TuioBlob):void{
			if(_showBlobs){
				_showCursors = true;
				updateTuioCursor(new TuioCursor("2dcur", tuioBlob.sessionID, tuioBlob.x, tuioBlob.y, tuioBlob.z,tuioBlob.X, tuioBlob.Y, tuioBlob.Z, tuioBlob.m, tuioBlob.frameID));
			}
				
		}
		
		/**
		 * Called if a tracked blob was removed.
		 * @param	tuioBlob The values of the received /tuio/**Dblb.
		 */
		public function removeTuioBlob(tuioBlob:TuioBlob):void{
			if(_showBlobs){
				_showCursors = true;
				removeTuioCursor(new TuioCursor("2dcur", tuioBlob.sessionID, tuioBlob.x, tuioBlob.y, tuioBlob.z,tuioBlob.X, tuioBlob.Y, tuioBlob.Z, tuioBlob.m, tuioBlob.frameID));
			}
		}
		
		public function newFrame(id:uint):void {
            // TODO
        }
		
		private function drawObjectString(textField:TextField, dObj:Sprite):void{
			var translationX:Number = -0.5*dObj.width+0.5*textField.width;
			var translationY:Number = 0.5*dObj.height-0.5*textField.height;
			drawString(textField, dObj, translationX, translationY);
		}
		
		private function drawCursorString(textField:TextField, dObj:Sprite):void{
			var translationX:Number = -0.5*_cursorRadius+0.5*textField.width+8;
			var translationY:Number = -0.5*_cursorRadius-0.5*textField.height+8;
			drawString(textField, dObj, translationX, translationY);
		}
		
		private function drawString(textField:TextField, dObj:Sprite, translationX:Number, translationY:Number):void{
			//copy TextField into a bitmap
			var typeTextBitmap : BitmapData = new BitmapData(textField.width, 
				textField.height,true,0x00000000);
			
			typeTextBitmap.draw(textField);
			
			//calculate center of TextField
			var typeTextTranslationX:Number = -0.5*textField.width+translationX+5;
			var typeTextTranslationY:Number = -0.5*textField.height+translationY-5;
			
			//create Matrix which moves the TextField to the center
			var matrix:Matrix = new Matrix();
			matrix.translate(typeTextTranslationX, typeTextTranslationY);
			
			//actually draw the text on the stage (with no-repeat and anti-aliasing)
			dObj.graphics.clear();
			dObj.graphics.beginBitmapFill(typeTextBitmap,matrix,false,true);
			dObj.graphics.lineStyle(0,0,0);
			dObj.graphics.drawRect(typeTextTranslationX, typeTextTranslationY, 
				textField.width, textField.height);
			dObj.graphics.endFill();
		}
		
		private function generateObjectLabelText(xVal:Number, yVal:Number, objectId:Number, sessionId:Number):String{
			return "x: " + xVal + "\ny: " + yVal + "\nobjectId: " + objectId+ "\nsessionId: " + sessionId;
		}
		
	}
}