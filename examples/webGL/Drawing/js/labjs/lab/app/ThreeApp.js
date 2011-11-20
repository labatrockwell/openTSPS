// include LabBase files
/** @namespace LAB.app */
LAB.require(LAB.src+"../../libs/three/Three.js");
LAB.require(LAB.src+"app/BaseApp.js");

// LAB Three includes

LAB.require(LAB.src+"three/Camera.js");
LAB.require(LAB.src+"three/Geometry.js");
LAB.require(LAB.src+"three/Object.js");
LAB.require(LAB.src+"three/Shader.js");

/**
* global gl reference to mirror normal openGL
* @type WebGLContext
*/
var gl = gl || null;

/** 
	@constructor 
	@extends LAB.app.BaseApp
*/
LAB.app.ThreeApp = function()
{
	LAB.app.BaseApp.call( this );
	
	this.container;
	this.camera, this.scene, this.projector, this.renderer;

	this.mouse = { x: 0, y: 0 };
}

LAB.app.ThreeApp.prototype = new LAB.app.BaseApp();
LAB.app.ThreeApp.prototype.constructor = LAB.app.ThreeApp;
LAB.app.ThreeApp.prototype.supr = LAB.app.BaseApp.prototype;

/************************************************************
	SETUP
************************************************************/

	/** 
		Call to set up Webgl, initialize the canvas, provide default THREE vars
		and start animate() loop
		@function 
		@public
	*/

	LAB.app.ThreeApp.prototype.begin = function()
	{
		// listen to mouse + keys by default
		this.registerKeyEvents();
		this.registerMouseEvents();

		/**
		* default THREE camera
		* @type THREE.Camera
		*/
		console.log("base app set up");
		//this.camera = new THREE.Camera( 35, window.innerWidth / window.innerHeight, .1, 1000 );
		this.camera = new LAB.three.Camera( 35, window.innerWidth / window.innerHeight, .1, 1000 );
        this.camera.setToWindowPerspective();
		
		/**
		* default THREE scene
		* @type THREE.Scene
		*/
		this.scene = new THREE.Scene();
		
		/**
		* default THREE projector
		* @type THREE.Projector
		*/
		this.projector = new THREE.Projector();
		
		/**
		* default THREE renderer with anti-aliasing, depth sorting off
		* @type THREE.WebGLRenderer
		*/
		this.renderer = new THREE.WebGLRenderer( { antialias: true } );
		this.renderer.sortObjects = false;
		this.renderer.setSize( window.innerWidth, window.innerHeight );
      	this.renderer.autoClear = false;

		// do we have a container?
	
		if (document.getElementById("labContainer") != null){
			this.container = document.getElementById("labContainer");
		} else {
			console.log("no labContainer in document, generating container div")
			this.container = document.createElement( 'div' );
			if (document.body)
				document.body.appendChild( this.container );
			else
				return;
		}
		
		this.container.appendChild(this.renderer.domElement);	
		
		gl = gl || this.renderer.getContext();
		
		this.setup();
		this.animate();
	}

/************************************************************
	DRAW: override the draw function in your app!
************************************************************/
