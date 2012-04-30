TSPS = function(){
	this.people			= {};
}

TSPS.prototype.newPerson		= function(json)
{
	if (json.type == "personEntered") this.onPersonEntered(json);
	else if (json.type == "personMoved") this.onPersonUpdated(json);
	else if (json.type == "personUpdated") this.onPersonMoved(json);
	else if (json.type == "personWillLeave") this.onPersonLeft(json);
	else console.log(json.type);
}

TSPS.prototype.onPersonEntered	= function(tspsObj){
	this.people[tspsObj.id] = tspsObj;
}

TSPS.prototype.onPersonUpdated	= function(tspsObj){
	this.people[tspsObj.id] = tspsObj;
}

TSPS.prototype.onPersonMoved	= function(tspsObj){
	this.people[tspsObj.id] = tspsObj;
}

TSPS.prototype.onPersonLeft 	= function(tspsObj){
	delete this.people[tspsObj.id];
}