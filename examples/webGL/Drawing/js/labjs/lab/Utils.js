/** @namespace LAB*/
LAB = LAB || {};

/********************************************
	MATH
********************************************/

/**
 @function
 */
LAB.random 			= function( _min, _max){
   return Math.random() * ( _max - _min ) + _min;
}
/**
 @function
 */
LAB.randomInt  		= function( _min, _max) {
   return Math.floor( LAB.random( _min, _max ));
}
/**
 @function
 */
LAB.randomObject  	= function( _array ){
   return _array[ Math.min(LAB.randomInt(0, _array.length ), _array.length-1)];
}
/**
 @function
 */
LAB.map				= function(value, _oldMin, _oldMax, _min, _max){    
   return _min + ((value-_oldMin)/(_oldMax-_oldMin)) * (_max-_min);
}
/**
 @function
 */
LAB.clamp 			= function( value, _min, _max ){
   return Math.min( Math.max( value, _min), _max );
}

/**
 @function
 */
LAB.degToRad		= function( deg ){
   return deg * 0.0174532925;
}
/**
 @function
 */
LAB.radToDeg		= function( rad ){
   return rad * 57.2957795;
}

/**
 * @returns The sign of this number, 0 if the number is 0.
 * example: aNumber.x.sign();
 */
Number.prototype.sign = function() {
  if(this > 0) {
    return 1;
  } else if (this < 0) {
    return -1;
  } else {
    return 0;
  }
}

/********************************************
	LOGGING
********************************************/
/**
 @function
 */
LAB.log				= function( text ) {
   if (window.console && window.console.log) {
      window.console.log( text );
   } else if (window.dump) {
      window.dump( text );
   }
}

/********************************************
	ANIMATION
********************************************/

	//http://paulirish.com/2011/requestanimationframe-for-smart-animating/
	/** 
		@function 
		@public
	*/
	if ( !window.requestAnimationFrame ) {

		window.requestAnimationFrame = ( function() {

			return window.webkitRequestAnimationFrame ||
			window.mozRequestAnimationFrame ||
			window.oRequestAnimationFrame ||
			window.msRequestAnimationFrame ||
			function( /* function FrameRequestCallback */ callback, /* DOMElement Element */ element ) {

				window.setTimeout( callback, 1000 / 60 );

			};

		} )();
	}
	
	// TO DO: window.cancelAnimationFrame

/********************************************
	BROWSER UTILS
********************************************/
	
	// get string from url. e.g. www.lab.com/index.html?sandwich=turkey returns 'turkey'
	/**
	 @function
	 */
	LAB.getQueryString = function (key)
	{
		key = key.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
		var regex = new RegExp("[\\?&]"+key+"=([^&#]*)");
		var qs = regex.exec(window.location.href);
		if(qs == null)
			return '';
		else
			return qs[1];
	}