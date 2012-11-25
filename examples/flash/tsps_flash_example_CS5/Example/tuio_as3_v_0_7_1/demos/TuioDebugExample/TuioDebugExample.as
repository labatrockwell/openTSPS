package {
	import flash.display.*;
	import flash.events.Event;
	import flash.text.*;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	
	import org.tuio.connectors.*;
	import org.tuio.*;
	import org.tuio.debug.*;
	
	/**
	 * This is a demo class for using the TuioDebug class.
	 * You can use this class either as main class or it to the stage.
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioDebugExample extends MovieClip {
		
		private var tf:TextField;
		private var tDbg:TuioDebug;
		
		public function TuioDebugExample() {
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void {	
			
			/**
			 * You can replace the connector used for creating the TuioClient with any connector you'd like to use.
			 */
			var tc:TuioClient = new TuioClient(new TCPConnector());
			tDbg = TuioDebug.init(stage);
			tc.addListener(tDbg);
			
			/**
			 * And that's it for the tuio part.
			 * You could also add the event listeners directly to the TuioManager if you don't happen to have a stage reference.
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