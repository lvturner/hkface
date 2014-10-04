var warningUrl = 'http://rss.weather.gov.hk/rss/WeatherWarningSummaryv2.xml';
var temperatureUrl = 'http://rss.weather.gov.hk/rss/CurrentWeather.xml';
var aqiUrl = 'http://www.aqhi.gov.hk/epd/ddata/html/out/aqhirss_Eng.xml';
var aqi = "";
var temperature = "";
var warning = "";

function fetchWeatherWarning(done) {
    console.log("Fetching weather");
    var req = new XMLHttpRequest();
    req.open('GET', warningUrl, true);
    req.onload = function(e) {
        if(req.status == 200) {
            warning = req.responseText.match(/<title><\!\[CDATA\[(.*?)]]><\/title>/)[1].trim();
            done();
        }
    };
    req.send(null);
}

function fetchCurrentWeather(done) {
    var req = new XMLHttpRequest();
    req.open('GET', temperatureUrl, true);
    req.onload = function(e) {
        if(req.status == 200) {
            temperature = req.responseText.match(/Air temperature : ([0-9]*)/)[1];
            done();
        }
    };
    req.send(null);
}

function fetchAQI() {
    var req = new XMLHttpRequest();
    req.open('GET', aqiUrl, true);
    req.onload = function(e) {
        if(req.status == 200) {
            aqi = req.responseText.match(/\(Health Risk: ([A-Z]*[a-z]*)/)[1].toLowerCase();
            console.log("AQI: " + aqi);
            console.log("TEMPERATURE: " + temperature);
            console.log("WARNING: " + warning);

            Pebble.sendAppMessage({
                "AQI": aqi,
                "TEMPERATURE": temperature + "°",
                "WARNING": warning
            });
        }
    };
    req.send(null);
}

Pebble.addEventListener("ready", function(e) {
    fetchWeatherWarning(function() {
        fetchCurrentWeather(function() {
            fetchAQI();
        });
    });
});

Pebble.addEventListener("appmessage", function(e) {
    fetchWeatherWarning(function() {
        fetchCurrentWeather(function() {
            fetchAQI();
        });
    });
});
