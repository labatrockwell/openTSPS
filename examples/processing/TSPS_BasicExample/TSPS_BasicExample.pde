/**
  TSPS Basic Example
  by Brett Renfer, 4/3/12
  
  Draw rectangles based on people in a scene.
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
  
  tspsReceiver.update();
  
  // you can loop through all the people elements in TSPS if you choose
  // TSPS' "peopple" element is a EnumMap ( http://docs.oracle.com/javase/6/docs/api/java/util/EnumMap.html )
  
  // get enumeration, which helps us loop through tsps.people
  Enumeration e = tspsReceiver.people.keys();
  
  // loop through people
  while (e.hasMoreElements())
  {
    // get person
    int id = (Integer) e.nextElement();
    TSPSPerson person = (TSPSPerson) tspsReceiver.people.get( id );
      
      // draw rect based on person's detected size
      // dimensions from TSPS are 0-1, so we multiply by window width and height
      fill(120,120,0);
      rect(person.boundingRect.x*width, person.boundingRect.y*height, person.boundingRect.width*width, person.boundingRect.height*height);		
      
      // draw circle based on person's centroid (also from 0-1)
      fill(255,255,255);
      ellipse(person.centroid.x*width, person.centroid.y*height, 10, 10);
      
      // text shows more info available
      text("id: "+person.id+" age: "+person.age, person.boundingRect.x*width, (person.boundingRect.y*height + person.boundingRect.height*height) + 2);	
  };
};



