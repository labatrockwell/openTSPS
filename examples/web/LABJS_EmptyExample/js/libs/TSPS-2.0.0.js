// Verson 1.2
/** @namespace TSPS */
var TSPS = TSPS || {};
TSPS.LogLevel = TSPS.LOG_ERROR;

/********************************************
	UTILS
********************************************/

	TSPS.LOG_DEBUG 		 = 0;
	TSPS.LOG_WARNING	 = 1;
	TSPS.LOG_ERROR 		 = 2;
	TSPS.LOG_FATAL_ERROR = 3;

	TSPS.Log = function( message, level ){
		var _level = ( level == null ? TSPS.LOG_DEBUG : level );
		if ( _level >= TSPS.LogLevel ){
			if ( console ){
				switch ( _level ){
					case TSPS.LOG_DEBUG:
						console.log( message );
						break;
					case TSPS.LOG_WARNING:
						console.warn( message );
						break;
					case TSPS.LOG_ERROR:
					case TSPS.LOG_FATAL_ERROR:
						console.error( message );
						break;
				}
			}
		}
	}

/********************************************
	DISPATCHER
********************************************/

/** 
	Creates a new TSPS parser + dispatcher
	@constructor
*/

TSPS.Dispatcher = function(){
	
	/**
		people
		@type Object
		Object with all current people sorted by ID
	*/
	this.people			= {};

	/**
	 * TSPS Scene object
	 * @type {TSPS.Scene}
	 */
	this.scene 			= new TSPS.Scene();
}
/**
@function
Pass a new JSON object into here to add it into TSPS
*/
TSPS.Dispatcher.prototype.onTSPSData		= function(json)
{
	if (json.type == "personEntered") this._onPersonEntered(json);
	else if (json.type == "personMoved") this._onPersonMoved(json);
	else if (json.type == "personUpdated") this._onPersonUpdated(json);
	else if (json.type == "personWillLeave") this._onPersonLeft(json);
	else if (json.type == "customEvent") this._onCustomEvent(json);
	else if (json.type == "scene") this._onSceneUpdate(json);
	else TSPS.Log("unknown type? "+json.type);
}

/**
@function
@private
*/
TSPS.Dispatcher.prototype._onPersonEntered	= function(tspsObj){
	// prevent this getting called more than once
	if ( !this.people[tspsObj.id]){
		this.people[tspsObj.id] = new TSPS.Person();
		this.people[tspsObj.id].update( tspsObj );
		this.onPersonEntered(this.people[tspsObj.id]);		
	}
}

/**
@function
@private
*/
TSPS.Dispatcher.prototype._onPersonUpdated	= function(tspsObj){
	if (!this.people[tspsObj.id]){
		this._onPersonEntered(tspsObj);
		return;
	} 
	this.people[tspsObj.id].update( tspsObj );
	this.onPersonUpdated(this.people[tspsObj.id]);
}

/**
@function
@private
*/
TSPS.Dispatcher.prototype._onPersonMoved	= function(tspsObj){
	if (!this.people[tspsObj.id]){
		this._onPersonEntered(tspsObj);
		return;
	} 
	this.people[tspsObj.id].update( tspsObj );
	this.onPersonMoved(this.people[tspsObj.id]);
}

/**
@function
@private
*/
TSPS.Dispatcher.prototype._onPersonLeft 	= function(tspsObj){
	if (!this.people[tspsObj.id]){
		TSPS.Log( "got weird id "+tspsObj.id);
		this.onPersonLeft( tspsObj );	
		return;
	} 
	this.people[tspsObj.id].update( tspsObj );
	this.onPersonLeft(this.people[tspsObj.id]);		
	delete this.people[tspsObj.id];
}

/**
 * @private
 */
TSPS.Dispatcher.prototype._onSceneUpdate 	= function(tspsScene){
	this.scene.update( tspsScene );
	this.onSceneUpdate( this.scene );
}

/**
 * @private
 */
TSPS.Dispatcher.prototype._onCustomEvent	= function(eventObj){
	this.onCustomEvent( eventObj.name, eventObj.data )
}

/**
override in your main app
@function
@example
var tsps = new TSPS.Dispatcher();
tsps.onPersonEntered = this.onPersonEntered;
tsps.onPersonUpdated = this.onPersonUpdated;
tsps.onPersonMoved = this.onPersonMoved;
tsps.onPersonLeft = this.onPersonLeft;	
*/
TSPS.Dispatcher.prototype.onPersonEntered  = function(person){};
/**
override in your main app
@function
*/
TSPS.Dispatcher.prototype.onPersonUpdated  = function(person){};
/**
override in your main app
@function
*/
TSPS.Dispatcher.prototype.onPersonMoved  = function(person){};

/**
override in your main app
@function
*/
TSPS.Dispatcher.prototype.onPersonLeft  = function(person){};

/**
override in your main app
@function
*/
TSPS.Dispatcher.prototype.onSceneUpdate  = function(scene){};

/**
override in your main app
@function
*/
TSPS.Dispatcher.prototype.onCustomEvent  = function(name, data){};

/********************************************
	PERSON
********************************************/

/** 
	Creates a new TSPS person
	@constructor
	@property {integer} id persistent id from frame to frame
	@property {integer} age how many frames has this person been in the system
	@property {object} 	boundingrect {x,y,width,height} enclosing area 
	@property {object} 	centroid center of mass of the person
	@property {array} 	contours array of points {x,y} of contour shape
	@property {number} 	depth raw depth from kinect
	@property {object} 	haarrect bounding rectangle of detected feature
	@property {object} 	opticalflow total optical flow in blob
	@property {object} 	velocity most recent movement of centroid
	@property {highest} highest point in a blob (brightest pixel, will really only work correctly with kinect)
*/               

TSPS.Person = function(){
	this.id 			= -1;
	this.age 			= 0;
	this.boundingrect 	= {"x":0, "y":0, "width":0, "height":0};
	this.height 	  	= 0;
	this.width 		 	= 0;
	this.x 				= 0;
	this.y 				= 0;
	this.centroid 		= {"x":0, "y":0};
	this.contours 		= [];
	this.depth 			= 0;
	this.haarrect 		= {"x":0, "y":0, "width":0, "height":0};
	this.opticalflow 	= {"x":0, "y":0};
	this.velocity		= {"x":0, "y":0};
	this.highest		= {"x":0, "y":0};

	this.update = function( person ){
		for ( var prop in person ){
			this[prop] = person[prop];			
		}
	}
};

/********************************************
	Scene
********************************************/

/**
 * TSPS Scene Object
 * @property {Number} numPeople 		Number of people in scene
 * @property {Number} percentCovered 	% of screen covered by People
 * @property {Object} averageMotion  	Average motion of all People in Scene
 * @property {Number} gridX 			X dimensions of true/false grid
 * @property {Number} gridY 			Y dimensions of true/false grid
 * @property {Array}  grid  			Array of booleans of gridX * gridY length (whether a Person is in quad)                         
 */
TSPS.Scene = function(){
	this.numPeople 			= 0;
	this.percentCovered		= 0;
	this.averageMotion		= {x:0,y:0};
	this.gridX				= 0;
	this.gridY				= 0;
	this.grid 				= [];

	/**
	 * Update from Scene object
	 * @param  {Object} scene
	 */
	this.update = function( scene ){
		for ( var prop in scene ){
			if ( prop == "grid" ){
				this.grid = [];
				for ( var i=0; i<scene[prop].length; i++){
					if (parseInt(scene[prop]) == 1){
						this.grid.push(true);
					} else {
						this.grid.push(false);
					}
				}
			} else {
				this[prop] = person[prop];		
			}	
		}
	}

	/**
	 * Check if certain square is filled
	 * @param  {Number}  x X coord of grid to check (0 to gridX-1)
	 * @param  {Number}  y Y coord of grid to check (0 to gridY-1)
	 * @return {Boolean}   Is anyone there?
	 */
	this.isSquareActive = function( x, y ){
		if ( x >= this.gridX || y >= this.gridY ){
			TSPS.log( "Coordinates out of grid range", TSPS.LOG_ERROR);
			return false;
		}
		return this.grid[ x + y * this.gridX];
	}
}

/********************************************
	COMPATIBILITY
********************************************/

// add bind method for browsers that don't currently support it (such as Safari)
if (!Function.prototype.bind) {  
  Function.prototype.bind = function (oThis) {  
	if (typeof this !== "function") {  
	  // closest thing possible to the ECMAScript 5 internal IsCallable function  
	  throw new TypeError("Function.prototype.bind - what is trying to be bound is not callable");  
	} 
  
	var aArgs = Array.prototype.slice.call(arguments, 1),   
		fToBind = this,   
		fNOP = function () {},  
		fBound = function () {  
		  return fToBind.apply(this instanceof fNOP  
								 ? this  
								 : oThis || window,  
							   aArgs.concat(Array.prototype.slice.call(arguments)));  
		};  
  
	fNOP.prototype = this.prototype;  
	fBound.prototype = new fNOP();  
  
	return fBound;  
  };  
} 

/********************************************
	CONNECTION
********************************************/

/** 
	TSPS Connection: Simple Websocket Wrapper
	@constructor
*/

TSPS.Connection = function( host, port, protocol ){
	TSPS.Dispatcher.call( this );			// extend dispatcher
	this.host 		= host || "localhost";	// host: localhost, host name, or IP address
	this.port 		= port || 7681; 		// default TSPS port
	this.socket = null;
}

TSPS.Connection.prototype = new TSPS.Dispatcher();
TSPS.Connection.prototype.constructor = TSPS.Connection;

/**
@function
*/
TSPS.Connection.prototype.connect = function(){
	try {
		this.socket = new WebSocket( "ws://" + this.host+":"+this.port );
		this.socket.onmessage = this._onMessageReceived.bind(this);
		this.socket.onopen = this._onConnectionOpened.bind(this);
		this.socket.onclose = this._onConnectionClosed.bind(this);
	} catch( ex ) {
		console.warn( ex );
	}
}

/**
@function
*/
TSPS.Connection.prototype.send = function( data ) {
	this.socket.send( data );
}

TSPS.Connection.prototype._onConnectionOpened = function() { this.onConnectionOpened(); }

/**
	@function
*/
TSPS.Connection.prototype.onConnectionOpened = function() {};

TSPS.Connection.prototype._onConnectionClosed = function() { this.onConnectionClosed(); }

/**
@function
*/
TSPS.Connection.prototype.onConnectionClosed = function() {};


TSPS.Connection.prototype._onMessageReceived = function( evt ) {
	var data = evt.data;
	try {
		var data =  jQuery.parseJSON( evt.data );	
	} catch(e) {
		// not valid JSON? Other reasons for error?
	}
	this.onMessageReceived( data );		
	var TSPSPeople = this.onTSPSData(data); //call dispatcher function
}

/**
Override in your main app to catch the raw TSPS data
@function
*/
TSPS.Connection.prototype.onMessageReceived = function( data ) {
}

