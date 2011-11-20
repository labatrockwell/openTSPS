/** @namespace LAB.agents */
LAB.agents = LAB.agents || {};

/** @constructor */
LAB.agents.Group = function() {
	this.Agents = [];
}

LAB.agents.Group.prototype.addToGroup = function(Agent) {
	// console.log("Added to Group" + Agent);
	console.log(this.Agents);
	this.Agents.push(Agent);
}

LAB.agents.Group.prototype.update = function() {
	var i = 0;
	for (i=0; i<this.Agents.length; i++) {
		this.Agents[i].update();
		this.Agents[i].applyBehaviors();
	}
}

LAB.agents.Group.prototype.draw = function() {
	var i = 0;
	for (i=0; i<this.Agents.length; i++) {
		this.Agents[i].draw();
	}
}

var thing = $.extend(true, LAB.agents.Group.prototype, LAB.agents.Agent.prototype); // How should we fix this?