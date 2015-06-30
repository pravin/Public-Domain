/*
 * Twitter Timeline Javascript v0.2 (March 20, 2007)
 * Pravin Paratey (http://www.dustyant.com)
 * Andrei Virlan  (http://its.squeak.in)
 *
 * Released under Creative Commons Attribution 2.5 Licence
 * http://creativecommons.org/licenses/by/2.5/
 *
 * Changelog:
 * 0.2 (Mar 20, 2007) - Moved to PlotKit to draw graphs
 * 0.1 (Feb 23, 2007) - Initial Release
 */

var userid = '754023'; // Change this value to your user-id

var timelineArray = new Array();
function drawGraph() {
    var layout = new PlotKit.Layout("bar", {});
    layout.addDataset("sqrt", timelineArray);
    layout.evaluate();
    var canvas = MochiKit.DOM.getElement("graph");
    var plotter = new PlotKit.SweetCanvasRenderer(canvas, layout, {});
    plotter.render();
}

function twitterCallback(obj) {
	// Create an array to hold the 24 hours of the day
	var hourArray = new Array(24);
	
	// Initialize array
	for(var i=0; i<24; i++) {
		hourArray[i] = 0;
	}
	
	for (var i=0; i<obj.length; i++) {
		// Get date
		var created_at = new Date(obj[i].created_at);
		if(obj[i].user.id == userid) {
			// Increment the hour 
			hourArray[created_at.getHours()]++;
		}
	}
	
	// Construct the timeline
	for(var i=0; i<24; i++) {
		timelineArray[i] = new Array(i, hourArray[i]);
	}
	MochiKit.DOM.addLoadEvent(drawGraph);
}
// Makes the twitter call
document.write('<scr'+'ipt type="text/javascript"' +
	'src="http://twitter.com/statuses/friends_timeline/' + 
	userid + '.json?callback=twitterCallback"></scr'+'ipt>');