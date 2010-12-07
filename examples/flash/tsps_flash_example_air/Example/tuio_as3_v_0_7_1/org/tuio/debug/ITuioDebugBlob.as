package org.tuio.debug
{
	/**
	 * <p><code>DisplayObject</code>s that implement <code>ITuioDebugBlob</code> are used by TuioDebug to show a debug cursor of
	 * a tuio blob element.</p>
	 * 
	 * <p><code>ITuioDebugBlob</code> is a marker interface without any function declarations. If a <code>DisplayObject</code> 
	 * implements <code>ITuioDebugBlob</code>
	 * TouchEvents will not be dispatched onto it by the <code>TuioManager</code> if it is under a finger. Instead, the 
	 * TouchEvent will be dispatched to the <code>DisplayObject</code> underneath the <code>ITuioDebugBlob</code> object.</p>
	 * 
	 *  <p>This prevents a problem in the implementation of <code>TuioManager</code>, which dispatches TouchEvents onto the 
	 * topmost <code>DisplayObject</code> under a tuio blob.</p>
	 *  
	 * @author johannes
	 * 
	 */
	public interface ITuioDebugBlob
	{
		
	}
}