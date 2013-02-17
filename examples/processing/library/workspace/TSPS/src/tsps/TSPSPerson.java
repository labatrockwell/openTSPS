package tsps;

import processing.core.PApplet;
import processing.core.PVector;
import java.util.ArrayList;
import java.util.Collections;

/**
 * TSPS Person object, containing properties unique to a specific tracked person
 */
public class TSPSPerson
{	
	private final PApplet app;

	/** Unique ID, different for each Person as long as TSPS is running */
	public int id;
	/** Ordered ID (not usually used), ID ranging from 0-Total Number of people */ 
	public int oid; 
	/** How long a person has been around (in seconds) */
	public int age; 
	/** Normalized (0.0-1.0) distance from camera. For Kinect camera, highest value (1) is approx. 10 meters*/
	public float depth; 
	/** Center of mass of person */
	public PVector centroid;  
	/** Speed since last update */
	public PVector velocity;
	/** Closest point to the camera (with Kinect). If using non-depth camera, represents brightest point on person. */
	public PVector highest; 
	/** Average motion within a Person's area */
	public PVector opticalFlow; 
	/** Bounding rectangle that surrounds Person's shape*/
	public Rectangle boundingRect;
	/** Rectangle representing a detected HAAR feature (if there is one) */
	public Rectangle haarRect;
	/** Defines the rough outline of a Person*/
	public ArrayList<PVector> contours;
	/** (deprecated) */
	public int lastUpdated;
	/** (deprecated) */
	public boolean dead;
	
	/**
	 * Create a TSPS Person object
	 * @param PApplet	Pass in app to enable debug drawing of TSPS Person object
	 */
	public TSPSPerson( PApplet _app ){
		app 			= _app;
		boundingRect 	= new Rectangle();
		haarRect 		= new Rectangle();
		centroid 		= new PVector();
		velocity 		= new PVector();
		opticalFlow 	= new PVector();
		highest			= new PVector();
		dead 			= false;
		contours 		= new ArrayList<PVector>();
		lastUpdated		= 0;
	}

	public void copy( TSPSPerson p){
		id 				= p.id;
		oid 			= p.oid; 
		age 			= p.age; 
		depth 			= p.depth;
		centroid 		= p.centroid;
		velocity 		= p.velocity;
		highest  		= p.highest;
		opticalFlow 	= p.opticalFlow;
		boundingRect	= p.boundingRect;
		haarRect 		= p.haarRect;
		lastUpdated		= p.lastUpdated;

		contours.clear();
		for (int i=0; i<p.contours.size(); i++){
			PVector pt = (PVector) p.contours.get(i);
			// why is this happening??
			if ( pt != null ){
				PVector co = new PVector(pt.x, pt.y);
				contours.add(co);
			}
		}
		dead 			= p.dead;
	}

	/**
	 * Draw an debug view
	 */
	public void draw(){
		// draw rect based on person's detected size
    	// dimensions from TSPS are 0-1, so we multiply by window width and height
      	app.noFill();
		app.stroke(255,100);
      	app.rect(boundingRect.x*app.width, boundingRect.y*app.height, boundingRect.width*app.width, boundingRect.height*app.height);		
      
    	// draw circle based on person's centroid (also from 0-1)
    	app.fill(255,255,255);
    	app.ellipse(centroid.x*app.width, centroid.y*app.height, 10, 10);
    	
    	// draw contours
    	app.noFill();
		app.stroke(255,100);
		app.beginShape();
		for (int i=0; i<contours.size(); i++){
			PVector pt = (PVector) contours.get(i);
			app.vertex(pt.x*app.width, pt.y*app.height);
		}
		app.endShape(PApplet.CLOSE);

    	// text shows more info available
    	app.text("id: "+id+" age: "+age, boundingRect.x*app.width, (boundingRect.y*app.height + boundingRect.height*app.height) + 2);
	}
};