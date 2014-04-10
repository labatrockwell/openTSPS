/**
  TSPS Basic 3D Example
  by Brett Renfer, 4/3/12
  
  People tracked via Kinect have a depth property! Draw spheres
  related to their distance. If you're not using a depth camera,
  this example might act a little weird.
*/
// get 3D!
import processing.opengl.*;

// import TSPS
import tsps.*;
TSPS tspsReceiver;

void setup(){
  size(1024,768, OPENGL);
	
  lights();

  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
};

void draw(){
  background(0);
  fill(255);
  
  // get array of people
  TSPSPerson[] people = tspsReceiver.getPeopleArray();
  
  // loop through people
  for (int i=0; i<people.length; i++){
    // get person
    TSPSPerson person = people[i];
    
    pushMatrix();
    translate( person.centroid.x * width, person.centroid.y * height, person.depth / 2 );
    sphere( person.boundingRect.width * width / 2 );
    popMatrix();
  };
};
