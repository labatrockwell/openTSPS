// include required files
/** @namespace LAB.geom*/
LAB.geom = LAB.geom || {};

/** 
 @constructor 
**/
LAB.geom.Point = function( x, y, z){
	this.x = x || 0;
	this.y = y || 0;
	this.z = z || 0;
}