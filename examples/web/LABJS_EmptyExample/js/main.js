var app;

$(document).ready( function() {
	DemoApp.prototype = $.extend(true, LAB.app.BaseApp.prototype, TSPS.Connection.prototype, DemoApp.prototype);
	app 	= new DemoApp();
	app.begin();
});

// ===========================================
// ===== DEMO APP
// ===========================================

	DemoApp = function(){
		LAB.app.BaseApp.call( this );
		TSPS.Connection.call( this );

		//-------------------------------------------------------
		this.setup = function (){
			this.host = LAB.getQueryString("host", "localhost");
			this.port = LAB.getQueryString("port", 7681);

			console.log( this.host )
			this.connect();
		}

		//-------------------------------------------------------
		this.update = function (){
		}

		//-------------------------------------------------------
		this.draw = function (){
		}	

		//-------------------------------------------------------
		this.onPersonEntered = function( person ){};
	
		//-------------------------------------------------------
		this.onPersonUpdated = function( person ){};

		//-------------------------------------------------------
		this.onPersonMoved = function( person ){};
	
		//-------------------------------------------------------
		this.onPersonLeft = function( person ){};

		//-------------------------------------------------------
		this.onSceneUpdate = function( scene ){};

		//-------------------------------------------------------
		this.onCustomEvent = function( name, data ){};

		//-------------------------------------------------------
		this.onMouseMoved = function( x,y ){
			
		};

		//-------------------------------------------------------
		this.onMousePressed = function( x,y ){
			
		};

		//-------------------------------------------------------
		this.onMouseDragged = function( x,y ){
			
		};

		//-------------------------------------------------------
		this.onMouseReleased = function( x,y ){
			
		};

		//-------------------------------------------------------
		this.onDocumentKeyDown = function( key ){
			
		};
	}
	
