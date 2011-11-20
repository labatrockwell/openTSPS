/** 
 * 
 * @namespace LAB 
 * @constructor
 * param {Class} target The instance of the class that implements the EventDispatcher
 */
LAB.EventDispatcher = function(target)
{
	var _target = target || null;
	this.getTarget = function() {
		return _target;
	}
	this.events = [];
}

LAB.EventDispatcher.prototype = {
	constructor:LAB.EventDispatcher,
	/**
	 * @param {String} event The event type
	 * @param {Function} callback The function to be called when the event is dispatched
	 */
	addEventListener: function(type, callback){
		this.events[type] = this.events[type] || []; // check for undefined arg
		if (this.events[type]){
			this.events[type].push(callback);
		}
	},
	/**
	 * @param {String} event The event type
	 * @param {Function} callback The function to be called when the event is dispatched
	 * return {boolean} True if is successful, false if not.
	 */
	removeEventListener: function(type,callback){
		if ( this.events[type] ) {
			var listeners = this.events[type];
			for ( var i = listeners.length-1; i>=0; --i ){
				if ( listeners[i] === callback ) {
					listeners.splice( i, 1 );
					return true;
				}
			}
		}
		return false
	},
	/**
	 * @param {String} event The event type
	 * return {boolean} True if listener exists for thie event type, false if not
	 */
	hasEventListener:function(type) {
		if (this.events[type]) {
			return true;
		} else {
			return false;
		}
	},
	/**
	 * There are 2 ways to use dispatchEvent. The first is to send an Event object (including
	 * objects that extend the Event class) and any optionl parameters. For example:
	 *
	 * @example
	 * someObject.dispatchEvent(new LAB.Event(Event.CHANGE));
	 * @example
	 * var params = {secret: "my secret", message: "hi"};
	 * someObject.dispatchEvent(new LAB.Event("customEventType"), params);
	 * <p>The second is to pass a string as the event type and any optional params</p>
	 * @example
	 * someObject.dispatchEvent("customEventType");
	 * @example
	 * var params = {secret: "my secret", message: "hi"};
	 * someObject.dispatchEvent("customEventType", params);	 
	 *
	 * @param {string|Event} typeOrEvent The event type as a string or an Event object
	 * @param {Object} optionalParams Optional parameters passed as an object.
	 * return {boolean} True if dispatch is successful, false if not.
	 */
	dispatchEvent:function(typeOrEvent, optionalParams){
		
		if (typeof typeOrEvent === 'string') {
			var event = new LAB.Event(typeOrEvent);
		} else if (typeof typeOrEvent=== 'object') {
			var event = typeOrEvent;
		} else {
			throw "First parameter of dispatchEvent must be of type object or string";
		}
		
		event.target = this.getTarget();
		
		for (var obj in optionalParams) {
			event[obj.toString()] = optionalParams[obj];
		}
		
		if ( this.events[event.type] ) {
			var listeners = this.events[event.type], len = listeners.length;
			while ( len-- ) {
				listeners[len](event);	//callback with self
			}		
			return true; // return success
		} else {
			return false;
		}
	}
}

/** 
 * Event 'base class' (but it can also be instantiated directly)
 *
 * @constructor
 * @param {String} type event type
 */
LAB.Event = function(type) {
	this.type = type;
	this.target = null;
}

/** @constant */
LAB.Event.CONNECTED = "connected";
/** @constant */
LAB.Event.CHANGE	= "change";
/** @constant */
LAB.Event.COMPLETE	= "complete";