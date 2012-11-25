package org.tuio.fiducial
{
	/**
	 * ITuioFiducialReceiver defines the interface of a fiducial callback class.
	 * Receivers are being registered in the class TuioFiducialDispatcher via
	 * TuioFiducialDispatcher.getInstance().registerReceiver(this,fiducialId);
	 * 
	 * @example The following example shows you a simple fiducial callback class:
		 * <listing version="3.0">
		 * public class SimpleFiducialCallbackClass extends Sprite implements ITuioFiducialReceiver
		*	{
		*			private var _fiducialSprite:Sprite;
		*		
		*			public function SimpleFiducialCallbackClass()
		*			{
		*				addEventListener(Event.ADDED_TO_STAGE, addedToStage);
		*				_fiducialSprite = new Sprite();
		*				_fiducialSprite.graphics.beginFill(0x99dd00);
		*				_fiducialSprite.graphics.drawRect(0,0, 80, 80);
		*				_fiducialSprite.graphics.endFill();
		*			}
		*		
		*			private function addedToStage(event:Event):void{
		*				TuioFiducialDispatcher.getInstance().registerReceiver(this,5);
		*			}
		*		
		*			public function onAdd(event:FiducialEvent):void
		*			{
		*				addChild(_fiducialSprite);
		*			}
		*		
		*			public function onNotifyRemoved(event:FiducialEvent, timeout:Number):void
		*			{
		*			}
		*		
		*			public function onNotifyReturned(event:FiducialEvent):void
		*			{
		*			}
		*		
		*			public function onRemove(event:FiducialEvent):void
		*			{
		*				removeChild(_fiducialSprite);
		*			}
		*		
		*			public function onMove(event:FiducialEvent):void
		*			{
		*				this.x = event.x;
		*				this.y = event.y;
		*			}
		*		
		*			public function onRotate(event:FiducialEvent):void
		*			{
		*				this.rotation = event.rotation;
		* }
		 * }
		 * </listing>
		 * 
	 * @author Johannes Luderschmidt
	 */
	public interface ITuioFiducialReceiver
	{
		
		/**
		 * is being called as soon as a fiducial with the id that has been registered 
		 * with <code>TuioFiducialDispatcher</code> is added to the tabletop.
		 * 
		 * @param event 
		 * 
		 */
		function onAdd(event:FiducialEvent):void;
		
		
		/**
		 * is being called as soon as a fiducial with the id that has been registered 
		 * with <code>TuioFiducialDispatcher</code> has been removed from arriving Tuio messages. 
		 * However, this function is NOT the same as <code>onRemove</code>: <code>onNotifyRemoved</code> is called 
		 * if the fiducial
		 * has been marked as removed in <code>TuioFiducialDispatcher</code>. As tracking applications
		 * like reacTIVision can temporarily lose fiducials on its surface while tracking
		 * <code>onNotifyRemoved</code> is being called whenever the fiducial could not be tracked.
		 * 
		 * <p>If you showed a sprite round the fiducial and you would remove this sprite whenever
		 * <code>onNotifyRemoved</code> is being called and add it again with <code>onNotifyReturned</code> if it returns the 
		 * sprite could start flickering.
		 * Thus, <code>TuioFiducialDispatcher</code> waits for a timeout period in which the fiducial id will not
		 * return. If the fiducial returns within that time <code>onNotifyReturned</code> will be called. 
		 * If the fiducial does not return <code>onRemove</code> will be called.</p>
		 * 
		 * @see onNotifyReturned
		 * @see onRemove
		 * 
		 * @param event
		 * @param timeout contains the 
		 * 
		 */
		function onNotifyRemoved(event:FiducialEvent, timeout:Number):void;
		
		/**
		 * If a fiducial that has been removed from the Tuio stream returns within a certain timeout <code>onNotifyReturned</code> will be called.
		 * 
		 * @param event
		 * 
		 * @see onNotifyRemoved
		 * @see onRemove
		 */
		function onNotifyReturned(event:FiducialEvent):void;
		
		/**
		 * If a fiducial that has been removed from the Tuio stream does not return within a certain timeout <code>onRemove</code> will be called.
		 * 
		 * @param event
		 * 
		 * @see onNotifyRemoved
		 * @see onRemove
		 */
		function onRemove(event:FiducialEvent):void;
		
		/**
		 * is being called as soon as a fiducial with the id that has been registered 
		 * with TuioFiducialDispatcher has been moved on the tabletop.
		 * 
		 * @param event
		 * 
		 */
		function onMove(event:FiducialEvent):void;
		
		/**
		 * is being called as soon as a fiducial with the id that has been registered 
		 * with TuioFiducialDispatcher has been rotated.
		 * 
		 * @param event
		 * 
		 */
		function onRotate(event:FiducialEvent):void;
	}
}