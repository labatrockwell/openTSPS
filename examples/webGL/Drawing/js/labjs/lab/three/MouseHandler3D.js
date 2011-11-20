LAB.namespace("LAB.three.MouseHandler3D");

/**
 * @namespace LAB.three
 * @constructor
 * @augments LAB.EventDispatcher
 * @param {Object} scene A reference to the scene containing objects to register mouse events with
 * @param {Object} camera A reference to the camera associated with the objects to register mouse events with
 */
LAB.three.MouseHandler3D = function(scene, camera) {
	"use strict";
	
	LAB.EventDispatcher.call(this, this);
	
	var scene = scene;
	var camera = camera;
	var self = this;

	var hash = {};
	
	var projector;
	
	projector = new THREE.Projector();
		
	
	/**
	 * Only add a listener when at least one 3d object is registered for the event type
	 *
	 * @private
	 */
	function addMouseEventListener(type) {
	
		// map 3d touch events to 2d touch events
		var eventMap = {
						"mouse3DDown": "mousedown",
						"mouse3DUp": "mouseup",
						"mouse3DClick": "click",
						"mouse3DMove": "mousemove",
						"mouse3DOver": "mouseover",
						"mouse3DOut": "mouseout"
						};
		
		switch (type) {
			case LAB.three.MouseEvent3D.DOWN:
				document.addEventListener( 'mousedown', onDownEvent, false);
				break;
			case LAB.three.MouseEvent3D.UP:
				document.addEventListener( 'mouseup', onUpEvent, false);
				break;
			case LAB.three.MouseEvent3D.CLICK:
				document.addEventListener( 'click', onClickEvent, false);
				break;
			case LAB.three.MouseEvent3D.MOVE:
				document.addEventListener( 'mousemove', onMoveEvent, false);
				break;
			case LAB.three.MouseEvent3D.OVER:
				document.addEventListener( 'mouseover', onOverEvent, false);
				break;
			case LAB.three.MouseEvent3D.OUT:
				document.addEventListener( 'mouseout', onOutEvent, false);
				break;
		}

	}	
	
	// this could be useful as a public method as well
	/**
	 * @private
	 */
	function hitTest(event) {
		//console.log(event);
		var vec,
			ray,
			intersects;
	
		vec = new THREE.Vector3( (event.clientX / window.innerWidth ) * 2 - 1, - (event.clientY / window.innerHeight ) * 2 + 1, 0.5 );
		projector.unprojectVector( vec, camera );
		
		ray = new THREE.Ray( camera.position, vec.subSelf( camera.position ).normalize() );
		
		intersects = ray.intersectScene( scene );
				
		return intersects;	
	}
	
	/**
	 * @privte
	 */
	function handleEvent(type, evt) {
		var i,
			len,
			intersects,
			hitObject,
			events,
			params;

		intersects = hitTest(evt);
		len = intersects.length;
		
		// if the hit test returned any objects
		if (len > 0) {

			// iterate through array of intersected objects
			for (i = 0; i < len; i++) {
			
				hitObject = intersects[i].object;
				
				// this could be useful when dealing with dense scenes in determining which
				// is the closest object
				// may also be useful to implement event bubbling
				//console.log("distance = " + intersects[i].distance);
				
				// make sure the object is registered with the event handler
				if (hash[hitObject.id]) {
					// make sure the objects match (this is an extra precaution and may not
					// be necessary)
					if (hitObject === hash[hitObject.id].object) {
						// if this event type is registered for an object, fire the corresponding
						// event, passing a reference to the object					
						events = hash[hitObject.id].events;
						if (events != undefined && events.indexOf(type) > -1) {
							params = {object: hitObject, data: evt};
							self.dispatchEvent(new LAB.three.MouseEvent3D(type), params);
						}
					} else {
						console.log("no match");
					}
				}
				
			}
		}	
	}
	
	/**
	 * @private
	 */
	function onMoveEvent(evt) {
		handleEvent(LAB.three.MouseEvent3D.MOVE, evt);
	}
	
	/**
	 * @private
	 */
	function onDownEvent(evt) {
		handleEvent(LAB.three.MouseEvent3D.DOWN, evt);
	}
	
	/**
	 * @private
	 */
	function onUpEvent(evt) {	
		// to do: make sure up target matches down target?
		handleEvent(LAB.three.MouseEvent3D.UP, evt);
	}
	
	/**
	 * @private
	 */
	function onClickEvent(evt) {
		handleEvent(LAB.three.MouseEvent3D.CLICK, evt);
	}
	
	/**
	 * @private
	 */
	function onOverEvent(evt) {
		handleEvent(LAB.three.MouseEvent3D.OVER, evt);
	}
	
	/**
	 * @private
	 */
	function onOutEvent(evt) {
		handleEvent(LAB.three.MouseEvent3D.OUT, evt);
	}	
	
	/**
	 * Register mouse or touch events with a 3D object (must inherit from THREE.Mesh).
	 * You can also use this method to register additional event types for a specific object.
	 *
	 * @param {Object} Object3D A 3D object to register events for.
	 * @param {String[]} events An array of event types (TouchEvent3D) to register.
	 */
	this.register = function(Object3D, events /* {String[]} */) {
		
		// To Do: make sure the object is a THREE.Mesh. If not, throw error
		
		var len = events.length;
		
		for (var j=0; j<len; j++) {
			// only start add event listeners as we need them
			addMouseEventListener(events[j]);
		}
		
		// To Do: only add listeners on this class for events that are registered
		// so if not 3D object listens for mouse move, then don't register mouse move?
		
		var registeredEvents,
			index;
		
		// this will only work if the object clicked on only has one unique mesh id
		if (!hash[Object3D.id]) {
			hash[Object3D.id] = {object: Object3D, events: events};
			//console.log(hash);
		} else {
			//if the object is already registered, append any new events
			registeredEvents = hash[Object3D.id].events;
			// iterate through events array
			for (var i=0; i<len; i++) {
				// if the event is not registered, add it
				if (registeredEvents.indexOf(events[i] == -1)) {
					registeredEvents.push(events[i]);
				}
			}
		}
	}
	
	
	/**
	 * Unregister mouse or touch events with a 3D object (must inherit from THREE.Mesh).
	 * Passing the object only with no events will unregister all events. Alternatively
	 * you can pass an arrayof one or more specific event types to unregister.
	 *
	 * @param {Object} Object3D A 3D object to unregister events for.
	 * @param {String[]} events An array of event types (TouchEvent3D) to unregister.
	 */	
	this.unregister = function(Object3D, events /* {String[]} */) {
	
		// To Do: make sure the object is a THREE.Mesh. If not, throw error 
	
		var events = events || null;
		var obj,
			index;
		
		// ensure the object is registered
		if (hash[Object3D.id]) {
			// if specific events are not unregistered, then unregister the object (delete key)
			if (events == null) {
				delete hash[Object3D.id];
			} else {
				// unregister individual events
				obj = hash[Object3D.id];
				//remove specified event type(s) from array
				for (var i=0, len=events.length; i<len; i++) {
					index = obj.events.indexOf(events[i]);
					if (index > -1) {
						obj.events.splice(index, 1);
					}
				}
			}
		}
		
	}
	
}

LAB.three.MouseHandler3D.prototype = LAB.inherit(LAB.EventDispatcher.prototype);
LAB.three.MouseHandler3D.prototype.constructor = LAB.three.MouseHandler3D;

LAB.namespace("LAB.three.MouseEvent3D");

/**
 * @namespace LAB.three
 * @constructor
 * @augments LAB.Event
 * @param {String} type event type
 */
LAB.three.MouseEvent3D = function(type) {
	"use strict";
	
	LAB.Event.call(this, type);
}

/** @constant */
LAB.three.MouseEvent3D.DOWN = "mouse3DDown";
/** @constant */
LAB.three.MouseEvent3D.UP = "mouse3DUp";
/** @constant */
LAB.three.MouseEvent3D.MOVE = "mouse3DMove";
/** @constant */
LAB.three.MouseEvent3D.OVER = "mouse3DOver";
/** @constant */
LAB.three.MouseEvent3D.OUT = "mouse3DOut";
/** @constant */
LAB.three.MouseEvent3D.CLICK = "mouse3DClick";


LAB.three.MouseEvent3D.prototype = LAB.inherit(LAB.Event.prototype);
LAB.three.MouseEvent3D.prototype.constructor = LAB.three.MouseEvent3D;
