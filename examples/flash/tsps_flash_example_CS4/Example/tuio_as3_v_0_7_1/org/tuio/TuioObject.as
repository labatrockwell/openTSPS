
package org.tuio {
	
	/**
	 * This class represents a TuioObject as specified in TUIO 1.1
	 * 
	 * @author Immanuel Bauer
	 */
	public class TuioObject extends TuioContainer {
		
		private var _id:uint;
		private var _a:Number;
		private var _b:Number;
		private var _c:Number;
		private var _A:Number;
		private var _B:Number;
		private var _C:Number;
		private var _r:Number;
		
		public function TuioObject(type:String, sID:Number, i:Number, x:Number, y:Number, z:Number, a:Number, b:Number, c:Number, X:Number, Y:Number, Z:Number, A:Number, B:Number, C:Number, m:Number, r:Number, frameID:uint):void {
			super(type, sID, x, y, z, X, Y, Z, m, frameID);
			
			this._id = i;
			this._a = a;
			this._b = b;
			this._c = c;
			this._A = A;
			this._B = B;
			this._C = C;
			this._r = r;
		}
		
		/* Updates the values of the TuioObject */
		public function update(x:Number, y:Number, z:Number, a:Number, b:Number, c:Number, X:Number, Y:Number, Z:Number, A:Number, B:Number, C:Number, m:Number, r:Number, frameID:uint):void {
			this._x = x;
			this._y = y;
			this._z = z;
			this._X = X;
			this._Y = Y;
			this._Z = Z;
			this._m = m;
			
			this._a = a;
			this._b = b;
			this._c = c;
			this._A = A;
			this._B = B;
			this._C = C;
			this._r = r;
			
			this._frameID = frameID;
		}
		
		public function get classID():uint {
			return this._id;
		}
		
		public function get a():Number {
			return this._a;
		}
		
		public function get b():Number {
			return this._b;
		}

		public function get c():Number {
			return this._c;
		}
		
		public function get A():Number {
			return this._A;
		}
		
		public function get B():Number {
			return this._B;
		}

		public function get C():Number {
			return this._C;
		}
		
		public function get r():Number {
			return this._r;
		}
		
		/* This function converts the TuioObjects values into a String for output purposes */
		public function toString():String {
			var out:String = "";
			out += "TuioObject(";
			out += "type: " + this._type;
			out += ", sessionID: " + this._sessionID;
			out += ", classID: " + this._id;
			out += ", x: " + this._x;
			out += ", y: " + this._y;
			out += ", z: " + this._z;
			out += ", a: " + this._a;
			out += ", b: " + this._b;
			out += ", c: " + this._c;
			out += ", X: " + this._X;
			out += ", Y: " + this._Y;
			out += ", Z: " + this._Z;
			out += ", A: " + this._A;
			out += ", B: " + this._B;
			out += ", C: " + this._C;
			out += ", m: " + this._m;
			out += ", r: " + this._r;
			out += ")";
			
			return out;
		}
		
		public function clone():TuioObject{
			return new TuioObject(_type,_sessionID, _id, _x, _y, _z, _a, _b, _c, _X, _Y, _Z, _A, _B, _C, _m, _r, _frameID);
		}
	}
	
}