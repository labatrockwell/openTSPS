//very basic supercollider 3.7 example for opentsps. f.olofsson, 2016

/*
OSCFunc.trace(true);  //debug on
OSCFunc.trace(false);  //debug off
*/


//sonifying number of people in the scene
(
s.latency= 0.05;
s.waitForBoot{
	a.release;
	a= {|ppl= 0| SinOsc.ar((ppl*2+60).midicps)*(ppl>0).lag(0.5)!2}.play;
	OSCdef(\scene, {|msg|
		msg.postln;  //debug
		a.set(\ppl, msg[2]);
	}, '/TSPS/scene/', recvPort:12000);
};
)
//stop with cmd+. (or alt+. on windows)
