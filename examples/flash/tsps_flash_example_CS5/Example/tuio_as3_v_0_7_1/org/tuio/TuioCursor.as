package org.tuio {

	/**
	 * This class represents a TuioCursor as specified in TUIO 1.1
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioCursor extends TuioContainer {
		
		public function TuioCursor(type:String, sID:Number, x:Number, y:Number, z:Number, X:Number, Y:Number, Z:Number, m:Number, frameID:uint ) {
			super(type, sID, x, y, z, X, Y, Z, m, frameID);
		}
	
		/* Updates the values of the TuioCursor */
		public function update(x:Number, y:Number, z:Number, X:Number, Y:Number, Z:Number, m:Number, frameID:uint):void {
			this._x = x;
			this._y = y;
			this._z = z;
			this._X = X;
			this._Y = Y;
			this._Z = Z;
			this._m = m;
			this._frameID = frameID;
		}
		
		/* This function converts the TuioCursors values into a String for output purposes */
		public function toString():String {
			var out:String = "";
			out += "TuioCursor(";
			out += "type: " + this._type;
			out += ", sessionID: " + this._sessionID;
			out += ", x: " + this._x;
			out += ", y: " + this._y;
			out += ", z: " + this._z;
			out += ", X: " + this._X;
			out += ", Y: " + this._Y;
			out += ", Z: " + this._Z;
			out += ", m: " + this._m;
			out += ")";
			
			return out;
		}
		
	}
	
}