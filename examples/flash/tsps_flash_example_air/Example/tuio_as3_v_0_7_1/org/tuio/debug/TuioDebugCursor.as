package org.tuio.debug
{
	import flash.display.Sprite;

	/**
	 * <p>Default implementation of the debug cursor circle that is being shown by <code>TuioDebug</code> for
	 * each tuio cursor.</p>
	 * 
	 * <p><code>TuioDebugCursor</code> implements <code>ITuioDebugCursor</code> in order to be marked as debug
	 * information and not as the content of the actual application. This is necessary for the event dispatching
	 * mechanism in <code>TuioManager</code>.</p>
	 *  
	 * @see ITuioDebugCursor
	 *   
	 * @author Johannes Luderschmidt
	 * 
	 */
	public class TuioDebugCursor extends Sprite implements ITuioDebugCursor
	{
		private var _sessionId:uint;
		
		/**
		 * 
		 * @param radius of the debug circle.
		 * @param color of the circle's fill.
		 * @param alpha of the circle's fill.
		 * @param lineThickness thickness of the line around the circle.
		 * @param lineColor color of the line around the circle.
		 * @param lineAlpha alpha of the line around the circle.
		 * 
		 */
		public function TuioDebugCursor(radius:Number, color:Number, alpha:Number, lineThickness:Number, lineColor:Number, lineAlpha:Number){
			super();
			adjustGraphics(radius, color, alpha, lineThickness, lineColor, lineAlpha);
		}
		
		
		/**
		 * carries out the Graphics drawing.
		 * 
		 * @param radius of the debug circle.
		 * @param color of the circle's fill.
		 * @param alpha of the circle's fill.
		 * @param lineThickness thickness of the line around the circle.
		 * @param lineColor color of the line around the circle.
		 * @param lineAlpha alpha of the line around the circle.
		 * 
		 */
		public function adjustGraphics(radius:Number, color:Number, alpha:Number, lineThickness:Number, lineColor:Number, lineAlpha:Number):void{
			this.graphics.beginFill(color,alpha);
			this.graphics.lineStyle(lineThickness, lineColor, lineAlpha);
			this.graphics.drawCircle(0, 0, radius);
			this.graphics.endFill();
		}
		
		
		public function get sessionId():uint{
			return _sessionId;
		}
		
		public function set sessionId(sessionId:uint):void{
			this._sessionId = sessionId;
		}
	}
}