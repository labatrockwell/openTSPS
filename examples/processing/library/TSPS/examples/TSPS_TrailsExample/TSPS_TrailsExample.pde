/**
  TSPS Drawing Example
  by Brett Renfer, 4/5/12
  
  Draw slowly fading paths based on people's movement.
*/

// import TSPS
import tsps.*;
TSPS tspsReceiver;

void setup(){
  frameRate( 60 );
  size(1024,768);
  
  background( 0 );
	
  noStroke();
  smooth();

  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
};

void draw(){
  // very slowly erase the background
  fill(0, 2);
  rect(0,0,width,height);
  
  fill( 255 );
  
  // update TSPS
  tspsReceiver.update();
  
  // get enumeration, which helps us loop through tsps.people
  Enumeration e = tspsReceiver.people.keys();
  
  // loop through people
  while (e.hasMoreElements())
  {
    // get person
    int id = (Integer) e.nextElement();
    TSPSPerson person = (TSPSPerson) tspsReceiver.people.get( id );
    
    ellipse( person.centroid.x * width, person.centroid.y * height, 5, 5 );
    
  };
};
