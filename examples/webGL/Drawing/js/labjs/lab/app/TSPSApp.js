// include LabBase files
/** @namespace LAB.app */
LAB.require(LAB.src+"../tsps/TSPS.js");
LAB.require(LAB.src+"app/BaseApp.js");
LAB.require(LAB.src+"utils/WebSocket.js");

/** 
	@constructor 
	@extends LAB.app.BaseApp
	@extends TSPS.dispatcher
*/
LAB.app.TSPSApp = function()
{
	TSPS.Dispatcher.call( this );
	LAB.app.BaseApp.call( this );
	
	this.socket 	= null;
}

LAB.app.TSPSApp.prototype = $.extend(true, LAB.app.BaseApp.prototype, TSPS.Dispatcher.prototype, LAB.app.TSPSApp.prototype);

// ===========================================
// ===== METHODS
// ===========================================

/**
Connect to TSPS WebSocket
@function
*/
LAB.app.TSPSApp.prototype.connect = function (port, host)
{
	host = host || "ws://localhost";
	
	this.socket = new LAB.utils.WebSocket( host+":"+port, "tsps-protocol");
	this.socket._onMessageReceived	= this._onMessageReceived.bind(this);
	this.socket.onMessageReceived 	= this.onMessageReceieved;
	this.socket.onConnectionOpened 	= this.onConnectionOpened;
	this.socket.onConnectionClosed 	= this.onConnectionClosed;
	this.socket.connect();
	
	console.log("connecting to "+ host+":"+port);
}

// ===========================================
// ===== WEBSOCKET HANDLERS
// ===========================================

/**
Override default WS functionality
@function
@private
*/

	LAB.app.TSPSApp.prototype._onMessageReceived = function( evt ) {
		this.onMessageReceieved.bind(this)(evt.data );
	}

/**
@function
@private
*/
	LAB.app.TSPSApp.prototype.onMessageReceieved = function( data ){
		var data =  jQuery.parseJSON( data );				
		var TSPSPeople = this.newPerson(data);
	}

/**
@function
@private
*/
	LAB.app.TSPSApp.prototype.onConnectionOpened = function(){
		console.log("connection opened!");
	}

/**
@function
@private
*/
	LAB.app.TSPSApp.prototype.onConnectionClosed = function(){
		console.log("connection closed!");
	}

// ===========================================
// ===== TSPS HANDLERS
// ===========================================

/**
Override in your app that extends TSPSApp
@function
*/
	LAB.app.TSPSApp.prototype.onPersonEntered = function( person ){};
	
/**
Override in your app that extends TSPSApp
@function
*/
	LAB.app.TSPSApp.prototype.onPersonUpdated = function( person ){};

/**
Override in your app that extends TSPSApp
@function
*/
	LAB.app.TSPSApp.prototype.onPersonMoved = function( person ){};
	
/**
Override in your app that extends TSPSApp
@function
*/
	LAB.app.TSPSApp.prototype.onPersonLeft = function( person ){};
	