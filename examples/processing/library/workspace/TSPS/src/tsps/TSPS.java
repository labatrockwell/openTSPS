package tsps;

import netP5.*;
import oscP5.*;
import processing.core.PApplet;
import processing.core.PVector;

import java.util.*;
import java.lang.reflect.Method;

/***********************************************************************
 * OSC Message is structured like this:
 * 
 * address: TSPS/person/ + ordered id (i.e. TSPS/person/0)
 * 
 * argument 1: pid; argument 2: age; argument 3: centroid.x; argument 4:
 * centroid.y; argument 5: velocity.x; argument 6: velocity.y; argument 7:
 * boundingRect.x; argument 8: boundingRect.y; argument 9: boundingRect.width;
 * argument 10: boundingRect.height;
 ***********************************************************************/

public class TSPS {

	PApplet parent;
	OscP5 receiver;
	public Hashtable<Integer, TSPSPerson> people;

	Method personEntered;
	Method personUpdated;
	Method personLeft;

	int port = 12000;

	public static void updatePerson(TSPSPerson p, OscMessage theOscMessage) {
		p.id = theOscMessage.get(0).intValue();
		p.age = theOscMessage.get(1).intValue();
		p.centroid.x = theOscMessage.get(2).floatValue();
		p.centroid.y = theOscMessage.get(3).floatValue();
		p.velocity.x = theOscMessage.get(4).floatValue();
		p.velocity.y = theOscMessage.get(5).floatValue();
		p.boundingRect.x = theOscMessage.get(6).floatValue();
		p.boundingRect.y = theOscMessage.get(7).floatValue();
		p.boundingRect.width = theOscMessage.get(8).floatValue();
		p.boundingRect.height = theOscMessage.get(9).floatValue();
		p.opticalFlow.x = theOscMessage.get(10).floatValue();
		p.opticalFlow.y = theOscMessage.get(11).floatValue();
		p.contours.clear();
		for (int i = 12; i < theOscMessage.arguments().length - 13; i += 2) {
			PVector point = new PVector();
			point.x = theOscMessage.get(i).floatValue();
			point.y = theOscMessage.get(i + 1).floatValue();
			p.contours.add(point);
		}
	}

	public TSPS(PApplet _parent) {
		parent = _parent;
		receiver = new OscP5(this, port);
		people = new Hashtable<Integer, TSPSPerson>();
		registerEvents();
	}

	public TSPS(PApplet _parent, int port) {
		parent = _parent;
		receiver = new OscP5(this, port);
		people = new Hashtable<Integer, TSPSPerson>();

		registerEvents();
	}

	private void registerEvents() {
		// check to see if the host applet implements methods:
		// public void personEntered(TSPSPerson p)
		// public void personEntered(TSPSPerson p)
		// public void personEntered(TSPSPerson p)
		try {
			personEntered = parent.getClass().getMethod("personEntered",
					new Class[] { TSPSPerson.class });
			personUpdated = parent.getClass().getMethod("personUpdated",
					new Class[] { TSPSPerson.class });
			personLeft = parent.getClass().getMethod("personLeft",
					new Class[] { TSPSPerson.class });
		} catch (Exception e) {
			// no such method, or an error.. which is fine, just ignore
		}
	}

	public void update() {
		// Enumeration elements = people.elements();

		// for (int i=people.size()-1; i>=0; i--)
		// for (Enumeration elements = v.elements() ; e.hasMoreElements() ;) {
		// {
		// TSPSPerson checkPerson = (TSPSPerson) people.get(i);
		// if (checkPerson.dead == true){
		// people.remove(i);
		// }
		// }
	}

	public void oscEvent(OscMessage theOscMessage) {
		// adding a person
		if (theOscMessage.checkAddrPattern("TSPS/personEntered/")) {
			TSPSPerson p = new TSPSPerson();
			updatePerson(p, theOscMessage);
			callPersonEntered(p);
			// updating a person (or adding them if they don't exist in the
			// system yet)
		} else if (theOscMessage.checkAddrPattern("TSPS/personMoved/")) {

			TSPSPerson p = people.get(theOscMessage.get(0).intValue());
			boolean personExists = (p != null);
			if (!personExists) {
				p = new TSPSPerson();
			}

			updatePerson(p, theOscMessage);
			if (!personExists) {
				callPersonEntered(p);
			} else {
				callPersonUpdated(p);
			}
		}

		// killing an object
		else if (theOscMessage.checkAddrPattern("TSPS/personWillLeave/")) {
			TSPSPerson p = people.get(theOscMessage.get(0).intValue());
			if (p == null) {
				return;
			}
			updatePerson(p, theOscMessage);

			callPersonLeft(p);

			people.remove(p.id);
		}
	}

	private void callPersonEntered(TSPSPerson p) {
		people.put(p.id, p);
		if (personEntered != null) {
			try {
				personEntered.invoke(parent, new Object[] { p });
			} catch (Exception e) {
				System.err
						.println("Disabling personEntered() for TSPS because of an error.");
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
				System.err
						.println("Disabling personUpdated() for TSPS because of an error.");
				e.printStackTrace();
				personUpdated = null;
			}
		}
	}

	// private void callPersonContoursUpdated ( TSPSPerson p )
	// {
	// if (personUpdated != null) {
	// try {
	// personUpdated.invoke(parent, new Object[] { p });
	// } catch (Exception e) {
	// System.err.println("Disabling personUpdated() for TSPS because of an error.");
	// e.printStackTrace();
	// personUpdated = null;
	// }
	// }
	// }

	private void callPersonLeft(TSPSPerson p) {
		if (personLeft != null) {
			try {
				personLeft.invoke(parent, new Object[] { p });
			} catch (Exception e) {
				System.err
						.println("Disabling personLeft() for TSPS because of an error.");
				e.printStackTrace();
				personLeft = null;
			}
		}
	}
};