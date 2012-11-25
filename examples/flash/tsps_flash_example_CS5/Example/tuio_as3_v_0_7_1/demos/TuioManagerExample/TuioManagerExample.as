package {
	import flash.display.*;
	import flash.events.Event;
	import flash.text.*;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	
	import org.tuio.connectors.*;
	import org.tuio.*;
	
	/**
	 * This is a demo class for using the TuioManager.
	 * It will draw red circles for every touch point.
	 * If you tap the big circle it will change its color.
	 * On the top left there is an indicator that turny fully red if events are triggered by the TuioManager.
	 * You can use this class either as main class or it to the stage.
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioManagerExample extends MovieClip {
		
		public var circleSize:Number;
		public var tf:TextField;
		public var touchMe:Sprite;
		
		private var curIndicator:Indicator;
		
		public function TuioManagerExample() {
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void {	
			
			/**
			 * You can replace the connector used for creating the TuioClient with any connector you'd like to use.
			 */
			var tc:TuioClient = new TuioClient(new TCPConnector());
			var tm:TuioManager = TuioManager.init(stage, tc);
			
			stage.addEventListener(TouchEvent.TOUCH_DOWN, handleDown);
			stage.addEventListener(TouchEvent.TOUCH_MOVE, handleMove);
			stage.addEventListener(TouchEvent.TOUCH_UP, handleUp);
			
			touchMe = new Sprite();
			touchMe.addEventListener(TouchEvent.TAP, handleTap);
			
			/**
			 * And that's it for the tuio part.
			 * You could also add the event listeners directly to the TuioManager if you don't happen to have a stage reference.
			 */
			
			touchMe.graphics.beginFill(0x6666cc);
			touchMe.graphics.drawCircle(200, 200, 100);
			touchMe.graphics.endFill();
			
			stage.addChild(touchMe);
			 
			this.circleSize = 10;
			
			tf = new TextField();
			tf.autoSize = TextFieldAutoSize.LEFT;
			tf.selectable = false;
			tf.defaultTextFormat = new TextFormat("Arial", null, 0x999999);
			tf.appendText("Press the UP arrow key to increase the circle size.");
			tf.appendText("\nPress the DOWN arrow key to decrease the circle size.");
			tf.appendText("\nPress SPACE for fullscreen.");
			tf.x = 70;
			tf.y = 11;
			
			this.curIndicator = new Indicator(10, 0xcc0000);
			this.curIndicator.y = 35;
			this.curIndicator.x = 35;
			
			this.addChild(this.curIndicator);
			this.addChild(tf);
			
			stage.addEventListener(KeyboardEvent.KEY_DOWN, handleKeyDown);
			
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align = StageAlign.TOP_LEFT;
			
		}
		
		public function handleDown(e:TouchEvent):void {
			var circle:Circle = new Circle(e.tuioContainer.sessionID.toString(), stage, e.stageX, e.stageY, circleSize, 0xcc0000);
			this.curIndicator.blink();
		}
		
		public function handleUp(e:TouchEvent):void {
			this.curIndicator.blink();
			var circle:DisplayObject = stage.getChildByName(e.tuioContainer.sessionID.toString());
			stage.removeChild(circle);
		}
		
		public function handleMove(e:TouchEvent):void {
			this.curIndicator.blink();
			var circle:DisplayObject = stage.getChildByName(e.tuioContainer.sessionID.toString());
			circle.x = e.stageX;
			circle.y = e.stageY;
		}
		
		public function handleTap(e:TouchEvent):void {
			this.curIndicator.blink();
			touchMe.graphics.clear();
			touchMe.graphics.beginFill(Math.random()*0xcccccc);
			touchMe.graphics.drawCircle(200, 200, 100);
			touchMe.graphics.endFill();
		}
		
		public function handleKeyDown(event:KeyboardEvent):void { 	
			if (event.keyCode == Keyboard.DOWN){
				this.circleSize -= 2;
			} else if (event.keyCode == Keyboard.UP){
				this.circleSize += 2;
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