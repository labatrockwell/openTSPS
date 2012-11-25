package {
		
	import org.tuio.*;
	import org.tuio.osc.*;
	import org.tuio.connectors.*;
	import flash.display.*;
	import flash.text.*;
	import flash.ui.*;
	import flash.events.*;
	
	/**
	 * This is a demo class for a simple ITuioListener implementation and using the TuioClient.
	 * You can use this class either as main class or it to the stage.
	 * It will draw red circles for every touch point, green circles for every object and blue circles for every blob.
	 * On the top left there are three indicators that turn fully opaque if corresponding messages are received by the TuioClient.
	 * This example should be very handy to test out if your setup is working correctly.
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioClientExample extends MovieClip implements ITuioListener {
		
		public var currentCircle:Circle;
		public var tf:TextField;
		public var circleSize:Number;
		
		private var lc:Boolean = true;
		private var tuio:TuioClient;
		
		private var curIndicator:Indicator;
		private var objIndicator:Indicator;
		private var blbIndicator:Indicator;
		
		public function TuioClientExample(){
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void {
			this.circleSize = 10;
			
			stage.addEventListener(KeyboardEvent.KEY_DOWN, handleKeyDown);
			
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align = StageAlign.TOP_LEFT;
			
			this.curIndicator = new Indicator(10, 0xcc0000);
			this.objIndicator = new Indicator(10, 0x00cc00);
			this.blbIndicator = new Indicator(10, 0x0000cc);
			
			this.curIndicator.y = this.objIndicator.y = this.blbIndicator.y = 35;
			
			this.curIndicator.x = 35;
			this.objIndicator.x = 65;
			this.blbIndicator.x = 95;
			
			tf = new TextField();
			tf.autoSize = TextFieldAutoSize.LEFT;
			tf.selectable = false;
			tf.defaultTextFormat = new TextFormat("Arial", null, 0x999999);
			tf.appendText("Press the UP arrow key to increase the circle size.");
			tf.appendText("\nPress the DOWN arrow key to decrease the circle size.");
			tf.appendText("\nPress SPACE for fullscreen.");
			tf.x = 130;
			tf.y = 11;
			
			this.addChild(this.curIndicator);
			this.addChild(this.objIndicator);
			this.addChild(this.blbIndicator);
			this.addChild(tf);
			
			/**
			 * You can replace the connector used for creating the TuioClient with any connector you'd like to use.
			 */
			this.tuio = new TuioClient(new TCPConnector());
			this.tuio.addListener(this);
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

		/**
		 * TuioObject handling.
		 */
		
		public function addTuioObject(tuioObject:TuioObject):void {
			objIndicator.blink();
			new Circle(tuioObject.sessionID.toString(), stage, tuioObject.x*stage.stageWidth, tuioObject.y * stage.stageHeight, this.circleSize, 0x33ee33);
		}
		
		public function updateTuioObject(tuioObject:TuioObject):void {
			objIndicator.blink();
			currentCircle = stage.getChildByName(tuioObject.sessionID.toString()) as Circle;
			currentCircle.x = tuioObject.x*stage.stageWidth;
			currentCircle.y = tuioObject.y*stage.stageHeight;
		}
		
		public function removeTuioObject(tuioObject:TuioObject):void {
			objIndicator.blink();
			currentCircle = stage.getChildByName(tuioObject.sessionID.toString()) as Circle;
			stage.removeChild(currentCircle);
		}

		/**
		 * TuioCursor handling.
		 */

		public function addTuioCursor(tuioCursor:TuioCursor):void {
			curIndicator.blink();
			new Circle(tuioCursor.sessionID.toString(), stage, tuioCursor.x*stage.stageWidth, tuioCursor.y * stage.stageHeight, this.circleSize, 0xee3333);
		}
		
		public function updateTuioCursor(tuioCursor:TuioCursor):void {
			curIndicator.blink();
			currentCircle = stage.getChildByName(tuioCursor.sessionID.toString()) as Circle;
			currentCircle.x = tuioCursor.x*stage.stageWidth;
			currentCircle.y = tuioCursor.y*stage.stageHeight;
		}
		
		public function removeTuioCursor(tuioCursor:TuioCursor):void {
			curIndicator.blink();
			currentCircle = stage.getChildByName(tuioCursor.sessionID.toString()) as Circle;
			stage.removeChild(currentCircle);
		}
		
		/**
		 * TuioBlob handling.
		 */
		
		public function addTuioBlob(tuioBlob:TuioBlob):void {
			blbIndicator.blink();
			new Circle(tuioBlob.sessionID.toString(), stage, tuioBlob.x*stage.stageWidth, tuioBlob.y * stage.stageHeight, this.circleSize, 0x3333ee);
		}
		
		public function updateTuioBlob(tuioBlob:TuioBlob):void {
			blbIndicator.blink();
			currentCircle = stage.getChildByName(tuioBlob.sessionID.toString()) as Circle;
			currentCircle.x = tuioBlob.x*stage.stageWidth;
			currentCircle.y = tuioBlob.y*stage.stageHeight;
		}
		
		public function removeTuioBlob(tuioBlob:TuioBlob):void {
			blbIndicator.blink();
			currentCircle = stage.getChildByName(tuioBlob.sessionID.toString()) as Circle;
			stage.removeChild(currentCircle);
		}
		
		public function newFrame(id:uint):void {}
	
	}

}