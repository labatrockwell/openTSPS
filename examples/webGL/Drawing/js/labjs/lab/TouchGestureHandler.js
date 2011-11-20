LAB.require(LAB.src+"geom/Point.js");

// TO DO: refactor to add functions to prototype

/**
 * A touch gesture handler that wraps mouse events. To use: create an instance of this
 * class and register either window or a DOM object (or any custom object that implements
 * EventDispatcher). Then listen for touch events (see TouchEvent) on the registerd object.
 *
 * @namespace LAB
 * @constructor
 * @augments LAB.EventDispatcher
 */
LAB.TouchGestureHandler = function() {
	"use strict";

	LAB.EventDispatcher.call(this, this);

	// private constants
	// tweak these to fine tune the gesture behavior
	var FLICK_TIMEOUT = 250,
		TAP_TIMEOUT = 80,
		FLICK_MIN_MOVEMENT = 20,
		DRAG_MIN_MOVEMENT = 5;
		
	// to do: add getters and setters for time and movement thresholds	
		
	// private variables
	var _self = this,
		_target = null,
		_isDrag = false,
		_isFlick = false,
		_pressPoint,
		_flickDirection,
		_flickDistance,
		_flickStartPoint,
		_flickEndPoint,
		_flickVelocity,
		_flickStartTime,
		_startTime,
		_endTime,
		_lastTime = -1,
		_averageVelocity = [],
		_velocity = [],
		_dragDirection,
		_dragDistance,
		_touchX,
		_touchY;
	
	/**
	 * @private
	 */
	function mouseDownHandler(event) {
	
		// events to listen to
		window.addEventListener("mouseup", mouseUpHandler);
		_target.addEventListener("mousemove", mouseMoveHandler);
		
		// events to remove
		_target.removeEventListener("mousedown", mouseDownHandler);
		
		startMove(event);
	}
	
	/**
	 * @private
	 */
	function startMove(event) {
		_touchX = event.clientX;
		_touchY = event.clientY;

		dispatch(LAB.TouchEvent.PRESS);
		
		var date = new Date();
		_startTime = date.getTime();
						
		_pressPoint = new LAB.geom.Point(event.clientX, event.clientY);
	}
	
	/**
	 * @private
	 */
	function mouseMoveHandler(event) {
		
		_touchX = event.clientX;
		_touchY = event.clientY;
		
		var date = new Date();
		
		var currentTime = date.getTime();
				
		// save current point
		var currentPoint = new LAB.geom.Point(_touchX, _touchY);
		
		// flick handling
		var distance = calcDistance(currentPoint, _pressPoint);
		var direction = calcDegree(currentPoint, _pressPoint);
		
		if (distance > FLICK_MIN_MOVEMENT) {
		
			var date;
		
			if (!_isFlick) {
				date = new Date();
				// get time and position the first time the flick movement threshold is met
				_flickStartTime = date.getTime();
				_flickStartPoint = new LAB.geom.Point(_touchX, _touchY);
			}
			
			_isFlick = true;
		}
				
		// drag handler
		// to do: only send drag events when an object is being dragged?
		if (distance > DRAG_MIN_MOVEMENT) {
			var velocity;
			// update the distance and direction each time the minimum drag movement
			// threshold is met
			_dragDistance = distance;
			_dragDirection = direction;
			
			var timeDiff = currentTime - _lastTime;
			
			velocity = calcVelocity(_dragDistance, timeDiff);
			if (velocity > -1) {
				_velocity.push(velocity);
			}
		
			_isDrag = true;
		}
		
		// to do: dispatch only on distance > 0?
		if (_isDrag) {
			// currently fired on each move event once the drag threshold has been met
			dispatch(LAB.TouchEvent.DRAG);
		}
		
		// update press point so we can have drag...drag...drag...flick
		_pressPoint = new LAB.geom.Point(currentPoint.x, currentPoint.y);
		_lastTime = currentTime;
	}
	
	/**
	 * This is important when using DOM objects so you know when you have
	 * dragged outside the object that the mouse down event was fired over
	 *
	 * @private
	 */
	function mouseOutHandler(event) {		
		// ignore if out is not this object
		if (event.target != _target) {
			return;
		}
		
		_target.removeEventListener("mouseout", mouseOutHandler);
		
		dispatch(LAB.TouchEvent.OUT);
	}	
	
	/**
	 * @private
	 */
	function mouseUpHandler(event) {
	
		_touchX = event.clientX;
		_touchY = event.clientY;
		
		_target.addEventListener("mousedown", mouseDownHandler);
		
		_target.removeEventListener("mousemove", mouseMoveHandler);
		window.removeEventListener("mouseup", mouseUpHandler);
		
		var date = new Date();
		_endTime = date.getTime();
		
		var timeChange = _endTime - _startTime;
		var flickTimeChange = _endTime - _flickStartTime;
		
		if (timeChange <= TAP_TIMEOUT && !_isDrag && !_isFlick) {
			dispatch(LAB.TouchEvent.TAP);
		}
		
		var velocitySum = 0;
		for (var i = 0, len = _velocity.length; i< len; i++) {
			velocitySum = velocitySum + _velocity[i];
		}
		_averageVelocity = velocitySum / _velocity.length;
		
		// must meet minimum time requirement for flick
		if (flickTimeChange <= FLICK_TIMEOUT && _isFlick) {
			
			_flickEndPoint = new LAB.geom.Point(_touchX, _touchY);
			_flickDirection = calcDegree(_flickEndPoint, _flickStartPoint);
			_flickDistance = calcDistance(_flickEndPoint, _flickStartPoint);
			_flickVelocity = calcVelocity(_flickDistance, timeChange);
		
			dispatch(LAB.TouchEvent.FLICK);
		}
		
		// always dispatch release whether or not release happened over target object
		dispatch(LAB.TouchEvent.RELEASE);
		// or only fire release event if release occured over target?
		// hit test for top most element		
		//if (_target == window || _target === document.elementFromPoint(_touchX, _touchY)) {								
			//dispatch(LAB.TouchEvent.RELEASE);
		//}
		
		resetForNext();	
	}
	
	/**
	 * @private
	 */
	function calcDistance(pointA, pointB) {
		var dist = Math.sqrt(Math.pow((pointB.x - pointA.x), 2) + Math.pow((pointB.y - pointA.y), 2));
		
		return dist;
	}
	
	/**
	 * @private
	 */
	function calcVelocity(dist, timeChange) {
		if (dist > 0 && timeChange > 0) {
			return dist / timeChange
		} else {
			return -1;
		}
	}
	
	/**
	 * @private
	 */
	function dispatch(type) {
		_self.dispatchEvent(new LAB.TouchEvent(type));
		
		// any way or reason to get something like this to work?
		// seems tricky for window and DOM events, need to use
		// customEvent or something like that, but it does not
		// seem to support additional parameters
		//_target.dispatchEvent(new LAB.TouchEvent(type, _self));
	}
	
	/**
	 * @private
	 */
	function resetForNext() {		
		_isDrag = false;
		_isFlick = false;
		
		// clear the array
		_averageVelocity = [];		
	}
	
	/**
	 * @private
	 */
	function calcDegree(startPoint, endPoint) {
		var diffX = startPoint.x - endPoint.x;
		var diffY = endPoint.y - startPoint.y;
		
		return tangentAngle(diffX, diffY);
	}
	
	/**
	 * @private
	 */
	function tangentAngle(xVal, yVal) {
		var radians = Math.atan2(yVal, xVal);
		
		// handle going all the way around
		if (radians < 0) radians += 6.283185;
		
		// convert radians to degrees
		return radians * 57.29578;
	}
	
		
	/**
	 * Register an object (DOM, window, or any object that implements EventDispatcher) with
	 * the touch gesture handler.
	 *
	 * @param {Object} target The object to register with the gesture handler
	 */
	this.register = function(target) {
		_target = target;
		_target.addEventListener("mousedown", mouseDownHandler);
	}
	
	/**
	 * Unregister an object with the touch gesture handler
	 *
	 * @param {Object} target The object to unregister
	 */
	this.unregister = function(target) {
		if (_target !== null) {
			_target.removeEventListener("mousedown", mouseDownHandler);
			_target.removeEventListener("mouseout", mouseOutHandler);
			_target.removeEventListener("mousemove", mouseMoveHandler);
			window.removeEventListener("mouseup", mouseUpHandler);
		}
	}
	
	/**
	 * @return {Number} The direction (in degrees 0 - 360) between the start and stop point 
	 * of the last drag event.
	 */
	this.getDragDirection = function() {
		return _dragDirection;
	}
	
	/**
	 * @return {Number} The distance between the start and stop point of the last drag event.
	 */
	this.getDragDistance = function() {
		return _dragDistance;
	}
	
	/**
	 * @return {Number} The direction (in degrees 0 - 360) between the start and stop point 
	 * of the flick
	 */
	this.getFlickDirection = function() {
		return _flickDirection;
	}
	
	/**
	 * @return {Number} The distance between the start and end point of the flick.
	 */
	this.getFlickDistance = function() {
		return _flickDistance;
	}	
	
	/**
	 * @return {Number} The x coordinate of the touch event.
	 */
	this.getTouchX = function() {
		return _touchX;
	}
	
	/**
	 * @return {Number} The y coordinate of the touch event.
	 */
	this.getTouchY = function() {
		return _touchY;
	}
	
	/**
	 * @return {Number} The average velocity (in pixels/milliseconds) of the drag 
	 * from press to release.
	 */
	this.getAverageVelocity = function() {
		return _averageVelocity;
	}
	
	/**
	 * @return {Number} The velocity (in pixels/milliseconds) of the flick.
	 */
	this.getFlickVelocity = function() {
		return _flickVelocity;
	}	
	
}

LAB.TouchGestureHandler.prototype = LAB.inherit(LAB.EventDispatcher.prototype);
LAB.TouchGestureHandler.prototype.constructor = LAB.TouchGestureHandler;


/**
 * @namespace LAB
 * @constructor
 * @augments LAB.Event
 * @param {String} type The event type
 */
LAB.TouchEvent = function(type) {
	"use strict";
	
	LAB.Event.call(this, type);
}

// events
/** @constant */
LAB.TouchEvent.TAP = "touchtap";
/** @constant */
LAB.TouchEvent.FLICK = "touchflick";
/** @constant */
LAB.TouchEvent.DRAG = "touchdrag";
/** @constant */
LAB.TouchEvent.RELEASE = "touchrelease";
/** @constant */
LAB.TouchEvent.OUT = "touchout";
/** @constant */
LAB.TouchEvent.PRESS = "touchPress";

LAB.TouchEvent.prototype = LAB.inherit(LAB.Event.parent);
LAB.TouchEvent.prototype.constructor = LAB.TouchEvent;