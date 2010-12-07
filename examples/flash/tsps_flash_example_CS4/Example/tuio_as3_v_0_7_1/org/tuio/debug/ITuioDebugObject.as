package org.tuio.debug
{
	/**
	 * <p><code>DisplayObject</code>s that implement <code>ITuioDebugObject</code> are used by TuioDebug to show debug information of
	 * a tuio object element (aka fiducial).</p>
	 * 
	 * <p><code>ITuioDebugObject</code> is a marker interface without any function declarations. If a <code>DisplayObject</code> 
	 * implements <code>ITuioDebugObject</code>
	 * TouchEvents will not be dispatched onto it by the <code>TuioManager</code> if it is under a finger. Instead, the 
	 * TouchEvent will be dispatched to the <code>DisplayObject</code> underneath the <code>ITuioDebugObject</code> object.</p>
	 * 
	 *  <p>This prevents a problem in the implementation of <code>TuioManager</code>, which dispatches TouchEvents onto the 
	 * topmost <code>DisplayObject</code> under a tuio cursor.</p>
	 *  
	 * @author johannes
	 * 
	 */
	public interface ITuioDebugObject
	{
		
	}
}