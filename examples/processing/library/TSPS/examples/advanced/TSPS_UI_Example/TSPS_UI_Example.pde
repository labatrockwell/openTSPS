/*
  TSPS UI Example
  by Brett Renfer, 2010
  
  This shows how to do some of your own tracking with TSPSPerson
  objects, how to map images to people, and some other stuff. 
  Not recommended as a starting point!
*/

import tsps.*;

TSPS tspsReceiver;

//width of active area ( used to scale bounding boxes )
float  	activeWidth;
float  	activeHeight;

UI ui;

//custom people objects
ArrayList customPeople;
String	backgrounds[] = new String[20];


int 	currentBackground = 0;

void setup(){
	size(1024,768);
	activeWidth = 1024;
	activeHeight = 700; //to accomadate for status bar
	for (int i=0; i<20; i++){
		backgrounds[i] = "backgrounds/background_"+(i+1)+".png";
	}
	
	//functions for drawing UI elements, etc
	ui = new UI( this );	
	customPeople = new ArrayList();
	
	//all you need to do to start TSPS
	tspsReceiver= new TSPS(this, 12000);
};

void draw(){
	ui.draw();
	
	//loop through custom person objects
	for (int i=customPeople.size()-1; i>=0; i--){
		CustomPerson p = (CustomPerson) customPeople.get(i);
		if (!p.dead) p.draw();
		else customPeople.remove(i);
	}
};

void personEntered( TSPSPerson p ){
	CustomPerson person = new CustomPerson(p,activeWidth, activeHeight);
	person.loadBackground( backgrounds[currentBackground]);
	
	currentBackground++;
	if (currentBackground >= 20) currentBackground = 0;
	customPeople.add(person);
	ui.personEntered();
}

void personUpdated( TSPSPerson p ){
	for (int i=0; i<customPeople.size(); i++){
		CustomPerson lookupPerson = (CustomPerson) customPeople.get(i);
		if (p.id == lookupPerson.id){
			lookupPerson.update(p);
			break;
		}
	}
	
	ui.personUpdated();
}

void personLeft( TSPSPerson p ){
  println("person left with id " + p.id);
	for (int i=0; i<customPeople.size(); i++){
		CustomPerson lookupPerson = (CustomPerson) customPeople.get(i);
		if (p.id == lookupPerson.id){
			//lookupPerson.update(p);
                        lookupPerson.dead = true;
                        //customPeople.remove(i);
			break;
		}
	}
	
	ui.personLeft();
}




