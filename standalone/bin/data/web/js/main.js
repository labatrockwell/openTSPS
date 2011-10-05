/* tsps protocol */

	var socket;
	var outputDiv;
	var TSPSObjects = [];
	var canvas;
	var ctx;
	
	var vid_width		= 640;
	var vid_height		= 480;
	
	var tsps 			= new TSPS();
	
	$(document).ready( function() {
		outputDiv 	= document.getElementById('output');
		canvas 		= document.getElementById('contourCanvas');
		canvas.width = vid_width;
		canvas.height = vid_height;
		ctx			= canvas.getContext('2d');
		ctx.fillStyle = 'rgba(0,0,0,0)';
		setupSocket();
		
		document.getElementById("brow").textContent = " " + BrowserDetect.browser + " "
			+ BrowserDetect.version +" " + BrowserDetect.OS +" ";
		
	});
	
	function render(){
		// draw contours
		for (var id in tsps.people){
			var person = tsps.people[id];

			// draw rect
			ctx.strokeStyle = 'black';
			ctx.strokeRect(person.boundingrect.x*vid_width,person.boundingrect.y*vid_height, person.boundingrect.x*vid_width+person.boundingrect.width*vid_width, person.boundingrect.y*vid_height+person.boundingrect.height*vid_height);
			ctx.stroke();

			// draw contours
			ctx.strokeStyle = '#ff0000';
			ctx.beginPath();
			ctx.moveTo(person.contours[0].x*vid_width,person.contours[0].y*vid_height);

			for (var j=1; j<person.contours.length; j++ ){
				ctx.lineTo( person.contours[j].x*vid_width,person.contours[j].y*vid_height );
			}				
			ctx.stroke();
		}
	}
	
	function setupSocket(){
		if (BrowserDetect.browser == "Firefox") {
			socket = new MozWebSocket(get_appropriate_ws_url(),
					   "tsps-protocol");
		} else {
			socket = new WebSocket(get_appropriate_ws_url(),
					   "tsps-protocol");
		}
		
		try {
			socket.onopen = function() {
				document.getElementById("wslm_statustd").style.backgroundColor = "#40ff40";
				document.getElementById("wslm_statustd").textContent = " websocket connection opened ";
			} 

			socket.onmessage =function got_packet(msg) {
				var TSPSPeople = tsps.parseMessage(msg);
				render();
			}

			socket.onclose = function(){
				document.getElementById("wslm_statustd").style.backgroundColor = "#ff4040";
				document.getElementById("wslm_statustd").textContent = " websocket connection CLOSED ";
			}
		} catch(exception) {
			alert('<p>Error' + exception);  
		}
	}