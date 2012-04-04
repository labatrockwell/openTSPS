// setup global vars

var TSPSConnection;

var coolDiv;
var coolDivs = {};

$(document).ready( function() {

	coolDiv = $("#coolDiv");
	coolDiv.remove(); //we're just going to use our cool div as a template

	// SETUP TSPS Connection 
	TSPSConnection = new TSPS.Connection();

	// IF YOU'VE CHANGED THE PORT:
	// TSPSConnection = new TSPS.Connection( "localhost", yourport )

	// IF YOU'RE CONNECTING TO ANOTHER MACHINE:
	// TSPSConnection = new TSPS.Connection( their IP, their TSPS port )

	TSPSConnection.connect(); 

	// add listeners
	TSPSConnection.onPersonEntered 	= onPersonEntered;
	TSPSConnection.onPersonMoved	= onPersonMoved;
	TSPSConnection.onPersonUpdated	= onPersonUpdated;
	TSPSConnection.onPersonLeft 	= onPersonLeft;
});

function updateDiv( div, person ){
	console.log(person)
	// we have to multiply by the window size here because our centroid is 0-1
	div.css("left", Math.round(person.boundingrect.x*window.innerWidth) );
	div.css("top", Math.round(person.boundingrect.y*window.innerHeight) );
	div.css("width", Math.round(person.boundingrect.width*window.innerWidth));
	div.css("height", Math.round(person.boundingrect.height*window.innerHeight));
}

function onPersonEntered( person ){
	// clone template + add it to body
	coolDivs[person.id] = coolDiv.clone();
	$(document.body).append(coolDivs[person.id]);
	updateDiv( coolDivs[person.id], person );
}

function onPersonUpdated( person ){
	updateDiv( coolDivs[person.id], person );
}

function onPersonMoved( person ){
	updateDiv( coolDivs[person.id], person );
}

function onPersonLeft( person ){
	coolDivs[person.id].remove();
}