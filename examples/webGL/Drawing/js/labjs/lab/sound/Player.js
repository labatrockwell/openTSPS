/** @namespace LAB.sound*/
LAB.sound = LAB.sound || {};

LAB.require(LAB.src+"EventDispatcher.js");

/** 
 @constructor 
 @augments LAB.EventDispatcher
 @param {HTMLElement} parent pass in an element to attach the player to
 @param {Boolean} loop 
 @param {Boolean} autoplay
 @param {Boolean} controls
 */
LAB.sound.Player = function( parent, loop, autoplay, controls ){		
	LAB.EventDispatcher.call( this, this );
	
	this.loop 		= loop || false;
	this.autoplay 	= autoplay || false;
	this.controls	= controls || false;
	
	this.eventsRegistered = false;
	
	if (!parent){
		console.log ("ERROR! please pass in an HTML element to attach the player to");
	} else {
		this.playerElement = document.createElement("audio");
		this.playerElement.style.border = "0px none";
		if (this.loop){
			this.playerElement.setAttribute('loop', 'loop');			
		}
		if (this.autoplay){
			this.playerElement.setAttribute('autoplay', 'autoplay');			
		}
		if (this.controls){
			this.playerElement.setAttribute('controls', 'controls');			
		}
		parent.appendChild(this.playerElement);
		this.playerElement.parent = parent;
	}	
}

// extend Event Dispatcher

LAB.sound.Player.prototype = new LAB.EventDispatcher();
LAB.sound.Player.prototype.constructor = LAB.sound.Player;
LAB.sound.Player.prototype.supr = LAB.EventDispatcher.prototype;

// METHODS

LAB.sound.Player.prototype.loadFile 		= function( url )
{	
	this.registerEvents();
	this.playerElement.setAttribute('src', url );
	this.playerElement.load();
}

// PLAYBACK METHODS

LAB.sound.Player.prototype.play	= function(){
	this.playerElement.play();
}

LAB.sound.Player.prototype.pause	= function(){
	this.playerElement.pause();
}

LAB.sound.Player.prototype.setVolume	= function(volume){
	this.playerElement.volume = volume;
}

LAB.sound.Player.prototype.seekTo	= function(time){
	this.playerElement.currentTime = time;
}

// EVENTS

/**
@function
@private
*/
LAB.sound.Player.prototype.registerEvents = function(){
	if (this.eventsRegistered) return;
	
	// register events
	this.playerElement.addEventListener("load", this.onLoad);
	this.playerElement.addEventListener("ended", this.onEnded);
	this.playerElement.addEventListener("error", this.onError);
	this.eventsRegistered = true;
}

/**
@function
@private
*/
LAB.sound.Player.prototype.onLoad	= function(event){
	this.dispatchEvent("onLoad", event);
}

/**
@function
@private
*/
LAB.sound.Player.prototype.onEnded	= function(event){
	this.dispatchEvent("onEnded", event);	
}

/**
@function
@private
*/
LAB.sound.Player.prototype.onError	= function(event){
	console.log(event);
	this.dispatchEvent("onError", event);	
}

// Getters
/**
@function
*/
LAB.sound.Player.prototype.getElement = function()
{
	return this.playerElement;
}
/**
@function
@param {boolean} bLoop
*/
LAB.sound.Player.prototype.isLooping = function( bLoop )
{
	return this.loop;
}
/**
@function
@param {boolean} bAutoplay
*/
LAB.sound.Player.prototype.isAutoplay = function( bAutoplay )
{
	return this.autoplay;
}

/**
@function
@param {boolean} bControls
*/
LAB.sound.Player.prototype.hasControls = function( bControls )
{
	return this.controls;
}

// Setters
/**
@function
*/

LAB.sound.Player.prototype.setLooping = function( bLoop )
{
	this.loop = bLoop;
	if (this.loop){
		this.playerElement.setAttribute('loop', 'loop');			
	} else {
		this.playerElement.removeAttribute('loop');
	}
}
/**
@function
*/
LAB.sound.Player.prototype.setAutoplay = function( bAutoplay )
{
	this.autoplay = bAutoplay;
	if (this.autoplay){
		this.playerElement.setAttribute('autoplay', 'autoplay');			
	} else {
		this.playerElement.removeAttribute('autoplay');
	}
}
/**
@function
*/
LAB.sound.Player.prototype.setControls = function( bControls )
{
	this.controls = bControls;
	if (this.controls){
		this.playerElement.setAttribute('controls', 'controls');			
	} else {
		this.playerElement.removeAttribute('controls');
	}
}