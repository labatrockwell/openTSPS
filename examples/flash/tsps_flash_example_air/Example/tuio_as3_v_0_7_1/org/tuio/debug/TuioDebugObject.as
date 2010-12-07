package org.tuio.debug
{
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.geom.Matrix;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;

	/**
	 * <p>Default implementation of the debug object square that is being shown by <code>TuioDebug</code> for
	 * each tuio object.</p>
	 * 
	 * <p><code>TuioDebugObject</code> implements <code>ITuioDebugObject</code> in order to be marked as debug
	 * information and not as the content of the actual application. This is necessary for the event dispatching
	 * mechanism in <code>TuioManager</code>.</p>
	 *  
	 * @see ITuioDebugObject
	 *   
	 * @author Johannes Luderschmidt
	 * 
	 */
	public class TuioDebugObject extends Sprite implements ITuioDebugObject
	{
		/**
		 * 
		 * @param objectId fiducial id
		 * @param width of the square
		 * @param height of the square
		 * @param color of the square's fill.
		 * @param alpha of the square's fill.
		 * @param lineThickness thickness of the line around the square.
		 * @param lineColor color of the line around the square.
		 * @param lineAlpha alpha of the line around the square.
		 * 
		 */
		public function TuioDebugObject(objectId:Number, width:Number, height:Number, color:Number, alpha:Number, lineThickness:Number, lineColor:Number, lineAlpha:Number){
			super();
			adjustGraphics(objectId, width, height, color, alpha, lineThickness, lineColor, lineAlpha);
		}
		
		/**
		 * carries out the Graphics drawing.
		 * 
		 * @param objectId fiducial id
		 * @param width of the square
		 * @param height of the square
		 * @param color of the square's fill.
		 * @param alpha of the square's fill.
		 * @param lineThickness thickness of the line around the square.
		 * @param lineColor color of the line around the square.
		 * @param lineAlpha alpha of the line around the square.
		 * 
		 */
		public function adjustGraphics(objectId:Number, width:Number, height:Number, color:Number, alpha:Number, lineThickness:Number, lineColor:Number, lineAlpha:Number):void{
			//draw object rect
			this.graphics.clear();
			this.graphics.beginFill(color,alpha);
			this.graphics.lineStyle(lineThickness, lineColor, lineAlpha);
			this.graphics.drawRect(-0.5*width, -0.5*height, width,height);
			this.graphics.endFill();
			
			//draw direction line
			this.graphics.lineStyle(3, 0x0, 1);
			this.graphics.moveTo(0,0);
			this.graphics.lineTo(0,-0.5*height+5);
			
			//draw objectid label
			var objectIdLabel:TextField = new TextField();
            objectIdLabel.autoSize = TextFieldAutoSize.LEFT;
            objectIdLabel.background = false;
            objectIdLabel.border = false;
			objectIdLabel.text = ""+objectId;
			objectIdLabel.width/2+5;
            objectIdLabel.defaultTextFormat = objectIdTextFormat();
            objectIdLabel.setTextFormat(objectIdTextFormat());
            
            var translationX:Number = -0.5*width+0.5*objectIdLabel.width;
            var translationY:Number = 0.5*height-0.5*objectIdLabel.height;
            //copy TextField into a bitmap
			var typeTextBitmap : BitmapData = new BitmapData(objectIdLabel.width, 
			                                objectIdLabel.height,true,0x00000000);
			typeTextBitmap.draw(objectIdLabel);
			 
			//calculate center of TextField
			var typeTextTranslationX:Number = -0.5*objectIdLabel.width+translationX+5;
			var typeTextTranslationY:Number = -0.5*objectIdLabel.height+translationY-5;
			 
			//create Matrix which moves the TextField to the center
			var matrix:Matrix = new Matrix();
			matrix.translate(typeTextTranslationX, typeTextTranslationY);
			
			//actually draw the text on the stage (with no-repeat and anti-aliasing)
			this.graphics.beginBitmapFill(typeTextBitmap,matrix,false,true);
			this.graphics.lineStyle(0,0,0);
			this.graphics.drawRect(typeTextTranslationX, typeTextTranslationY, 
			                                objectIdLabel.width, objectIdLabel.height);
			this.graphics.endFill();
		}
		
		private function objectIdTextFormat():TextFormat{
			var format:TextFormat = new TextFormat();
            format.font = "Arial";
            format.color = 0xffffff;
            format.size = 11;
            format.underline = false;
	            
        	return format;
		}
	}
}