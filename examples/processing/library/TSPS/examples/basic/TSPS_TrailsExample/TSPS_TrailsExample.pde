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
  
  // get array of people
  TSPSPerson[] people = tspsReceiver.getPeopleArray();
  
  // loop through people
  for (int i=0; i<people.length; i++){
    // get person
    TSPSPerson person = people[i];
    
    ellipse( person.centroid.x * width, person.centroid.y * height, 5, 5 );
    
  };
};
