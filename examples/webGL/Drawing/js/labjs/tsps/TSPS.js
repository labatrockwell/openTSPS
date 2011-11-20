/** @namespace TSPS */
var TSPS = TSPS || {};

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
TSPS.Dispatcher.prototype.newPerson		= function(json)
{
	if (json.type == "TSPS/personEntered") this._onPersonEntered(json);
	else if (json.type == "TSPS/personMoved") this._onPersonUpdated(json);
	else if (json.type == "TSPS/personUpdated") this._onPersonMoved(json);
	else if (json.type == "TSPS/personWillLeave") this._onPersonLeft(json);
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
	this.people[tspsObj.id] = tspsObj;
	this.onPersonUpdated(tspsObj);
}

/**
@function
@private
*/
TSPS.Dispatcher.prototype._onPersonMoved	= function(tspsObj){
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

/** 
	Creates a new TSPS person
	TO-DO: add template of all the properties this should have
	@constructor
*/

TSPS.person = function()
{
};