// Get current sensor readings when the page loads
window.addEventListener('load', getReadings);

// set timezone if wrong
Highcharts.setOptions({
  time: {
    timezoneOffset: -120 //Add your time zone offset here in seconds
  }
});

// GAUGES -------------------------------------------------------
// Create Temperature Gauge
var gaugeTemp = new LinearGauge({
  renderTo: 'gauge-temperature',
  width: 120,
  height: 400,
  units: "Temperature C",
  minValue: 0,
  startAngle: 90,
  ticksAngle: 180,
  maxValue: 40,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 2,
  valueInt: 2,
  majorTicks: [
      "0",
      "5",
      "10",
      "15",
      "20",
      "25",
      "30",
      "35",
      "40"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 30,
          "to": 40,
          "color": "rgba(200, 50, 50, .75)"
      }
  ],
  colorPlate: "#fff",
  colorBarProgress: "#CC2936",
  colorBarProgressEnd: "#049faa",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear",
  barWidth: 10,
}).draw();
  
// Create Humidity Gauge
var gaugeHum = new RadialGauge({
  renderTo: 'gauge-humidity',
  width: 300,
  height: 300,
  units: "Humidity (%)",
  minValue: 0,
  maxValue: 100,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 2,
  majorTicks: [
      "0",
      "20",
      "40",
      "60",
      "80",
      "100"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 80,
          "to": 100,
          "color": "#03C0C1"
      }
  ],
  colorPlate: "#fff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "line",
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

// Create Pressure Gauge
var gaugePress = new RadialGauge({
  renderTo: 'gauge-pressure',
  width: 300,
  height: 300,
  units: "Pressure (hPa)",
  minValue: 0,
  maxValue: 2000,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 2,
  majorTicks: [
      "0",
      "200",
      "400",
      "600",
      "800",
      "1000",
      "1200",
      "1400",
      "1600",
      "1800",
      "2000"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 1025,
          "to": 2000,
          "color": "#FF7433",
      }
  ],
  colorPlate: "#fff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "line",
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();
// END GAUGES ---------------------------------------------------

// CHARTS -------------------------------------------------------
// Create Temperature Chart
var chartT = new Highcharts.Chart({
  chart:{
    renderTo:'chart-temperature'
  },
  series: [
    {
      name: 'Temperature AM2320',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Temperature BMP388',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 3,
        fillColor: '#00A6A6',
      }
    },
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Temperature Degrees Celsius'
    }
  },
  credits: {
    enabled: false
  }
});

// Create Humidity Chart
var chartH = new Highcharts.Chart({
  chart:{
    renderTo:'chart-humidity'
  },
  series: [
    {
      name: 'Humidity AM2320',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Humidity %'
    }
  },
  credits: {
    enabled: false
  }
});

// Create Pressure Chart
var chartP = new Highcharts.Chart({
  chart:{
    renderTo:'chart-pressure'
  },
  series: [
    {
      name: 'Pressure BMP388',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    },
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Pressure hPa'
    }
  },
  credits: {
    enabled: false
  }
});
// END CHARTS ---------------------------------------------------

// PLOT FUNCTIONS -----------------------------------------------
//Plot temperature in the temperature chart
function plotTemperature(jsonValue) {

  var keys = Object.keys(jsonValue);
  console.log(keys);
  console.log(keys.length);

  // get time
  var t = (new Date()).getTime();
  console.log(t);
  var counter = 11;				// counter for time delay
  var delayKey = keys[48];			// grab delayKey
  var delay = Number(jsonValue[delayKey]); 	// grab delay

  // we know that am2320temp array is place 0 to 11 (inclusive)
  for (var i = 0; i < 12; i++){
    // adjust time
    x = t - (counter * delay);
    const key = keys[i];
    var y = Number(jsonValue[key]);
    console.log(y);

  // if more than 11 datapoints, shifts of the leftmost and add rightmost
  // else just add measurement point
    if(chartT.series[0].data.length > 11) {
      chartT.series[0].addPoint([x, y], true, true, true);
    } else {
      chartT.series[0].addPoint([x, y], true, false, true);
    }
    counter = counter - 1;
  }

  counter = 11;				// reset counter for time delay
  // we know that bmp388temp array is place 12 to 23 (inclusive)
  for (var i = 12; i < 24; i++){
    // adjust time
    x = t - (counter * delay);
    const key = keys[i];
    var y = Number(jsonValue[key]);
    console.log(y);

  // if more than 11 datapoints, shifts of the leftmost and add rightmost
  // else just add measurement point
    if(chartT.series[1].data.length > 11) {
      chartT.series[1].addPoint([x, y], true, true, true);
    } else {
      chartT.series[1].addPoint([x, y], true, false, true);
    }
    counter = counter - 1;
  }
}

//Plot humidity in the humidity chart
function plotHumidity(jsonValue) {

  var keys = Object.keys(jsonValue);
  console.log(keys);
  console.log(keys.length);
  
  // get time
  var t = (new Date()).getTime();
  console.log(t);
  var counter = 11;				// counter for time delay
  var delayKey = keys[48];			// grab delayKey
  var delay = Number(jsonValue[delayKey]); 	// grab delay
	
  // we know that am2320humid array is place 24 to 35 (inclusive)
  for (var i = 24; i < 36; i++){
    // adjust time
    x = t - (counter * delay);
    const key = keys[i];
    var y = Number(jsonValue[key]);
    console.log(y);

  // if more than 11 datapoints, shifts of the leftmost and add rightmost
  // else just add measurement point
    if(chartH.series[0].data.length > 11) {
      chartH.series[0].addPoint([x, y], true, true, true);
    } else {
      chartH.series[0].addPoint([x, y], true, false, true);
    }
    counter = counter - 1;
  }

}


//Plot pressure in the pressure chart
function plotPressure(jsonValue) {

  var keys = Object.keys(jsonValue);
  console.log(keys);
  console.log(keys.length);

  // get time
  var t = (new Date()).getTime();
  console.log(t);
  var counter = 11;				// counter for time delay
  var delayKey = keys[48];			// grab delayKey
  var delay = Number(jsonValue[delayKey]); 	// grab delay

  // we know that bmp388pressure array is place 36 to 47 (inclusive)
  for (var i = 36; i < 48; i++){
    // adjust time
    x = t - (counter * delay);
    const key = keys[i];
    var y = Number(jsonValue[key]);
    console.log(y);
    console.log(x)
  // if more than 11 datapoints, shifts of the leftmost and add rightmost
  // else just add measurement point
    if(chartP.series[0].data.length > 11) {
      chartP.series[0].addPoint([x, y], true, true, true);
    } else {
      chartP.series[0].addPoint([x, y], true, false, true);
    }
    counter = counter - 1;
  }
}
// END PLOT FUNCTIONS -------------------------------------------

// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      console.log(this.responseText);	
      // GET DATA AND PARSE
      var myObj = JSON.parse(this.responseText);	// parse response
      console.log(myObj);				// log
      // GAUGES
      var temp = myObj.AM2320temp11;			// take temp var
      var hum = myObj.AM2320hum11;			// take humid var
      var press = myObj.BMP388press11;			// take pressure var
      gaugeTemp.value = temp;				// put temp in gauge
      gaugeHum.value = hum;				// put humid in gauge
      gaugePress.value = press;				// put pressure in gauge	
      // CHARTS
      plotTemperature(myObj);
      plotHumidity(myObj);
      plotPressure(myObj);

    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    // CHARTS
    plotTemperature(myObj);
    plotHumidity(myObj);
    plotPressure(myObj);
    // GAUGES
    gaugeTemp.value = myObj.AM2320temp11;
    gaugeHum.value = myObj.AM2320hum11;
    gaugePress.value = myObj.BMP388press11;
  }, false);
}