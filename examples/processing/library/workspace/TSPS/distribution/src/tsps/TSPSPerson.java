package tsps;

import processing.core.PApplet;
import processing.core.PVector;
import java.util.ArrayList;

public class TSPSPerson
{
	
	private final PApplet app;

	public int id; 
	public int oid; 
	public int age; 
	public float depth; 
	public PVector centroid;  
	public PVector velocity;
	public PVector highest; 
	public PVector opticalFlow; 
	public Rectangle boundingRect;
	public Rectangle haarRect;
	public boolean dead;
	
	public ArrayList<PVector> contours;
	
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
	}
	
	public void update ( TSPSPerson p ){
		age 		= p.age;
	 	depth 		= p.depth;
		boundingRect = p.boundingRect;
		centroid 	= p.centroid;
		velocity 	= p.velocity;
		opticalFlow = p.opticalFlow;
		contours 	= p.contours;
		highest		= p.highest;
	}

	public void draw(){
		// draw rect based on person's detected size
    	// dimensions from TSPS are 0-1, so we multiply by window width and height
      	app.fill(120,120,0);
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