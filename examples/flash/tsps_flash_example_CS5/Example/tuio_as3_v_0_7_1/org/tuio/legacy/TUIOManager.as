package org.tuio.legacy
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.TimerEvent;
	import flash.ui.Keyboard;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	
	import mx.core.Application;
	
	import org.tuio.TuioClient;
	import org.tuio.connectors.UDPConnector;
	import org.tuio.debug.TuioDebug;
	import org.tuio.fiducial.TuioFiducialDispatcher;
	
	/**
	 * Legacy TUIOManager from fiducialtuioas3 (http://code.google.com/p/fiducialtuioas3/).
	 * 
	 * For a newer version of a fiducial callback implementation see <code>org.tuio.fiducial.TuioFiducialDispatcher</code> and 
	 * <code>org.tuio.fiducial.ITuioFiducialReceiver</code>.  
	 * 
	 * @author Frederic Friess
	 * 
	 */
	public class TUIOManager extends Sprite{
		
		private var PropObjectDict:Dictionary;
		private var AliveDict:Dictionary;
		
		private var timeInMillisec:Number;
		
		
		public function TUIOManager(){
			
			PropObjectDict = new Dictionary();
			AliveDict = new Dictionary();
			var RemoveChecker:Timer = new Timer(1000, 0);
			
			addEventListener(Event.ADDED_TO_STAGE, onStageAdd);
			
			RemoveChecker.addEventListener(TimerEvent.TIMER, timerHandler);
			RemoveChecker.start();
			
			
			this.timeInMillisec = 0;
		}
		
		// for Hardware Test
		public function makeHotSpotImage(evt:KeyboardEvent):void{
			
			if(evt.charCode == Keyboard.ENTER){
//				trace("start:");
				var time:Date = new Date();
				this.timeInMillisec = (time.getSeconds()*1000) +  time.getMilliseconds();
			}
		}
		
		// for Hardware Test
		public function Touchhandler(evt:TouchEvent):void{
			var time:Date = new Date();
			var temp:Number = (time.getSeconds()*1000) +  time.getMilliseconds();
//			trace("Lag of "+String( ((temp - this.timeInMillisec)/1000))+" sek");
			this.timeInMillisec = 0;
		}
		
		private function onStageAdd(evt:Event=null):void{
			

			var tuio:TuioClient = 
				new TuioClient(new UDPConnector());
			var legacyListener:TuioLegacyListener = TuioLegacyListener.init(stage, tuio);
			var tuioDebug:TuioDebug = TuioDebug.init(stage);
			tuioDebug.showDebugText = false;
			tuioDebug.cursorLineColor = 0xcccccc;
			tuioDebug.cursorLineAlpha = 1;
			tuioDebug.cursorLineThickness = 5;
			var fiducialLegacyListener:FiducialTuioAS3LegacyListener = FiducialTuioAS3LegacyListener.init(stage, tuio, this);
			tuio.addListener(legacyListener);
			tuio.addListener(tuioDebug);
			tuio.addListener(fiducialLegacyListener);
			tuio.addListener(TuioFiducialDispatcher.init(stage,1000));
			
			Application.application.stage.addEventListener(TouchEvent.MOUSE_DOWN, Touchhandler);
			Application.application.stage.addEventListener(KeyboardEvent.KEY_DOWN, makeHotSpotImage);
			
			removeEventListener(Event.ADDED_TO_STAGE, onStageAdd);
		}
		
		
		
		private function timerHandler(evt:TimerEvent):void{
			//speichern der statuse der Props vor der änderung
			var preAliveArray:Array = new Array();
			for (var f_id in PropObjectDict) {
				//trace(f_id+"----------")
				var prop:PropObject = PropObject(PropObjectDict[f_id]);
				if(prop.isActiv){
					preAliveArray.push(f_id);
				}else{
					preAliveArray.push(0);
				}
			}
			
			// alle auf 0 setzen
			var tempArray:Array = new Array();
			for (var f_id in PropObjectDict) {
				tempArray.push(0);
			}
			
			// alle deaktivieren
			for (var f_id in PropObjectDict) {
				var prop:PropObject = PropObject(PropObjectDict[f_id]); 
				prop.isActiv = false;
			}
			
		
			// die lebenden wieder aktivieren
			for (var f_id in PropObjectDict) {
				var prop:PropObject = PropObject(PropObjectDict[f_id]); 
				var s_id = prop.s_id;
				for (var alive_s_id in AliveDict){
					if(s_id == alive_s_id){
						prop.isActiv = true;
					}
				}
			}	
			AliveDict = new Dictionary();
			
			
			//speichern der statuse der Props nach der änderung
			var postAliveArray:Array = new Array();
			for (var f_id in PropObjectDict) {
				//trace(f_id+"----------")
				var prop:PropObject = PropObject(PropObjectDict[f_id]);
				if(prop.isActiv){
					postAliveArray.push(f_id);
				}else{
					postAliveArray.push(0);
				}
			}
			
			// die gestorbenen melden
			for (var i=0; i < postAliveArray.length; i++){
				if( (preAliveArray[i] != postAliveArray[i]) && (postAliveArray[i] == 0)){
					var f_id = preAliveArray[i];
					var prop:PropObject = PropObject(PropObjectDict[f_id]);
					prop.dispatchEvent(new PropEvent(PropEvent.REMOVE_PROP, prop.s_id, prop.f_id));
				}
			}
		}
		


		public function getProp(id:Number):PropObject{
			if (this.PropObjectDict[id] == null){
				
				var evt:PropEvent = new PropEvent(PropEvent.SET_PROP,-1,id);
				PropObjectDict[id] = createProp(evt);
			}
			return this.PropObjectDict[id];	
		}
		
		
		private function createProp(evt:PropEvent):PropObject{
			//var spr:PropView = new PropView();
			var tmpProp:PropObject = new PropObject(evt.s_id,evt.f_id);
			return tmpProp;
		}
	
		
		public function onPropSet(evt:PropEvent):void{
			if (PropObjectDict[evt.f_id] != null){
				
				var prop:PropObject = this.getProp(evt.f_id);
				prop.set_s_ID(evt.s_id);
				
				// onAdd ueberprüfen
				if (!prop.isActiv){
					prop.dispatchEvent(new PropEvent(PropEvent.ADD_PROP, evt.s_id,evt.f_id, evt.xpos, evt.ypos, evt.angle, evt.xspeed, evt.yspeed, evt.rspeed, evt.maccel, evt.raccel, evt.speed, true, true));
				}
				prop.isActiv = true;
		
				prop.dispatchEvent(new PropEvent(PropEvent.MOVE_PROP,			evt.s_id, evt.f_id, evt.xpos, evt.ypos, evt.angle, evt.xspeed, evt.yspeed, evt.rspeed, evt.maccel,evt.raccel,evt.speed,true,true));
				prop.dispatchEvent(new PropEvent(PropEvent.ROTATE_PROP, 		evt.s_id, evt.f_id, evt.xpos, evt.ypos, evt.angle, evt.xspeed, evt.yspeed, evt.rspeed, evt.maccel,evt.raccel,evt.speed,true,true));
				prop.dispatchEvent(new PropEvent(PropEvent.VELOCETY_MOVE_PROP, 	evt.s_id, evt.f_id, evt.xpos, evt.ypos, evt.angle, evt.xspeed, evt.yspeed, evt.rspeed, evt.maccel,evt.raccel,evt.speed,true,true));
				prop.dispatchEvent(new PropEvent(PropEvent.VELOCETY_ROTATE_PROP,evt.s_id, evt.f_id, evt.xpos, evt.ypos, evt.angle, evt.xspeed, evt.yspeed, evt.rspeed, evt.maccel,evt.raccel,evt.speed,true,true));
				prop.dispatchEvent(new PropEvent(PropEvent.ACCEL_MOVE_PROP, 	evt.s_id, evt.f_id, evt.xpos, evt.ypos, evt.angle, evt.xspeed, evt.yspeed, evt.rspeed, evt.maccel,evt.raccel,evt.speed,true,true));
				prop.dispatchEvent(new PropEvent(PropEvent.ACCEL_ROTATE_PROP, 	evt.s_id, evt.f_id, evt.xpos, evt.ypos, evt.angle, evt.xspeed, evt.yspeed, evt.rspeed, evt.maccel,evt.raccel,evt.speed,true,true));
			}
		}
		
		
		
		public function onPropAlive(evt:PropEvent):void{
			AliveDict[evt.s_id] = true;
		}
	}
}