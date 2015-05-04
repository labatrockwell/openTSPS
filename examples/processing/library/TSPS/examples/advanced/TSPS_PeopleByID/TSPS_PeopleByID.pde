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

// where you'll store a tracking ID
int trackingID = -1;

void draw(){
  background(0);
  
  // get people by ID
  // first, get all the keys from the "people" hash table
  Array ids = tspsReceiver.people.keySet().toArray();
  
  // is TSPS tracking anyone?
  if ( ids.length() > 0 ){
    
    // are we tracking any IDs in processing?
    if ( trackingID != -1 ){
      // check for our ID
      if ( !tspsReceiver.people.containsKey( trackingID ) ){
        // reset tracking ID if not found
        trackingID = -1;
      }
    }
    
    // either not tracking anyone, or just reset
    if ( trackingID == -1){
 
      // assign the first blob ID to tracking ID
      // can be 0 - ids.length()-1
      trackingID = ids[0];  
    }
    
  // not tracking anyone
  } else {
    trackingID = -1;
  }
  
  // if we have an ID, do something!
  if ( trackingID != -1 ){
    TSPSPerson p = tspsReceiver.people.get( trackingID );
    fill(255,255,255);
    ellipse(p.centroid.x*width, p.centroid.y*height, 10, 10);
      
  }
  
};
