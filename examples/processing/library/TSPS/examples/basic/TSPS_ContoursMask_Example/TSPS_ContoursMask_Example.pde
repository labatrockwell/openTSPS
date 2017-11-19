/**
  TSPS Basic Example
  by Brett Renfer, 9/8/12
  
  Set up TSPS + use the built-in Person draw function to visualize the data you're getting.
*/

//import OSCP5
import netP5.*;
import oscP5.*;

// import TSPS
import tsps.*;
TSPS tspsReceiver;

PGraphics mask;
PImage    toMask;

void setup(){
  size(1024,768);
	
  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
  toMask = loadImage("dalicat.jpg");
  mask = createGraphics( toMask.width,  toMask.height );
};

void draw(){
  background(255);
  
  // get array of people
  TSPSPerson[] people = tspsReceiver.getPeopleArray();
  
  mask.beginDraw();
  mask.background(0);
  mask.fill(255);
  // loop through people
  for (int i=0; i<people.length; i++){
      
      // draw contours
      mask.beginShape();
      for (int j=0; j<people[i].contours.size(); j++){
        PVector pt = (PVector) people[i].contours.get(j);
        if ( pt != null ){
          mask.vertex(pt.x*width, pt.y*height);
        }
      }
      mask.endShape(CLOSE);

  };
  mask.endDraw();
  
//  image(mask,0,0);
  toMask.mask(mask);
  image(toMask,0,0);
};




