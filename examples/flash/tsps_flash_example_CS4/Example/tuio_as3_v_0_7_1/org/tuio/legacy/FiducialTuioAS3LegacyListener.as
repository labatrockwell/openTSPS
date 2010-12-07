package org.tuio.legacy
{
	
	import flash.events.EventDispatcher;
	import flash.display.Stage;
	
	import org.tuio.ITuioListener;
	import org.tuio.TuioBlob;
	import org.tuio.TuioClient;
	import org.tuio.TuioCursor;
	import org.tuio.TuioObject;

	
	/**
	 * <p><code>ITuioListener</code> version of fiducialtuioas3 (http://code.google.com/p/fiducialtuioas3/).
	 * fiducialtuioas3 calls callback functions of classes that implement the <code>PropHandleble</code>
	 * interface and are registered with <code>FiducialTuioAS3LegacyListener</code>.</p>
	 * 
	 * <p>As the package name implies <code>FiducialTuioAS3LegacyListener</code> is legacy code. For a newer version
	 * of a fiducial callback implementation see <code>org.tuio.fiducial.TuioFiducialDispatcher</code> and 
	 * <code>org.tuio.fiducial.ITuioFiducialReceiver</code>.</p>  
	 * 
	 * @see TUIOManager
	 * @see PropHandleble
	 * @see PropEvent 
	 * 
	 * @see http://code.google.com/p/fiducialtuioas3/
	 * 
	 * @author Johannes Luderschmidt
	 * 
	 */
	public class FiducialTuioAS3LegacyListener implements ITuioListener{
		private var stage:Stage;
		private var tuioClient:TuioClient;
		private var tuioManager:TUIOManager;
		public var evtDispatcher:EventDispatcher;
		
		private static var allowInst:Boolean;
		private static var inst:FiducialTuioAS3LegacyListener;
		
		/**
		 * As FiducialTuioAS3LegacyListener has been implemented as a Singleton class this constructor cannot be 
		 * called directly. Call <code>init(...)</code> instead.  
		 * 
		 * @param stage
		 * @param tuioClient
		 * @param tuioManager
		 * 
		 * @see init
		 * 
		 */
		public function FiducialTuioAS3LegacyListener(stage:Stage, tuioClient:TuioClient, tuioManager:TUIOManager){
			if (!allowInst) {
	            throw new Error("Error: Instantiation failed: Use FiducialTuioAS3LegacyListener.getInstance() instead of new.");
			}else{
				this.stage = stage;
				this.tuioClient = tuioClient;
				this.tuioManager = tuioManager;
//				this.tuioClient.addListener(this);
				evtDispatcher = new EventDispatcher();
			}
		}
		
		/**
		 * initializes Singleton instance of <code>FiducialTuioAS3LegacyListener</code>. Must be called before an
		 * instance of <code>FiducialTuioAS3LegacyListener</code> can be retrieved via <code>getInstance()</code>.
		 *  
		 * @param stage
		 * @param tuioClient
		 * @param tuioManager
		 * @return 
		 * 
		 */
		public static function init(stage:Stage, tuioClient:TuioClient, tuioManager:TUIOManager):FiducialTuioAS3LegacyListener{
			if(inst == null){
				allowInst = true;
				inst = new FiducialTuioAS3LegacyListener(stage, tuioClient, tuioManager);
				allowInst = false;
			}
			
			return inst;
		}
		
		/**
		 * gets Singleton instance
		 */
		public static function getInstance():FiducialTuioAS3LegacyListener{
			if(inst == null){
				throw new Error("Please initialize with method init(...) first!");
			}
			return inst;
		}
		
		/**
		 * <code>ITuioListener</code> callback method.
		 *  
		 * @param tuioObject
		 * 
		 */
		public function addTuioObject(tuioObject:TuioObject):void{
			var propSetEvent:PropEvent = new PropEvent(PropEvent.SET_PROP, tuioObject.sessionID, tuioObject.classID, tuioObject.x*stage.stageWidth, tuioObject.y*stage.stageHeight, tuioObject.a, tuioObject.X, tuioObject.Y, tuioObject.A, tuioObject.m, tuioObject.r, Math.sqrt((tuioObject.X*tuioObject.X)+(tuioObject.Y*tuioObject.Y))); 
			tuioManager.onPropSet(propSetEvent);
		}
		
		/**
		 * <code>ITuioListener</code> callback method.
		 *  
		 * @param tuioObject
		 * 
		 */
		public function updateTuioObject(tuioObject:TuioObject):void{
			var propSetEvent:PropEvent = new PropEvent(PropEvent.SET_PROP, tuioObject.sessionID, tuioObject.classID, tuioObject.x*stage.stageWidth, tuioObject.y*stage.stageHeight, tuioObject.a, tuioObject.X, tuioObject.Y, tuioObject.A, tuioObject.m, tuioObject.r, Math.sqrt((tuioObject.X*tuioObject.X)+(tuioObject.Y*tuioObject.Y))); 
			tuioManager.onPropSet(propSetEvent);
			var propAliveEvent:PropEvent = new PropEvent(PropEvent.ALIVE_PROP, tuioObject.sessionID, tuioObject.classID, tuioObject.x*stage.stageWidth, tuioObject.y*stage.stageHeight, tuioObject.a, tuioObject.X, tuioObject.Y, tuioObject.A, tuioObject.m, tuioObject.r, Math.sqrt((tuioObject.X*tuioObject.X)+(tuioObject.Y*tuioObject.Y))); 
			tuioManager.onPropAlive(propAliveEvent);			
		}
		
		/**
		 * <code>ITuioListener</code> callback method.
		 *  
		 * @param tuioObject
		 * 
		 */
		public function removeTuioObject(tuioObject:TuioObject):void{
			
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function addTuioCursor(tuioCursor:TuioCursor):void
		{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function updateTuioCursor(tuioCursor:TuioCursor):void
		{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioCursor
		 * 
		 */
		public function removeTuioCursor(tuioCursor:TuioCursor):void
		{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioBlob
		 * 
		 */
		public function addTuioBlob(tuioBlob:TuioBlob):void
		{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioBlob
		 * 
		 */
		public function updateTuioBlob(tuioBlob:TuioBlob):void
		{
		}
		
		/**
		 * <code>ITuioListener</code> callback method. Not implemented.
		 *  
		 * @param tuioBlob
		 * 
		 */
		public function removeTuioBlob(tuioBlob:TuioBlob):void
		{
		}
		
		/**
		 * Called if a new frameID is received.
		 * @param	id The new frameID
		 */
		public function newFrame(id:uint):void{}
	}
}