/********************************************************
	TSPS WebGL Example
	* this example uses a THREE.JS (https://github.com/mrdoob/three.js) for rendering
	  and the LAB Javascript Framework (https://github.com/labatrockwell/LAB_JS_Framework)
	  for app logic.
	* what it should do: draw lines based on the position of all current TSPS people.
	  these lines will gradually fade into the distance
	* troubleshooting / questions: 
	  1. My app is just black!
	    - is your websocket server on? make sure the box is checked in the Communication panel in TSPS
		- is the port correct? check the number under "Send via Websockets" in the TSPS Communcation
		  panel. it should be the same number as where you call this.connect("NUMBER") below.
		- does your browser support WebGL? if not, this will never work. check the Javascript panel
		  in your browser to look at the errors
		- something else? submit an issue on GitHub or email feedback@tsps.com
********************************************************/

var demoApp;

$(document).ready( function() {
	// our app uses the THREE.JS base app
	DemoApp.prototype = $.extend(true, LAB.app.ThreeApp.prototype, DemoApp.prototype);
	demoApp = new DemoApp();
	demoApp.begin();
});

// ===========================================
// ===== DEMO APP
// ===========================================
DemoApp = function() {
	// call the constructors of our parent objects
	LAB.app.ThreeApp.call( this );	
   
	var lines	 = {}; 		// hash of lines
	var colors	 = {}; 		// hash of colors
	var maxIndices = 500;	// how complex are our lines?

	// ===========================================
	// ===== TSPS
	// ===========================================	

	var TSPSConnection;

	// ===========================================
	// ===== SETUP
	// ===========================================	
	
	this.setup = function() {
		// CONNECT TO TSPS WEBSOCKET SERVER
		TSPSConnection = new TSPS.Connection();

		// IF YOU'VE CHANGED THE PORT:
		// var TSPSConnection = new TSPS.Connection( "localhost", yourport )

		// IF YOU'RE CONNECTING TO ANOTHER MACHINE:
		// var TSPSConnection = new TSPS.Connection( their IP, their TSPS port )

		TSPSConnection.connect(); 

		// add listeners
		TSPSConnection.onPersonEntered 	= this.onPersonEntered.bind(this);
		TSPSConnection.onPersonLeft 	= this.onPersonLeft.bind(this);
		
		// so stuff fades out in the distance
		//this.scene.fog = new THREE.FogExp2( 0x000000, 0.0008 );

		this.camera.position.x = window.innerWidth/2;
		this.camera.position.y = window.innerHeight/2;
		this.camera.position.z = 1000;
	}
	
	// ===========================================
	// ===== UPDATE
	// ===========================================
	
	this.update = function() {	
		// first, push all of the lines back into z-space
		for (var id in lines){
			var line = lines[id];
			for (var i=line.geometry.vertices.length-1; i>0; i--){
				line.geometry.vertices[ i ].position.z -= 1;
				line.geometry.__dirtyVertices = true; // THREE uses this to update the shape
				line.geometry.__dirtyColors = true;
				
				if (line.geometry.vertices[ i ].position.z < -2000){
					// this one is pretty far back,let's delete it
					this.scene.remove(line);
					delete line;
					break;
				}
			}
		}

		// now loop through all the current people
		for (var id in TSPSConnection.people){	
			var person  = TSPSConnection.people[id];
			var line 	= lines[id] || this.newLine(id);
			var c 		= colors[person.id];
			
			// the person data is 0-1, so we multiply it by the window dimensions
			var point = new THREE.Vector3(window.innerWidth-person.centroid.x*window.innerWidth,window.innerHeight-person.centroid.y*window.innerHeight,person.depth);
			
			line.geometry.vertices[0] = new THREE.Vertex( point ) ;
			line.geometry.colors [0]  = new THREE.Color(c.hex);
			
			// shift the new positions backward
			for (var i=line.geometry.vertices.length-1; i>0; i--){
				if (line.geometry.vertices[i-1].x != Number.POSITIVE_INFINITY){
					line.geometry.vertices[ i ] = line.geometry.vertices[ i-1 ];
					line.geometry.colors[ i ] = line.geometry.colors[ i-1 ];
				}
			}
			line.geometry.__dirtyVertices = true;
			line.geometry.__dirtyColors = true;
		}
	}
	
	// ===========================================
	// ===== DRAW
	// ===========================================
	this.draw = function() {
      	gl.clearColor( 0,0,0, 1 );
      	gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );
		this.renderer.render( this.scene, this.camera );
	}
	
	// make a new THREE line and a new color
	this.newLine = function (id)
	{
		//setup colors
	
		colors[id] = {};
		colors[id].h = Math.random();
		colors[id].s = Math.random();
		colors[id].v = Math.random();
		colors[id].hex = Math.random()*0xffffff;
		
		//setup lines
		var lineGeom = new THREE.Geometry();
		lineGeom.dynamic = true;
		
		var material = new THREE.LineBasicMaterial( { color: 0xffffff, opacity: 1, linewidth: Math.floor(LAB.random(1,5)) });
		material.vertexColors = true;
		
		for (var i=0; i<maxIndices; i++){
			//hidden
			lineGeom.vertices.push(new THREE.Vertex( new THREE.Vector3(Number.POSITIVE_INFINITY, Number.POSITIVE_INFINITY, Number.POSITIVE_INFINITY)) );
			lineGeom.colors.push(new THREE.Color(Math.random()*0xffffff));
		}
		
		lines[id] = new THREE.Line( lineGeom, material );
		lines[id].dynamic = true;
		lines[id].index	  = 0;
		this.scene.add(lines[id]); // TO-DO! this syntax is deprecated in new versions of THREE!
		return lines[id];
	}
	
	// ===========================================
	// ===== TSPS
	// ===========================================
	
	this.onPersonEntered  	= function(person){
		colors[person.id] = {};
		colors[person.id].h = LAB.random(0,255);
		colors[person.id].s = LAB.random(0,255);
		colors[person.id].v = LAB.random(0,255);
		this.newLine(person.id);
	};
	this.onPersonLeft  		= function(person){
		// you can delete stuff here if you want!
		//this.scene.removeObject(lines[person.id]);
		//delete lines[person.id];
		//delete lines[person.id];
	};
	
	// ===========================================
	// ===== ETC
	// ===========================================
	this.onWindowResized = function( width, height ) {};
	this.onMouseMoved = function( x, y ) {};
	this.onMousePressed	= function( x, y ) {};
}