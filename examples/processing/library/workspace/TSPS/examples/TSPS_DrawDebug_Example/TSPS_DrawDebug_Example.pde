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
  
  // get array of people
  TSPSPerson[] people = tspsReceiver.getPeopleArray();
  
  // loop through people
  for (int i=0; i<people.length; i++){
      // draw person!
      noFill();
      stroke(255,100);
      rect(people[i].boundingRect.x*width, people[i].boundingRect.y*height, people[i].boundingRect.width*width, people[i].boundingRect.height*height);    
      
      // draw circle based on person's centroid (also from 0-1)
      fill(255,255,255);
      ellipse(people[i].centroid.x*width, people[i].centroid.y*height, 10, 10);
      
      // draw contours
      noFill();
      stroke(255,100);
      beginShape();
      for (int j=0; j<people[i].contours.size(); j++){
        PVector pt = (PVector) people[i].contours.get(j);
        if ( pt == null ){
          println(j);
        } else {
          vertex(pt.x*width, pt.y*height);
        }
      }
      endShape(CLOSE);

      // text shows more info available
      text("id: "+people[i].id+" age: "+people[i].age, people[i].boundingRect.x*width, (people[i].boundingRect.y*height + people[i].boundingRect.height*height) + 2);
  
  };
};




