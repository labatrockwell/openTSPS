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
};

/* TSPS EVENTS:*/

/* Add these functions to your app to catch events when people enter a scene,
move around, or leave */

void customEvent( ArrayList<String> strings ){
  println("cool");
  for ( int i=0; i<strings.size(); i++){
    println(strings.get(i));
  }
}
