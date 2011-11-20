/** @namespace LAB.three */
LAB.three = LAB.three || {};

/** 
	@constructor 
*/

LAB.three.Object = function( _renderer, _scene ) {
   
   this.renderer = _renderer;
   this.scene = _scene || new THREE.Scene();;
   this.autoClear = true;
   this.labObj = new THREE.Object3D();
   
   //this isn't great. temporary stop gap
   this.posStack = [];
   this.rotStack = [];
   this.sclStack = [];
   
   this.scene.addObject( this.labObj );
};


LAB.three.Object.prototype = {
   
constructor: LAB.three.Object,

/**
	@function
	@public
*/
addGeometry: function( _geometry, _material){
   this.labObj.addChild( new THREE.Mesh( _geometry, _material  ) );
},

/**
	@function
	@public
*/
addObject: function( _object ){
   this.labObj.addChild( _object );
},

/**
	@function
	@public
*/
addChild: function( _child ){
   this.labObj.addChild( _child );
},

/**
	@function
	@public
*/
addLight: function( _light ){
   this.scene.addLight( _light );
},

/**
	@function
	@public
*/ 
draw: function( _camera, _material, _renderTarget )
{   
   this.scene.overrideMaterial = _material || null;
   this.renderer.render(this.scene, 
                        _camera,
                        _renderTarget || null,
                        this.autoClear );
},

/**
	@function
	@public
*/
translate: function( x,y,z ){
   this.labObj.position.x += x;
   this.labObj.position.y += y;
   this.labObj.position.z += z;
},
   
/**
	@function
	@public
*/
setTranslation: function( x,y,z ){
   this.labObj.position.x = x;
   this.labObj.position.y = y;
   this.labObj.position.z = z;
},
   
//rotate: function(angle, x,y,z ){
//   var rotMat = new THREE.Matrix4().setRotationAxis( new THREE.Vector3(x,y,z), angle * 0.0174532925 );
//   var rotVec = new THREE.Vector3();
//   rotVec.setRotationFromMatrix( rotMat );
//   this.labObj.rotation.addSelf( rotVec );
//},

/**
	@function
	@public
*/
setRotation: function( angle, x,y,z ){
   var rotMat = new THREE.Matrix4().setRotationAxis( new THREE.Vector3(x,y,z).normalize(), angle * 0.0174532925 );
   
   this.labObj.rotation.setRotationFromMatrix( rotMat );
},
   
/**
	@function
	@public
*/
scale: function( x, y, z){
   this.labObj.scale.set(this.labObj.scale.x * x,
                       this.labObj.scale.y * y,
                       this.labObj.scale.z * z);
},

/**
	@function
	@public
*/
setScale: function( x, y, z){
   this.labObj.scale.set( x, y, z );
},
   
/**
	@function
	@public
*/
multMatrix: function( m ){
  //TODO: we might need to make this an extension of THREE.Object3D so that we can override updateMatrix() which is called by the render  er
},
   
/**
	@function
	@public
*/
pushMatrix: function(){
   this.posStack.push( this.labObj.position );
   this.rotStack.push( this.labObj.rotation );
   this.sclStack.push( this.labObj.scale );
},
   
/**
	@function
	@public
*/
popMatrix: function(){
   if( this.posStack.length ){
   this.labObj.position.copy( this.posStack.pop() );
   this.labObj.rotation.copy( this.rotStack.pop() );
   this.labObj.scale.copy( this.sclStack.pop() );
   }
},
   
///**
// @function
// @public
// */ 
//updateModelViewMatrices: function(){
//      //   smoothMat.__webglInit = false;//true
//      //   smoothMat._modelViewMatrix = new THREE.Matrix4();
//      //   smoothMat._normalMatrixArray = new Float32Array( 9 );
//      //   smoothMat._modelViewMatrixArray = new Float32Array( 16 );
//      //   smoothMat._objectMatrixArray = new Float32Array( 16 );
//      //   smoothMat.matrixWorld.flattenToArray( smoothMat._objectMatrixArray );
//   },
//
///**
// @function
// @public
// */
//setupMatrices: function( object, camera, computeNormalMatrix ) {
//      
//      this._modelViewMatrix.multiplyT§oArray( camera.matrixWorldInverse, this.matrixWorld, this._modelViewMatrixArray );
//      
//      if ( computeNormalMatrix ) {
//         
//         THREE.Matrix4.makeInvert3x3( this._modelViewMatrix ).transposeIntoArray( this._normalMatrixArray );
//         
//      }
//      
//   },
      
};
