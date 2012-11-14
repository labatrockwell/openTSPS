package tsps;

import netP5.*;
import oscP5.*;
import processing.core.PApplet;
import processing.core.PVector;

import java.util.*;
import java.lang.reflect.Method;
import java.lang.Integer;

/**
 * TSPS Connection: Connects to TSPS app and provides your applet with TSPS People objects as they arrive.
 */

public class TSPS {

	private final PApplet parent;
	private OscP5 receiver;

	/**
	 * Hashtable of current People objects. Accessibly by unique id, e.g. people.get( 0 )
	 */
	public Hashtable<Integer, TSPSPerson> people;

	/**
	 * Current active list of People, copied to public list before every call to draw()
	 */
	private Hashtable<Integer, TSPSPerson> _currentPeople;
	
	private Method personEntered;
	private Method personUpdated;
	private Method personLeft;
	private Method customEvent;

	private int defaultPort = 12000;

	/**
	 * Starts up TSPS with the default port (12000).
	 * Will also attempt to set up default TSPS events, so will look for three methods implemented in your app:
	 * void personEntered( TSPSPerson p);
	 * void personUpdated( TSPSPerson p);
	 * void personLeft( TSPSPerson p);
	 * @param PApplet _parent Your app (pass in as "this")
	 */
	public TSPS(PApplet _parent) {
		parent = _parent;
		receiver = new OscP5(this, defaultPort);
		people = new Hashtable<Integer, TSPSPerson>();
		_currentPeople = new Hashtable<Integer, TSPSPerson>();
		registerEvents();
		parent.registerPre(this);
	}

	/**
	 * Starts up TSPS with a specific port. The port must match what is specified in the TSPS GUI.
	 * Will also attempt to set up default TSPS events, so will look for three methods implemented in your app:
	 * void personEntered( TSPSPerson p);
	 * void personUpdated( TSPSPerson p);
	 * void personLeft( TSPSPerson p);
	 * @param PApplet	_parent Your app (pass in as "this")
	 * @param int  		Port set in TSPS app
	 */
	public TSPS(PApplet _parent, int port) {
		parent = _parent;
		receiver = new OscP5(this, port);
		people = new Hashtable<Integer, TSPSPerson>();
		_currentPeople = new Hashtable<Integer, TSPSPerson>();

		registerEvents();
		parent.registerPre(this);
	}	

	public void pre(){

		// get enumeration, which helps us loop through tsps.people
		Enumeration e = _currentPeople.keys();
		  
		// loop through people + copy all to public hashtable
		people.clear();
		while (e.hasMoreElements())
		{
		    // get person
		    int id = (Integer) e.nextElement();
		    TSPSPerson person = (TSPSPerson) _currentPeople.get( id );

		    person.lastUpdated --;
		    // haven't gotten an update in ~2 seconds
		    if ( person.lastUpdated < -120 ){
		    	callPersonLeft(person);
				_currentPeople.remove(person.id);
		    } else {
		    	TSPSPerson p = new TSPSPerson(parent);
		    	p.copy(person);
		    	people.put(p.id, p);
		    }
		}
	}

	/**
	 * Access the current People objects as an array instead of a Hashmap
	 * @return Array of TSPSPerson objects
	*/
	public TSPSPerson[] getPeopleArray(){
		return (TSPSPerson [])(people.values().toArray(new TSPSPerson [people.values().size()]));
	}

	/**
	 * @return Current number of people
	 */
	public int getNumPeople(){
		return people.size();
	}

	// Update a person
	private static void updatePerson(TSPSPerson p, OscMessage theOscMessage) {
		p.id 					= theOscMessage.get(0).intValue();
		p.oid 					= theOscMessage.get(1).intValue();
		p.age 					= theOscMessage.get(2).intValue();
		p.centroid.x 			= theOscMessage.get(3).floatValue();
		p.centroid.y 			= theOscMessage.get(4).floatValue();
		p.velocity.x 			= theOscMessage.get(5).floatValue();
		p.velocity.y 			= theOscMessage.get(6).floatValue();
		p.depth 	 			= theOscMessage.get(7).floatValue();
		p.boundingRect.x 		= theOscMessage.get(8).floatValue();
		p.boundingRect.y 		= theOscMessage.get(9).floatValue();
		p.boundingRect.width 	= theOscMessage.get(10).floatValue();
		p.boundingRect.height 	= theOscMessage.get(11).floatValue();
		p.highest.x 			= theOscMessage.get(12).floatValue();
		p.highest.y 			= theOscMessage.get(13).floatValue();
		p.haarRect.x 			= theOscMessage.get(14).floatValue();
		p.haarRect.y 			= theOscMessage.get(15).floatValue();
		p.haarRect.width 		= theOscMessage.get(16).floatValue();
		p.haarRect.height 		= theOscMessage.get(17).floatValue();
		p.opticalFlow.x 		= theOscMessage.get(18).floatValue();
		p.opticalFlow.y 		= theOscMessage.get(19).floatValue();
		p.contours.clear();
		for (int i = 20; i < theOscMessage.arguments().length; i += 2) {
			PVector point = new PVector();
			point.x = theOscMessage.get(i).floatValue();
			point.y = theOscMessage.get(i + 1).floatValue();
			p.contours.add(point);
		}
		p.lastUpdated++;
	}

	// Set up (optional) TSPS Events
	private void registerEvents() {
		// check to see if the host applet implements methods:
		// public void personEntered(TSPSPerson p)
		// public void personEntered(TSPSPerson p)
		// public void personEntered(TSPSPerson p)
		// public void customEvent(ArrayList<String> args)
		try {
			personEntered = parent.getClass().getMethod("personEntered",
					new Class[] { TSPSPerson.class });
		} catch (Exception e) {
			// no such method, or an error.. which is fine, just ignore
		}
		try {
			personUpdated = parent.getClass().getMethod("personUpdated",
					new Class[] { TSPSPerson.class });
		} catch (Exception e) {
			// no such method, or an error.. which is fine, just ignore
		}
		try {
			personLeft = parent.getClass().getMethod("personLeft",
					new Class[] { TSPSPerson.class });
		} catch (Exception e) {
			// no such method, or an error.. which is fine, just ignore
		}
		try {
			System.err.println("Set up custom event");
			customEvent = parent.getClass().getMethod("customEvent",
					new Class[] { ArrayList.class });
		} catch (Exception e) {
			// no such method, or an error.. which is fine, just ignore
		}
	}

	// Parse incoming OSC Message
	protected void oscEvent(OscMessage theOscMessage) {
		// adding a person
		if (theOscMessage.checkAddrPattern("/TSPS/personEntered/")) {
			TSPSPerson p = new TSPSPerson( parent );
			updatePerson(p, theOscMessage);
			callPersonEntered(p);

		// updating a person (or adding them if they don't exist in the system yet)
		} else if (theOscMessage.checkAddrPattern("/TSPS/personUpdated/")) {

			TSPSPerson p = _currentPeople.get(theOscMessage.get(0).intValue());
			boolean personExists = (p != null);
			if (!personExists) {
				p = new TSPSPerson( parent );
			}

			updatePerson(p, theOscMessage);
			if (!personExists) {
				callPersonEntered(p);
			} else {
				callPersonUpdated(p);
			}
		}

		// person is about to leave
		else if (theOscMessage.checkAddrPattern("/TSPS/personWillLeave/")) {
			TSPSPerson p = _currentPeople.get(theOscMessage.get(0).intValue());
			if (p == null) {
				return;
			}
			updatePerson(p, theOscMessage);

			callPersonLeft(p);
			_currentPeople.remove(p.id);
		}

		// scene
		else if (theOscMessage.checkAddrPattern("/TSPS/scene")){
			
		}

		// custom event
		else if (theOscMessage.checkAddrPattern("/TSPS/customEvent")){
			System.err.println("custom");
			ArrayList<String> args = new ArrayList<String>();
			for (int i=0; i<theOscMessage.arguments().length; i++){
				args.add( theOscMessage.get(i).stringValue());
			}
			callCustomEvent(args);
		}
	}

	private void callPersonEntered(TSPSPerson p) {
		_currentPeople.put(p.id, p);
		if (personEntered != null) {
			try {
				personEntered.invoke(parent, new Object[] { p });
			} catch (Exception e) {
				System.err.println("Disabling personEntered() for TSPS because of an error.");
				e.printStackTrace();
				personEntered = null;
			}
		}
	}

	private void callPersonUpdated(TSPSPerson p) {
		if (personUpdated != null) {
			try {
				personUpdated.invoke(parent, new Object[] { p });
			} catch (Exception e) {
				System.err.println("Disabling personUpdated() for TSPS because of an error.");
				e.printStackTrace();
				personUpdated = null;
			}
		}
	}

	private void callPersonLeft(TSPSPerson p) {
		if (personLeft != null) {
			try {
				personLeft.invoke(parent, new Object[] { p });
			} catch (Exception e) {
				System.err.println("Disabling personLeft() for TSPS because of an error.");
				e.printStackTrace();
				personLeft = null;
			}
		}
	}

	private void callCustomEvent(ArrayList<String> strings) {
		if (customEvent != null) {
			try {
				customEvent.invoke(parent, new Object[] { strings });
			} catch (Exception e) {
				System.err.println("Disabling customEvent() for TSPS because of an error.");
				e.printStackTrace();
				customEvent = null;
			}
		}
	}
};