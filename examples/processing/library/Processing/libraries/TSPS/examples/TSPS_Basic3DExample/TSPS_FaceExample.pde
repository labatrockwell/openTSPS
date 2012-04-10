/**
  TSPS Haar Example
  by Brett Renfer, 4/3/12
  
  Draw a face when a haar element is detected!
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
  
  // get enumeration, which helps us loop through tsps.people
  Enumeration e = tspsReceiver.people.keys();
  
  // loop through people
  while (e.hasMoreElements())
  {
    // get person
    int id = (Integer) e.nextElement();
    TSPSPerson person = (TSPSPerson) tspsReceiver.people.get( id );
    
    // if the dimensions of your haar rectangle are not 0,0,0,0, there's something there!
    // note: just like other values, a person's haar rectangle is 0-1, so we need to multiply
    // by width (x,width) or height (u, height)
    if ( person.haarRect.x*width >= 0 || person.haarRect.y*height >= 0 || person.haarRect.width*width >= 0 person.haarRect.height*height >= 0){
      pushMatrix();
      translate( person.haarRect.x*width, person.haarRect.y*height );
      ellipse(0,0,20,20);
      ellipse(0,person.haarRect.width*width, 20,20);
      rect(0,person.haarRect.height*height - 5, person.haarRect.width*width, 5);
      popMatrix();
    }
  };
};



