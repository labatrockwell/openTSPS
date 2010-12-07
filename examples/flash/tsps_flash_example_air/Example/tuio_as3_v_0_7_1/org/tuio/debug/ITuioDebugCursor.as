package org.tuio.debug
{
	/**
	 * <p><code>DisplayObject</code>s that implement <code>ITuioDebugCursor</code> are used by TuioDebug to show a debug cursor of
	 * a tuio cursor element.</p>
	 * 
	 * <p><code>ITuioDebugCursor</code> is a marker interface without any function declarations. If a <code>DisplayObject</code> 
	 * implements <code>ITuioDebugCursor</code>
	 * TouchEvents will not be dispatched onto it by the <code>TuioManager</code> if it is under a finger. Instead, the 
	 * TouchEvent will be dispatched to the <code>DisplayObject</code> underneath the <code>ITuioDebugCursor</code> object.</p>
	 * 
	 *  <p>This prevents a problem in the implementation of <code>TuioManager</code>, which dispatches TouchEvents onto the 
	 * topmost <code>DisplayObject</code> under a tuio cursor.</p>
	 *  
	 * @author johannes
	 * 
	 */
	public interface ITuioDebugCursor
	{
		//necessary for mouse to tuio simulation
		function get sessionId():uint;
		function set sessionId(sessionId:uint):void;
	}
}