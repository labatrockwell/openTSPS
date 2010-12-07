package org.tuio
{
	import org.tuio.TouchEvent;

	public interface ITuioTouchReceiver
	{
		function updateTouch(event:TouchEvent):void;
		function removeTouch(event:TouchEvent):void;
	}
}