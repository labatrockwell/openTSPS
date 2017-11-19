/**
  TSPS Haar Example
  by Brett Renfer, 4/3/12
  
  Draw a face when a haar element is detected! :|
  Haar tracking must be turned on for this to work! 
  - check "detect and send features in blobs" under the "sensing" panel
*/

//import OSCP5
import netP5.*;
import oscP5.*;

// import TSPS
import tsps.*;
TSPS tspsReceiver;

void setup(){
  size(1024,768);
	
  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
  noStroke();
};

void draw(){
  background(0);
  
  // get array of current people
  TSPSPerson[] people = tspsReceiver.getPeopleArray();
  
  // loop through people
  for (int i=0; i<people.length; i++){
    // get person
    TSPSPerson person = people[i];
    
    // if the dimensions of your haar rectangle are not 0,0,0,0, there's something there!
    // note: just like other values, a person's haar rectangle is 0-1, so we need to multiply
    // by width (x,width) or height (u, height)
    if ( person.haarRect.x*width > 0 || person.haarRect.y*height > 0 || person.haarRect.width*width > 0 || person.haarRect.height*height > 0){
      // head
      fill(50);
      ellipseMode( CORNER );
      ellipse( person.haarRect.x*width,  person.haarRect.y*height,  person.haarRect.width*width,  person.haarRect.height*height);
      
      // face
      fill(255);
      pushMatrix();
        // translate to center of haar rectangle and scale according to its size
        translate( person.haarRect.x*width + person.haarRect.width*width/2, person.haarRect.y*height + person.haarRect.height*height/2);
        scale( (person.haarRect.width)*5,  (person.haarRect.height)*5 );
        
        // left eye
        ellipseMode( CENTER );
        ellipse( -30, -40, 10, 10);
        // right eye
        ellipse( 30, -40, 10, 10 );
        // mouth
        rect( -70, 20, 140, 5 );
      popMatrix();
    }
  };
};



