/** @namespace LAB.sound*/
LAB.sound = LAB.sound || {};

LAB.require(LAB.src+"geom/Point.js");

// creates global audio context if there isn't one yet
LAB.require(LAB.src+"sound/AudioContext.js");

/** 
 Web Audio Player
 Web Audio must be enabled in Chrome via about:flags!
 @constructor 
 */
LAB.sound.WAPlayer = function( ){//loop, autoplay, controls ){		
	LAB.EventDispatcher.call( this, this );
	
	this.buffer = null;
	this.pan	= new LAB.geom.Point();
	this.pitch	= 0;
	this.gain	= 0;
	
	this.filter = null;
	
	this.currentSource 	= null;
	this.currentTime	= 0;
	
	this.panner			= LAB.sound.Context.createPanner();
	this.panner.setPosition(0,0,0);
	
	this.start 	= 0;
	this.end	= 0;
}

// METHODS

LAB.sound.WAPlayer.prototype.loadFile 		= function( url )
{	
	//this.registerEvents();
	var request = new XMLHttpRequest();
	request.parent = this;
    request.open("GET", url, false);
    request.responseType = "arraybuffer";
	request.parent = this;
	
    request.onload = function() { 
        request.parent.buffer = LAB.sound.Context.createBuffer(request.response, true);
		request.parent.end = Math.floor(request.parent.buffer.duration);
    }

    request.send();	
}

LAB.sound.WAPlayer.prototype.setFilter	= function( filter )
{
	this.filter = filter;
}

// PLAYBACK METHODS

LAB.sound.WAPlayer.prototype.play	= function(){
	//this.source.noteOff(0);
	var source 					= LAB.sound.Context.createBufferSource();	
	source.playbackRate.value 	= this.pitch;
	source.gain					= this.gain;
	source.buffer 				= this.buffer;
	
	if (this.filter != null){
		source.connect(this.filter);
		this.filter.connect(this.panner);
	}  else {
		source.connect(this.panner);
	}
	
	this.panner.connect(LAB.sound.Context.destination);
	source.noteGrainOn(0, this.start, this.end);
}

LAB.sound.WAPlayer.prototype.setGain	= function(gain){
	this.gain = gain;
}

LAB.sound.WAPlayer.prototype.setPan	= function(x, y, z){
	this.pan.x = x;
	this.pan.y = y || this.pan.y;
	this.pan.z = z || this.pan.z;
	this.panner.setPosition(this.pan.x, this.pan.y, this.pan.z);
}

LAB.sound.WAPlayer.prototype.setPitch	= function(pitch){
	this.pitch = Math.pow(2.0, pitch / 1200.0);
}