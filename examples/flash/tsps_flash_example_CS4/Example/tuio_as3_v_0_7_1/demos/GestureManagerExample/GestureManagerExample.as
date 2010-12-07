package {
	import flash.display.*;
	import flash.events.Event;
	import flash.text.*;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	import org.tuio.gestures.DragGesture;
	import org.tuio.gestures.GestureManager;
	
	import org.tuio.connectors.*;
	import org.tuio.*;
	import org.tuio.debug.*;
	
	/**
	 * This is a demo class for using the GestureManager.
	 * It uses TuioDebug to visualize the touch points.
	 * A number of randomly colored rectangles will be drawn on the stage. 
	 * You can drag, scale, rotate them with the help of the corresponding gestures.
	 * You can use this class either as main class or it to the stage.
	 * 
	 * @author Immanuel Bauer
	 */
	public class GestureManagerExample extends MovieClip {
		
		public var circleSize:Number;
		public var tf:TextField;
		
		private var tDbg:TuioDebug;
		
		public function GestureManagerExample() {
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void {	
			
			/**
			 * You can replace the connector used for creating the TuioClient with any connector you'd like to use.
			 */
			var tc:TuioClient = new TuioClient(new TCPConnector());
			var tm:GestureManager = GestureManager.init(stage, tc);
			GestureManager.addGesture(new DragGesture());
			tDbg = TuioDebug.init(stage);
			tc.addListener(tDbg);
			
			/**
			 * And that's it for the tuio part.
			 */
			tf = new TextField();
			tf.autoSize = TextFieldAutoSize.LEFT;
			tf.selectable = false;
			tf.defaultTextFormat = new TextFormat("Arial", null, 0x999999);
			tf.appendText("Press the UP arrow key to increase the circle size.");
			tf.appendText("\nPress the DOWN arrow key to decrease the circle size.");
			tf.appendText("\nPress SPACE for fullscreen.");
			tf.x = 11;
			tf.y = 11;

			this.addChild(tf);
			
			stage.addEventListener(KeyboardEvent.KEY_DOWN, handleKeyDown);
			
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align = StageAlign.TOP_LEFT;
			
			var item:DragRotateScaleMe;
			for (var c:int = 0; c < 10; c++ ) {
				item = new DragRotateScaleMe(100 + Math.random() * (stage.stageWidth - 200), 100 + Math.random() * (stage.stageHeight - 200), stage.stageWidth / 2, stage.stageHeight / 2);
				stage.addChild(item);
			}
			
		}
		
		public function handleKeyDown(event:KeyboardEvent):void { 	
			if (event.keyCode == Keyboard.DOWN){
				tDbg.cursorRadius -= 2;
			} else if (event.keyCode == Keyboard.UP){
				tDbg.cursorRadius += 2;
			} else if (event.keyCode == Keyboard.SPACE) {
				if(stage.displayState == StageDisplayState.NORMAL){
					stage.displayState = StageDisplayState.FULL_SCREEN;
					tf.text = "";
					tf.appendText("Press the UP arrow key to increase the circle size.");
					tf.appendText("\nPress the DOWN arrow key to decrease the circle size.");
					tf.appendText("\nPress SPACE for windowmode.");
				} else {
					stage.displayState = StageDisplayState.NORMAL;
					tf.text = "";
					tf.appendText("Press the UP arrow key to increase the circle size.");
					tf.appendText("\nPress the DOWN arrow key to decrease the circle size.");
					tf.appendText("\nPress SPACE for fullscreen.");
				}
			}
		} 
	}

}