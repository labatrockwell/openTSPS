/**
  TSPS Basic 3D Example
  by Brett Renfer, 4/3/12
  
  People tracked via Kinect have a depth property! Draw spheres
  related to their distance.
*/
// get 3D!
import processing.opengl.*;

// import TSPS
import tsps.*;
TSPS tspsReceiver;

void setup(){
  size(1024,768, OPENGL);
	
  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
};

void draw(){
  background(0);
  
  tspsReceiver.update();
  
    // get enumeration, which helps us loop through tsps.people
  Enumeration e = tspsReceiver.people.keys();
  
  // loop through people
  while (e.hasMoreElements())
  {
    // get person
    int id = (Integer) e.nextElement();
    TSPSPerson person = (TSPSPerson) tspsReceiver.people.get( id );
    
    pushMatrix();
    translate( person.centroid.x * width, person.centroid.y * height, person.depth );
    ellipse( 20, 20, 20 );
    popMatrix();
  };
};
