/** @namespace LAB.agents */
LAB.agents = LAB.agents || {};

/** @constructor */
LAB.agents.Agent = function() {

};

LAB.agents.Agent.prototype.setup = function(_id, _x, _y, _z, _rotation, _sWidth, _sHeight, _isVisible) {
	this.pos = [];
	this.myID = _id || 0;
	this.groupSize;
	this.rotation = _rotation || 0;
	this.currentWidth = _sWidth || 0;
	this.currentHeight = _sHeight || 0;
	this.isVisible = _isVisible || true;
	this.startWidth;
	this.startHeight;
	this.xScale = 1;
	this.yScale = 1;
	this.color;
	this.pos['x'] = _x || 0; // = 0; //100;
	this.pos['y'] = _y || 0; // = 0; //100;
	this.pos['z'] = _z || 0; // = 0; //100;
	this.Behaviors = [];
};

LAB.agents.Agent.prototype.addBehavior = function(Behavior) {
	this.Behaviors.push(Behavior);
	Behavior.setup(this);
	console.log(this.Behaviors);
};

LAB.agents.Agent.prototype.removeBehavior = function(Behavior) {
	// this.Behaviors.push(Behavior);
	// Behavior.setup(this);
	// console.log(this.Behaviors);
};

LAB.agents.Agent.prototype.applyBehaviors = function() {
	var i=0;
	for (i=0; i<this.Behaviors.length; i++) {
		this.Behaviors[i].apply(this);
	}
}

LAB.agents.Agent.prototype.checkIt = function() {
	console.log("check!");
};