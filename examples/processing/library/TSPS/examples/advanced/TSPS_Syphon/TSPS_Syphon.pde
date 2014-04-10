/**
  TSPS +Syphon example
  by Brett Renfer, 9/8/12
  
  Set up TSPS + use the built-in Person draw function to visualize the data you're getting.
  !!! This requires the Syphon processing library, which is found here:
  https://code.google.com/p/syphon-implementations/downloads/list
*/

import codeanticode.syphon.*;

// import TSPS
import tsps.*;
TSPS tspsReceiver;

// Syphon
PGraphics canvas;
SyphonClient client;

void setup(){
  size(1024,768, P3D);
	
  //all you need to do to start TSPS
  tspsReceiver= new TSPS(this, 12000);
  
  // connect to TSPS Syphon sever
  client = new SyphonClient(this, "openTSPS");
};

void draw(){
  background(0);
  
  // draw syphon frame if there is one
  if (client.available()) {
    canvas = client.getGraphics(canvas);
  }
  
if ( canvas != null ){
    tint(150);
    image(canvas, 0, 0, width, height); 
  }
  
  fill(255);
  // get array of people
  TSPSPerson[] people = tspsReceiver.getPeopleArray();
  
  // loop through people
  for (int i=0; i<people.length; i++){
      // draw person!
      people[i].draw();
  };
};
