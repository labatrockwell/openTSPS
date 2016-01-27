//more advanced supercollider 3.7 example for opentsps. f.olofsson, 2016

//sonifying individual people (in the x dimension)
(
p= ();  //dictionary with person data
s.latency= 0.05;
s.waitForBoot{
	OSCdef(\personEntered, {|msg|
		var pid= msg[1];
		msg.postln;  //debug
		("new person detected!"+msg).postln;
		p.put(pid, {|age= 0, cx= 0.5, dx= 0|
				var freq= pid%8*100+200;  //8 ppl in cycle
				var feed= (age*0.005).min(0);
				var pan= cx.linlin(0.35, 0.65, -1, 1);
				var mod= dx.linlin(-10, 10, -1, 1);
				Pan2.ar(SinOscFB.ar(freq+mod, feed+SinOsc.ar(mod)), pan);
			}.play(fadeTime:3);  //fade in time
		);
	}, '/TSPS/personEntered/', recvPort:12000);
	OSCdef(\personUpdated, {|msg|
		var pid= msg[1];
		var age= msg[3];
		var centx= msg[4];
		var velox= msg[6];
		msg.postln;  //debug
		if(p[pid].notNil, {
			p[pid].set(\age, age, \cx, centx, \dx, velox);
		}, {
			("person not detected - skipping"+msg).postln;
		});
	}, '/TSPS/personUpdated/', recvPort:12000);
	OSCdef(\personWillLeave, {|msg|
		var pid= msg[1];
		msg.postln;  //debug
		if(p[msg[1]].notNil, {
			p[msg[1]].release(1);  //stop synth
			p[msg[1]]= nil;  //erase person
		});
	}, '/TSPS/personWillLeave/', recvPort:12000);
};
)
