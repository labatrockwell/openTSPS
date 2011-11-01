//-----------------------------------------------------------------
// IMPORT LIBS
//-----------------------------------------------------------------
import tsps.*;
TSPS tspsReceiver;

//-----------------------------------------------------------------
// SETUP
//-----------------------------------------------------------------
void setup(){
  size(1024,768);
	
  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
};

//-----------------------------------------------------------------
// DRAW
//-----------------------------------------------------------------
void draw(){
  background(0);
  tspsReceiver.update();
	
  //you can loop through all the people elements in TSPS if you choose
  for (Enumeration e = tspsReceiver.people.keys() ; e.hasMoreElements() ;)
  {
    //get person
    int i = (Integer) e.nextElement();
    TSPSPerson person = (TSPSPerson) tspsReceiver.people.get(i);
      
      //draw rect + centroid scaled by activeWidth + activeHeight
      fill(120,120,0);
      rect(person.boundingRect.x*width, person.boundingRect.y*height, person.boundingRect.width*width, person.boundingRect.height*height);		
      fill(255,255,255);
      ellipse(person.centroid.x*width, person.centroid.y*height, 10, 10);
      text("id: "+person.id+" age: "+person.age, person.boundingRect.x*width, (person.boundingRect.y*height + person.boundingRect.height*height) + 2);	
  };
};

//-----------------------------------------------------------------
// PERSON ENTERED
//-----------------------------------------------------------------
void personEntered( TSPSPerson p ){
}

//-----------------------------------------------------------------
// PERSON UPDATED
//-----------------------------------------------------------------
void personUpdated( TSPSPerson p ){
}

//-----------------------------------------------------------------
// PERSON LEFT
//-----------------------------------------------------------------
void personLeft( TSPSPerson p ){
}




