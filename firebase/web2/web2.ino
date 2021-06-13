#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = " Cafe Hoang Mai";
const char* password = "12345678";

int t = 23;
int h = 45;

int led=0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long previousMillis = 0;    // will store last time DHT was updated

const long interval = 1000;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>ESP8266 DHT Server</h2>
  <p>Temperature</p>
  <p id="temperature">%TEMPERATURE%</p>
  <p>Humidity</p>
  <p id="humidity">%HUMIDITY%</p>
  <button onclick="myFunction()">LED ON</button>
  <p id="test">%TEST%</p>
</body>
<script>
setInterval(function ( )
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if (this.readyState == 4 && this.status == 200)
    {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( )
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if (this.readyState == 4 && this.status == 200)
    {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;


function myFunction()
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if (this.readyState == 4 && this.status == 200)
    {
      document.getElementById("test").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/test", true);
  xhttp.send();
}
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var)
{
  Serial.println(var);
  if (var == "TEMPERATURE")
  {
    return String(t);
  }
  else if (var == "HUMIDITY")
  {
    return String(h);
  }
  else if (var == "TEST")
  {
    return String(h);
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/test", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Start server
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    t++;
    h++;
    Serial.println(t);
    Serial.println(h);
  }
}
