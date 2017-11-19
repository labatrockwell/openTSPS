/* tsps protocol */

	var socket;
	var outputDiv;
	var TSPSObjects = [];
	var canvas;
	var ctx;
	
	var vid_width		= 640;
	var vid_height		= 480;
	
	var tsps;
	
	$(document).ready( function() {
		outputDiv 	= document.getElementById('output');
		canvas 		= document.getElementById('contourCanvas');
		canvas.width = vid_width;
		canvas.height = vid_height;
		ctx			= canvas.getContext('2d');
		ctx.fillStyle = 'rgba(0,0,0,0)';

		// connect with default settings
		tsps = new TSPS.Connection();
		tsps.connect();
		
		// render each time we get a message
		tsps.onMessageReceived = render;
	});
	
	function render(){
		canvas.width = canvas.width;
		// draw contours
		for (var id in tsps.people){
			var person = tsps.people[id];

			// draw rect
			ctx.strokeStyle = 'black';
			ctx.strokeRect(person.boundingrect.x*vid_width,person.boundingrect.y*vid_height, person.boundingrect.width*vid_width, person.boundingrect.height*vid_height);
			ctx.stroke();

			// draw contours
			ctx.fillStyle = '#111111';
			ctx.font	  = "11pt Helvetica";
			ctx.fillText("id: "+person.id, person.centroid.x*vid_width, person.centroid.y*vid_height);
			ctx.fillText("age: "+person.age, person.centroid.x*vid_width, person.centroid.y*vid_height+20);
			ctx.fillText("depth: "+person.depth, person.centroid.x*vid_width, person.centroid.y*vid_height+40);
			ctx.strokeStyle = '#ff0000';
			ctx.beginPath();
			if ( person.contours.length > 0 ){
                ctx.beginPath();
                ctx.moveTo(person.contours[0].x*vid_width,person.contours[0].y*vid_height);

                for (var j=1; j<person.contours.length; j++ ){
                    ctx.lineTo( person.contours[j].x*vid_width,person.contours[j].y*vid_height );
                }               
                ctx.stroke();
            }

			for (var j=1; j<person.contours.length; j++ ){
				ctx.lineTo( person.contours[j].x*vid_width,person.contours[j].y*vid_height );
			}				
			ctx.stroke();
		}
	}