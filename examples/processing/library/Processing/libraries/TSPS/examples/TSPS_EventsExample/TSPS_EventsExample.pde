/**
  TSPS Events Example
  by Brett Renfer, 4/5/12
  
  Show text when new people enter and leave!
*/

// import TSPS
import tsps.*;
TSPS tspsReceiver;

int lastDrawn = 0;

void setup(){
  size(1024,768);
  background( 0 );
  noStroke();
  
  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
};

void draw(){
  // quickly erase the background
  fill(0, 5);
  rect(0,0,width,height);
  
  fill( 255 );
  
  // we haven't seen anyone in 3 seconds, time to complain!
  if ( tspsReceiver.people.size() == 0 && millis() - lastDrawn > 3000 ){
    background( 0 );
    textAlign( CENTER );
    text( "I'm all alone in this cruel world :(", width / 2, height / 2 );
  };  
  
  // just update TSPS; all of our drawing is in TSPS events
  tspsReceiver.update();
};

void personEntered( TSPSPerson p ){
  background( 0 );
  fill( 255 );
  textAlign( CENTER );
  
  text( "Hello new person!", width / 2, height / 2 );
}

void personUpdated( TSPSPerson p ){
}

void personLeft( TSPSPerson p ){
  background( 0 );
  int numPeopleLeft = tspsReceiver.people.size() - 1;
  
  fill( 255 );
  textAlign( CENTER );
  
  // no one left :(
  if (numPeopleLeft == 0){
    text( "All alone again...", width / 2, height / 2 );
  } else {
    text( "See ya!\nGlad I've got "+ numPeopleLeft +" more friends to kick it with.", width / 2, height / 2 );
  }
  lastDrawn = millis();
    
}
