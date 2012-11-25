package org.tuio {

	/**
	 * This is a generic class that contains values present in every profile specified in TUIO 1.1
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioContainer {
		
		internal var _sessionID:uint;
		internal var _x:Number;
		internal var _y:Number;
		internal var _z:Number;
		internal var _X:Number;
		internal var _Y:Number;
		internal var _Z:Number;
		internal var _m:Number;
		internal var _type:String;
		internal var _frameID:uint;
		
		public var isAlive:Boolean;
		
		public function TuioContainer(type:String, sID:Number, x:Number, y:Number, z:Number, X:Number, Y:Number, Z:Number, m:Number, frameID:uint) {
			this._type = type;
			this._sessionID = sID;
			this._x = x;
			this._y = y;
			this._z = z;
			this._X = X;
			this._Y = Y;
			this._Z = Z;
			this._m = m;
			this._frameID = frameID;
			this.isAlive = true;
		}
		
		public function get type():String {
			return this._type;
		}
		
		public function get sessionID():uint {
			return this._sessionID;
		}
		
		public function get x():Number {
			return this._x;
		}
		
		public function get y():Number {
			return this._y;
		}
		
		public function get z():Number {
			return this._z;
		}
		
		public function get X():Number {
			return this._X;
		}
		
		public function get Y():Number {
			return this._Y;
		}
		
		public function get Z():Number {
			return this._Z;
		}
		
		public function get m():Number {
			return this._m;
		}
		
		public function get frameID():uint {
			return this._frameID;
		}
	}
	
}