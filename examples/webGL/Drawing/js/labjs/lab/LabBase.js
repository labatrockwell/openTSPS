// include required files
/** @namespace LAB*/
var LAB = LAB || {};

// reference to global context, in most cases 'window'.
LAB.global = this;

// require based on: http://closure-library.googlecode.com/svn/trunk/closure/goog/base.js

/*
 * helper (for javascript importing within javascript).
 */

var included = {};

/**
 * Copied from Flanagan, David. Javascript: The Definitive Guide.
 * Use this function to inherit objects.
 *
 * @function
 * @example
 * Child.prototype = LAB.inherit(Parent.prototype);
 * Child.prototype.constructor = Child;
 *
 * @param {Object} p The Parent object to inherit. The object can either be the prototype of the Parent
 * object (Parent.prototype) or a new instance of the parent (new Parent())
 * @return {Object} A copy of the prototype of the Parent object.
 */
LAB.inherit = function(p) {
	if (p == null) throw TypeError(); // p must be a non-null object
	if (Object.create) { // If Object.create() is defined...
		return Object.create(p); // then just use it
	}
	var t = typeof p; // otherwise do some more type checking
	if (t !== "object" && t !== "function") throw TypeError();
	function f() {}; // define a dummy constructor function
	f.prototype = p; // Set its prototype property to p
	return new f(); // use f() to create an 'heir' of p.
};

// alternate version of inherit:
///**
// * Copied from Stefanov, Stoyan. JavaScript Patterns.
// * Use this function to inherit objects
// *
// * @function
// * @example
// * LAB.inherit(Child, Parent);
// *
// * @param {Object} C The Child object.
// * @param {Object} P The Parent object.
// */
//LAB.utils.inherit = function(C, P) {
//	var F = function() {};
//	F.prototype = P.prototype;
//	C.prototype = new F();
//	C.uber = P.prototype;
//	C.prototype.constructor = C;
//}

/**
 * Use this function to safely create a new namespace
 * if a namespace already exists, it won't be recreated.
 *
 * @function
 * @param {String} nsString The namespace as a string.
 * @return {Object} The namespace object.
 */
LAB.namespace = function (nsString) {
	var parts = nsString.split('.'),
		parent = LAB,
		i;
		
	// strip redundant leading global
	if (parts[0] === "LAB") {
		parts = parts.slice(1);
	}
	
	for (i=0; i<parts.length; i +=1) {
		// create a property if it doesn't exist
		if (typeof parent[parts[i]] === "undefined") {
			parent[parts[i]] = {};
		}
		parent = parent[parts[i]];
	}
	return parent;
};

/**
@function
@private
*/
LAB.writeScriptTag_ = function(src) {
	var doc = LAB.global.document;
	doc.write(
		'<script type="text/javascript" src="' + src + '"></' + 'script>');
	included[src] = true;
	return true;
};

/**
	Use this to load script files! (only script files right now)
	@function
	@public
*/
LAB.require = function(src) {
	src in included ? console.log("already included") : LAB.writeScriptTag_(src);
};

LAB.getScriptPath = function(filename) {
	var scripts = document.getElementsByTagName('script');

    for (var i = scripts.length - 1; i >= 0; --i) {
		var src = scripts[i].src;
		var l = src.length;
		var length = filename.length;
		if (src.substr(l - length) == filename) {
        	// set a global propery here
        	return src.substr(0, l - length);
		}
    }
	return "";
};

LAB.toScriptPath = function( className ){
	return LAB.src+className+".js";
};

LAB.src= LAB.getScriptPath("LabBase.js");

// start including stuff
LAB.require( LAB.src+"EventDispatcher.js" );
LAB.require( LAB.src+"app/BaseApp.js" );
LAB.require( LAB.src+"Utils.js" );

// key constants
LAB.SHIFT 	= 16;
LAB.CONTROL = 17;
LAB.ALT		= 18;
LAB.COMMAND = 91;