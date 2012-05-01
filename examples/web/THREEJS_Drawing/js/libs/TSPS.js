/** @namespace TSPS */
var TSPS = TSPS || {};

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
}
/**
@function
Pass a new JSON object into here to add it into TSPS
*/
TSPS.Dispatcher.prototype.onTSPSData		= function(json)
{
	if (json.type == "personEntered") this._onPersonEntered(json);
	else if (json.type == "personMoved") this._onPersonUpdated(json);
	else if (json.type == "personUpdated") this._onPersonMoved(json);
	else if (json.type == "personWillLeave") this._onPersonLeft(json);
	else console.log(json.type);
}

/**
@function
@private
*/
TSPS.Dispatcher.prototype._onPersonEntered	= function(tspsObj){
	this.people[tspsObj.id] = tspsObj;
	this.onPersonEntered(tspsObj);
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
	this.people[tspsObj.id] = tspsObj;
	this.onPersonUpdated(tspsObj);
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
	this.people[tspsObj.id] = tspsObj;
	this.onPersonMoved(tspsObj);
}

/**
@function
@private
*/
TSPS.Dispatcher.prototype._onPersonLeft 	= function(tspsObj){
	delete this.people[tspsObj.id];
	this.onPersonLeft(tspsObj);		
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

/********************************************
	PERSON
********************************************/

/** 
	Creates a new TSPS person
	@constructor
*/

TSPS.person = function()
{
	this.id 			= -1;
	this.age 			= 0;
	this.boundingrect 	= {"x":0, "y":0, "width":0, "height":0};
	this.centroid 		= {"x":0, "y":0};
	this.contours 		= [];
	this.depth 			= 0;
	this.haarrect 		= {"x":0, "y":0, "width":0, "height":0};
	this.opticalflow 	= {"x":0, "y":0};
	this.velocity		= {"x":0, "y":0};
};

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
	this.protocol 	=  "tsps-protocol"; 	//don't change this unles you change it in your TSPS Source!
	this.socket = null;
}

TSPS.Connection.prototype = new TSPS.Dispatcher();
TSPS.Connection.prototype.constructor = TSPS.Connection;

/**
@function
*/
TSPS.Connection.prototype.connect = function(){
	try {
		this.socket = new WebSocket( "ws://" + this.host+":"+this.port, this.protocol );
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

