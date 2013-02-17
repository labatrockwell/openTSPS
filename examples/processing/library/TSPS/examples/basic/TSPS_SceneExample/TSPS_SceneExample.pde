/**
  TSPS Basic Example
  by Brett Renfer, 9/8/12
  
  Set up TSPS + use the built-in Person draw function to visualize the data you're getting.
*/

// import TSPS
import tsps.*;
TSPS tspsReceiver;

void setup(){
  size(1024,768);
	
  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
};

void draw(){
  background(0);
  
  // the scene can tell you how many people are there
  text("I see "+ tspsReceiver.scene.getNumPeople() + " people", width/2, height/2);
  
  // you can draw the scene; if you set up a detection grid in TSPS, it will show up here!
  tspsReceiver.scene.draw(width, height);
};




