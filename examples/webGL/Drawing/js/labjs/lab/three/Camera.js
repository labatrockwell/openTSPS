/** @namespace LAB.three */
LAB.three = LAB.three || {};

/** 
 @constructor 
 @extends THREE.Camera
 */
LAB.three.Camera = function ( fov, aspect, near, far ) {
   THREE.Camera.call( this, fov, aspect, near, far );// I think this works, seems to work...
   this.matrix.identity();
   this.mvMatrixStack = [];
   
   //   this.setToWindowPerspective();
   this.useTarget = true;
   this.useQuaternion = true;
   this.bUsePushPop = false;
   
   this.setToWindowPerspective();
};

LAB.three.Camera.prototype = new THREE.Camera();
LAB.three.Camera.prototype.constructor = LAB.three.Camera;
LAB.three.Camera.prototype.supr = THREE.Camera.prototype;

/**
 @function
 @public
 */
LAB.three.Camera.prototype.usePushPop = function( _bUsePushPop ){
   
   this.bUsePushPop = _bUsePushPop || true;
   this.useTarget = false;
};

/**
 @function
 @public
 */
LAB.three.Camera.prototype.updateMatrix = function () {
   
   //this bypasses the THREE.Camera.updateMatrix() called by the renderer
   //without this the camera would automaticllay update according to it's position scale and rotation vectors
   
   
   //if we're not using push&pop then we need to update the matrix 
   if( this.bUsePushPop == false ){
      //the following is copied from the THREE.Object3D method updateMatrix()
		this.matrix.setPosition( this.position );
      
		if ( this.useQuaternion )  {
         
			this.matrix.setRotationFromQuaternion( this.quaternion );
         
		} else {
         
			this.matrix.setRotationFromEuler( this.rotation, this.eulerOrder );
         
		}
      
		if ( this.scale.x !== 1 || this.scale.y !== 1 || this.scale.z !== 1 ) {
         
			this.matrix.scale( this.scale );
			this.boundRadiusScale = Math.max( this.scale.x, Math.max( this.scale.y, this.scale.z ) );
         
		}
      
		this.matrixWorldNeedsUpdate = true;
   } else {
		this.position.setPositionFromMatrix(this.matrix);
	}
};



/**
 @function
 @public
 */
LAB.three.Camera.prototype.setToWindowPerspective = function( _fov, _nearClip, _farClip ){

   var fov = _fov || 60;
   
	var viewW = window.innerWidth;
	var viewH = window.innerHeight;
   
	var eyeX = viewW / 2;
	var eyeY = viewH / 2;
	var halfFov = Math.PI * fov / 360;
	var theTan = Math.tan(halfFov);
	var dist = eyeY / theTan;
	var aspect = viewW / viewH;
   
   var near = _nearClip || dist / 10;
   var far = _farClip || dist * 10;
   this.projectionMatrix = THREE.Matrix4.makePerspective( fov, aspect, near, far );
   
   this.position.set( eyeX, eyeY, dist );
   this.target.position.set( eyeX, eyeY, 0 );
   this.up.set( 0, 1, 0 );
   
   this.matrix.lookAt( this.position, this.target.position, this.up );
   this.matrix.setPosition( this.position );
   
   
   //	if(ofDoesHWOrientation()){
   //		if(vFlip){
   //			glScalef(1, -1, 1);
   //			glTranslatef(0, -height, 0);
   //		}
   //	}
};

/**
 project a THREE.Vector3 from world to screen coords
 @function
 @public
 */
LAB.three.Camera.prototype.projectToScreen = function( worldPos ){
   //adaptded from https://github.com/mrdoob/three.js/issues/78
   var pos = worldPos.clone();
   projScreenMat = new THREE.Matrix4();
   projScreenMat.multiply( this.projectionMatrix, this.matrixWorldInverse );
   projScreenMat.multiplyVector3( pos );
   
   return { x: ( pos.x + 1 ) * window.innerWidth / 2 ,
   y: window.innerHeight - ( - pos.y + 1) * window.innerHeight / 2,
      z: 0 };
};

/**
 project a THREE.Vector3 from screen coords to world space
 @function
 @public
 */
LAB.three.Camera.prototype.projectToWorld = function( screenPos ){
   //this was helpful http://jsfiddle.net/gero3/PE4x7/25/
   var pos = screenPos.clone();
   pos.x = ( pos.x / window.innerWidth ) * 2 - 1;
   pos.y = ( -(window.innerHeight-pos.y) / window.innerHeight ) * 2 + 1;
   
   var projector = new THREE.Projector();
   projector.unprojectVector( pos, this );
   return pos;
};

/**
 @function
 @public
 */
LAB.three.Camera.prototype.pushMatrix = function(){
   this.mvMatrixStack.push( new THREE.Matrix4().copy( this.matrix ));
};

/**
 @function
 @public
 */
LAB.three.Camera.prototype.popMatrix = function(){
   if( this.mvMatrixStack.length > 0){
      this.matrix.copy( this.mvMatrixStack.pop() );
   }
};

/**
 @function
 @public
 */
LAB.three.Camera.prototype.lookAt = function( x, y, z ){
   if( this.bUsePushPop ){
      this.matrix.lookAt(this.matrix.getPosition(),//eye
                         new THREE.Vector3(x,y,z), //target
                         this.up );
   }else{
      this.useTarget = true;
      this.target.position.set( x,y,z );
   }
};

/**
 @function
 @public
 */
LAB.three.Camera.prototype.translateMatrix = function( x, y, z ){
   this.matrix.multiply( new THREE.Matrix4().setTranslation(x,y,z), this.matrix );
};


/**
 @function
 @public
 */
LAB.three.Camera.prototype.scaleMatrix = function( x, y, z ){
   
   this.matrix.n11 *= x;
   this.matrix.n12 *= x;
   this.matrix.n13 *= x;
   this.matrix.n14 *= x;
   
   this.matrix.n21 *= y;
   this.matrix.n22 *= y;
   this.matrix.n23 *= y;
   this.matrix.n24 *= y;
   
   this.matrix.n31 *= z;
   this.matrix.n32 *= z;
   this.matrix.n33 *= z;
   this.matrix.n34 *= z;
   
};

/**
 @function
 @public
 */
LAB.three.Camera.prototype.rotateMatrix = function( angle, x, y, z ){
   //adapted from tdl.fast.matrix4.axisRotate 
   var n = Math.sqrt(x * x + y * y + z * z);
   x /= n;
   y /= n;
   z /= n;
   var xx = x * x;
   var yy = y * y;
   var zz = z * z;
   var c = Math.cos(angle);
   var s = Math.sin(angle);
   var oneMinusCosine = 1 - c;
   
   var r00 = xx + (1 - xx) * c;
   var r01 = x * y * oneMinusCosine + z * s;
   var r02 = x * z * oneMinusCosine - y * s;
   var r10 = x * y * oneMinusCosine - z * s;
   var r11 = yy + (1 - yy) * c;
   var r12 = y * z * oneMinusCosine + x * s;
   var r20 = x * z * oneMinusCosine + y * s;
   var r21 = y * z * oneMinusCosine - x * s;
   var r22 = zz + (1 - zz) * c;
   
   var m00 = this.matrix.n11;
   var m01 = this.matrix.n12;
   var m02 = this.matrix.n13;
   var m03 = this.matrix.n14;
   var m10 = this.matrix.n21;
   var m11 = this.matrix.n22;
   var m12 = this.matrix.n23;
   var m13 = this.matrix.n24;
   var m20 = this.matrix.n31;
   var m21 = this.matrix.n32;
   var m22 = this.matrix.n33;
   var m23 = this.matrix.n34;
   var m30 = this.matrix.n41;
   var m31 = this.matrix.n42;
   var m32 = this.matrix.n43;
   var m33 = this.matrix.n44;
   
   this.matrix.n11 = r00 * m00 + r01 * m10 + r02 * m20;
   this.matrix.n12 = r00 * m01 + r01 * m11 + r02 * m21;
   this.matrix.n13 = r00 * m02 + r01 * m12 + r02 * m22;
   this.matrix.n14 = r00 * m03 + r01 * m13 + r02 * m23;
   this.matrix.n21 = r10 * m00 + r11 * m10 + r12 * m20;
   this.matrix.n22 = r10 * m01 + r11 * m11 + r12 * m21;
   this.matrix.n23 = r10 * m02 + r11 * m12 + r12 * m22;
   this.matrix.n24 = r10 * m03 + r11 * m13 + r12 * m23;
   this.matrix.n31 = r20 * m00 + r21 * m10 + r22 * m20;
   this.matrix.n32 = r20 * m01 + r21 * m11 + r22 * m21;
   this.matrix.n33 = r20 * m02 + r21 * m12 + r22 * m22;
   this.matrix.n34 = r20 * m03 + r21 * m13 + r22 * m23;
};

/**
 @function
 @public
 */

LAB.three.Camera.prototype.multMatrix = function( m ){
   this.matrix.multiply( m, this.matrix );
};

/**
 @function
 @public
 */
LAB.three.Camera.prototype.setToWindowOrtho = function( _nearClip, _farClip ){
   
   this.projectionMatrix = THREE.Matrix4.makeOrtho(window.innerWidth/-2, 
                                                   window.innerWidth/2,
                                                   window.innerHeight/2,
                                                   window.innerHeight/-2,
                                                   _nearClip || -1000,
                                                   _farClip || 1000 );
};

LAB.three.Camera.prototype.makeOrtho = function( _nearClip, _farClip ){
   
   this.projectionMatrix = THREE.Matrix4.makeOrtho(window.innerWidth/-2, 
                                                   window.innerWidth/2,
                                                   window.innerHeight/2,
                                                   window.innerHeight/-2,
                                                   _nearClip || -1000,
                                                   _farClip || 1000 );
};

