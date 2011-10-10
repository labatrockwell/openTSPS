TSPS = function(){
	this.latestPeople 	= [];
	this.people			= {};
}

TSPS.prototype.newPerson		= function(json)
{
	if (json.type == "TSPS/personEntered") this.onPersonEntered(json);
	else if (json.type == "TSPS/personMoved") this.onPersonUpdated(json);
	else if (json.type == "TSPS/personUpdated") this.onPersonMoved(json);
	else if (json.type == "TSPS/personWillLeave") this.onPersonLeft(json);
	else console.log(json.type);
}

// deprecated (already!)

TSPS.prototype.parseMessage		= function(msg)
{
	this.latestPeople = [];			
	
	// parse message
	var allMessages = msg.data.split("\\");
	for (var h=0; h<allMessages.length; h++){
		var TSPSObj = {};
		
		var dataArray = allMessages[h].split(";");
		if (dataArray.length > 0){
			TSPSObj.type = dataArray[0];
			for (var i=1; i<dataArray.length; i++){
				var dObj = dataArray[i].split("/");
				if (dObj.length < 2){

				} else if (dObj[1].indexOf(",") != -1){
					var unpked = dObj[1].split(",");
					TSPSObj[dObj[0]] = [];
					for (var j=0; j<unpked.length; j++){
						TSPSObj[dObj[0]][j] = {};
						var obj = unpked[j].split(":");
						for (var k=0; k<obj.length; k++){
							var point = obj[k].split(">");
							TSPSObj[dObj[0]][j][point[0]] = point[1];								
						}
					}
				} else if (dObj[1].indexOf(":") != -1){
					var unpked = dObj[1].split(":");
					TSPSObj[dObj[0]] = {};

					for (var j=0; j<unpked.length; j++){
						var split = unpked[j].split(">");
						TSPSObj[dObj[0]][split[0]] = split[1];
					}						
				} else {
					TSPSObj[dObj[0]] = dObj[1];						
				}
			}
			
			if (TSPSObj.type == "TSPS/personEntered/") this.onPersonEntered(TSPSObj);
			else if (TSPSObj.type == "TSPS/personMoved/") this.onPersonUpdated(TSPSObj);
			else if (TSPSObj.type == "TSPS/personUpdated/") this.onPersonMoved(TSPSObj);
			else if (TSPSObj.type == "TSPS/personWillLeave/") this.onPersonLeft(TSPSObj);
		}
		
		this.latestPeople[this.latestPeople.length] = TSPSObj;
		//console.log(TSPSObj);
	}
	
	canvas.width = canvas.width;
	ctx.fillStyle = 'rgba(0,0,0,0)';
				
	return this.latestPeople;
}

TSPS.prototype.onPersonEntered	= function(tspsObj){
	this.people[tspsObj.id] = tspsObj;
}

TSPS.prototype.onPersonUpdated	= function(tspsObj){
	this.people[tspsObj.id] = tspsObj;
}

TSPS.prototype.onPersonMoved	= function(tspsObj){
	this.people[tspsObj.id] = tspsObj;
}

TSPS.prototype.onPersonLeft 	= function(tspsObj){
	delete this.people[tspsObj.id];
}