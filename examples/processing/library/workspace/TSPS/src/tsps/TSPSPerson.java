package tsps;

import processing.core.PVector;
import java.util.ArrayList;

public class TSPSPerson
{
	
	public int id; 
	public int oid; 
	public int age; 
	public int depth; 
	public PVector centroid;  
	public PVector velocity; 
	public PVector opticalFlow; 
	public Rectangle boundingRect;
	public Rectangle haarRect;
	public boolean dead;
	
	public ArrayList<PVector> contours;
	
	public TSPSPerson(){
		boundingRect 	= new Rectangle();
		haarRect 		= new Rectangle();
		centroid 		= new PVector();
		velocity 		= new PVector();
		opticalFlow 	= new PVector();
		dead 			= false;
		contours 		= new ArrayList<PVector>();
	}
	
	public void update ( TSPSPerson p )
	{
		age 		= p.age;
	 	depth 		= p.depth;
		boundingRect = p.boundingRect;
		centroid 	= p.centroid;
		velocity 	= p.velocity;
		opticalFlow = p.opticalFlow;
		contours 	= p.contours;
	}
};