class UI
{
	PApplet parent;
	PFont	timesBold18;
	PImage 	statusBar;
	PImage 	numberOfPeople;
	PImage 	personEnteredNotice;
	PImage 	personUpdatedNotice;
	PImage	personLeftNotice;
	
	int		bPersonEntered;
	int  	bPersonUpdated;
	int		bPersonLeft;
	
	UI( PApplet _parent ){
		parent = _parent;
		hint(ENABLE_NATIVE_FONTS);
		timesBold18 = loadFont("TimesNewRomanPS-BoldItalicMT-18.vlw");
		textFont(timesBold18, 18);
		smooth();
		noStroke();
		
		bPersonEntered = bPersonUpdated = bPersonLeft = 0;
		
		loadImages();
		drawBackground();
	}
		
	void draw(){
		drawBackground();
		if (bPersonEntered > 0){
			image(personEnteredNotice, 397,728);
			bPersonEntered--;
			if (bPersonEntered < 0) bPersonEntered = 0;
		}
		
		if (bPersonUpdated > 0){
			image(personUpdatedNotice, 533,728);
			bPersonUpdated--;
			if (bPersonUpdated < 0) bPersonUpdated = 0;
		}
		
		if (bPersonLeft > 0){
			image(personLeftNotice, 666,728);
			bPersonLeft--;
			if (bPersonLeft < 0) bPersonLeft = 0;
		}
		
		//number of people
		fill(43, 150,152);
		textFont(timesBold18, 18);
		text(Integer.toString(customPeople.size()), 347, 740);
		fill(255);
	}
	
	void personEntered(){
		bPersonEntered = 5;
	};
	
	void personUpdated(){
		bPersonUpdated = 5;		
	};
	
	void personLeft(){
		bPersonLeft = 5;		
	};
	
	void drawBackground(){
		background(148,129,85);	
		image (statusBar, 0, 700);
	}

	void loadImages(){
		statusBar = loadImage("bottomBar.png");
		personEnteredNotice = loadImage( "triggers/PersonEntered_Active.png" );
		personUpdatedNotice = loadImage( "triggers/PersonUpdated_Active.png" );
		personLeftNotice 	= loadImage( "triggers/PersonLeft_Active.png" );
	}
}