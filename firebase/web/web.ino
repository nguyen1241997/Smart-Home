#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = " Cafe Hoang Mai";
const char* password = "12345678";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

int temp = 23;
int humi = 45;
int gas = 1000;

int web_rc[5];

unsigned long previousMillis = 0;    // will store last time DHT was updated

const long interval = 1000;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 1.2rem;}
    p {font-size: 1.2rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 10px;}
    .switch {position: relative; display: inline-block; width: 90px; height: 50px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 40px; width: 40px; left: 5px; bottom: 5px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 2px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(40px); -ms-transform: translateX(40px); transform: translateX(40px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER%
  <p>Temperature</p>
  <p id="temperature">%TEMPERATURE%</p>
  <p>Humidity</p>
  <p id="humidity">%HUMIDITY%</p>
  <p>Gas sensor</p>
  <p id="gas">%GAS%</p>
<script>
  function toggleCheckbox(element)
  {
    var xhr = new XMLHttpRequest();
    if(element.checked)
    {
      xhr.open("GET", "/update?output="+element.id+"&state=1", true);
    }
    else
    {
      xhr.open("GET", "/update?output="+element.id+"&state=0", true);
    }
    xhr.send();
  }

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

setInterval(function ( )
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if (this.readyState == 4 && this.status == 200)
    {
      document.getElementById("gas").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/gas", true);
  xhttp.send();
}, 1000 ) ;

</script>
</body>
</html>
)rawliteral";


// Replaces placeholder with button section in your web page
String processor(const String& var)
{
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"0\" " + outputState(0) + "><span class=\"slider\"></span></label>";
    buttons += "<br><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"1\" " + outputState(1) + "><span class=\"slider\"></span></label>";
    buttons += "<br><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<br><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"3\" " + outputState(3) + "><span class=\"slider\"></span></label>";
    buttons += "<br><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  else if (var == "TEMPERATURE")
  {
    return String(temp);
  }
  else if (var == "HUMIDITY")
  {
    return String(humi);
  }
  else if (var == "GAS")
  {
    return String(gas);
  }
  return String();
}

String outputState(int output)
{
  if(0)
  {
    return "checked";
  }
  else
  {
    return "";
  }
}

void setup()
{
  
  // Serial port for debugging purposes
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2))
    {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      //digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
      int a=inputMessage1.toInt();
      int b=inputMessage2.toInt();
      web_rc[a]=b;
      if (b)
      {
        //mySerial.write(char('A'+a));
        Serial.println(char('A'+a));
      }
      else
      {
        //mySerial.write(char('a'+a));
        Serial.println(char('a'+a));
      }
    }
    else
    {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", String(temp).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", String(humi).c_str());
  });
    server.on("/gas", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", String(gas).c_str());
  });

  // Start server
  server.begin();
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    temp++;
    humi++;
    gas++;
  }
}
