#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

SoftwareSerial mySerial(2,16); // RX, TX, D0-PA3, D4

const String wifiName = " Cafe Hoang Mai";
const String password = "12345678";

int temp=0, humi=0, gas=0;
String _temp, _humi, _gas;

String inputString = "";
String inputString2 = "";

String bf2[15], bf1[15];
String rc[15], rc1[15];

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";
int web_rc[5];

void send_firebase(void);
void receive_uart(void);
void Get_data_Firebase2(void);

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
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.print("Connecting");
  WiFi.begin(wifiName, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected");
  
  Firebase.begin("smart-home-a4a54.firebaseio.com", "M5Jax2jJQnVnnWPeJegIrJtWhMbZwtI0wts0ihKy");

  int i;
  for(i=0;i<15;i++)
  {
    bf2[i]="0";
    bf1[i]="0";
    rc[i]="0";
    rc1[i]="0";
  }

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
        mySerial.write(char('A'+a));
        Serial.println(char('A'+a));
      }
      else
      {
        mySerial.write(char('a'+a));
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
  inputString = "";
  inputString2 = "";
  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();
    inputString2 += inChar;
  }

  receive_uart();
  send_firebase();
  Get_data_Firebase2();
  
  Serial.println("..........");
  delay(10);
}



void receive_uart(void)
{
  int m;
  m=inputString2.lastIndexOf('_');
  inputString = inputString2.substring(m-26,m);

  Serial.print("Data received: ");
  Serial.println(inputString);
  Serial.println(inputString2);

  
  //temparature
  int a1 = -1, a2 = -1;
  a1 = inputString.indexOf('#');
  a2 = inputString.indexOf('$');
  if (a1 >= 0 && a2 >= 0)
  {
    _temp = "";
    _temp = inputString.substring(a1 + 1, a2);
    temp = _temp.toInt();
    Serial.println(temp);
  }

  //humidity
  int b1 = -1, b2 = -1;
  b1 = inputString.indexOf('^');
  b2 = inputString.indexOf('*');
  if (b1 >= 0 && b2 >= 0)
  {
    _humi = "";
    _humi = inputString.substring(b1 + 1, b2);
    humi = _humi.toInt();
    Serial.println(humi);
  }

  int c1 = -1, c2 = -1;
  c1 = inputString.indexOf('!');
  c2 = inputString.indexOf('@');
  if (c1 >= 0 && c2 >= 0)
  {
    _gas = "";
    _gas = inputString.substring(c1 + 1, c2);
    gas = _gas.toInt();
    Serial.println(gas);
  }

  Firebase.setInt("temparature", temp);
  Firebase.setInt("humidity", humi);
  Firebase.setInt("gas_sensor", gas);
}

void send_firebase(void)
{
  if (inputString.indexOf('A') >= 0)
  {
    rc1[0]="1";
  }
  else if (inputString.indexOf('a') >= 0)
  {
    rc1[0]="0";
  }

  if (inputString.indexOf('B') >= 0)
  {
    rc1[1]="1";
  }
  else if (inputString.indexOf('b') >= 0)
  {
    rc1[1]="0";
  }

  if (inputString.indexOf('C') >= 0)
  {
    rc1[2]="1";
  }
  else if (inputString.indexOf('c') >= 0)
  {
    rc1[2]="0";
  }

  if (inputString.indexOf('D') >= 0)
  {
    rc1[3]="1";
  }
  else if (inputString.indexOf('d') >= 0)
  {
    rc1[3]="0";
  }

  if (inputString.indexOf('E') >= 0)
  {
    rc1[4]="1";
  }
  else if (inputString.indexOf('e') >= 0)
  {
    rc1[4]="0";
  }

  if (inputString.indexOf('F') >= 0)
  {
    rc1[5]="1";
  }
  else if (inputString.indexOf('f') >= 0)
  {
    rc1[5]="0";
  }

  if (inputString.indexOf('G') >= 0)
  {
    rc1[6]="1";
  }
  else if (inputString.indexOf('g') >= 0)
  {
    rc1[6]="0";
  }

  if (inputString.indexOf('H') >= 0)
  {
    rc1[7]="1";
  }
  else if (inputString.indexOf('h') >= 0)
  {
    rc1[7]="0";
  }

  if (inputString.indexOf('I') >= 0)
  {
    rc1[8]="1";
  }
  else if (inputString.indexOf('i') >= 0)
  {
    rc1[8]="0";
  }

  if (inputString.indexOf('J') >= 0)
  {
    rc1[9]="1";
  }
  else if (inputString.indexOf('j') >= 0)
  {
    rc1[9]="0";
  }

  if (inputString.indexOf('K') >= 0)
  {
    rc1[10]="1";
  }
  else if (inputString.indexOf('k') >= 0)
  {
    rc1[10]="0";
  }

  if (inputString.indexOf('L') >= 0)
  {
    rc1[11]="1";
  }
  else if (inputString.indexOf('l') >= 0)
  {
    rc1[11]="0";
  }

  if (inputString.indexOf('M') >= 0)
  {
    rc1[12]="1";
  }
  else if (inputString.indexOf('m') >= 0)
  {
    rc1[12]="0";
  }

  //send data to firebase
  if (rc1[0] != bf1[0])
  {
    bf1[0] = rc1[0];
    if(rc1[0]=="1")
    {
      Firebase.setInt("state_led_LVR", 1);
    }
    else
    {
      Firebase.setInt("state_led_LVR", 0);
    }
  }

  if (rc1[1] != bf1[1])
  {
    bf1[1] = rc1[1];
    if(rc1[1]=="1")
    {
      Firebase.setInt("state_led_KCR", 1);
    }
    else
    {
      Firebase.setInt("state_led_KCR", 0);
    }
  }

  if (rc1[2] != bf1[2])
  {
    bf1[2] = rc1[2];
    if(rc1[2]=="1")
    {
      Firebase.setInt("state_led_BedR", 1);
    }
    else
    {
      Firebase.setInt("state_led_BedR", 0);
    }
  }

  if (rc1[3] != bf1[3])
  {
    bf1[3] = rc1[3];
    if(rc1[3]=="1")
    {
      Firebase.setInt("state_led_BaR", 1);
    }
    else
    {
      Firebase.setInt("state_led_BaR", 0);
    }
  }

  if (rc1[4] != bf1[4])
  {
    bf1[4] = rc1[4];
    if(rc1[4]=="1")
    {
      Firebase.setInt("state_led_garage", 1);
    }
    else
    {
      Firebase.setInt("state_led_garage", 0);
    }
  }
/*
  if (rc1[5] != bf1[5])
  {
    bf1[5] = rc1[5];
    if(rc1[5]=="1")
    {
      Firebase.setInt("state_fan_LVR", 1);
    }
    else
    {
      Firebase.setInt("state_fan_LVR", 0);
    }
  }

  if (rc1[6] != bf1[6])
  {
    bf1[6] = rc1[6];
    if(rc1[6]=="1")
    {
      Firebase.setInt("state_fan_KCR", 1);
    }
    else
    {
      Firebase.setInt("state_fan_KCR", 0);
    }
  }

  if (rc1[7] != bf1[7])
  {
    bf1[7] = rc1[7];
    if(rc1[7]=="1")
    {
      Firebase.setInt("state_fan_BedR", 1);
    }
    else
    {
      Firebase.setInt("state_fan_BedR", 0);
    }
  }

  if (rc1[8] != bf1[8])
  {
    bf1[8] = rc1[8];
    if(rc1[8]=="1")
    {
      Firebase.setInt("state_door_garage", 1);
    }
    else
    {
      Firebase.setInt("state_door_garage", 0);
    }
  }
*/
  if (rc1[9] != bf1[9])
  {
    bf1[9] = rc1[9];
    if(rc1[9]=="1")
    {
      Firebase.setInt("state_clothes", 1);
    }
    else
    {
      Firebase.setInt("state_clothes", 0);
      Firebase.setString("control_clothes","0");
    }
  }

  if (rc1[10] != bf1[10])
  {
    bf1[10] = rc1[10];
    if(rc1[10]=="1")
    {
      Firebase.setInt("state_window", 1);
      Firebase.setString("control_window","1");
    }
    else
    {
      Firebase.setInt("state_window", 0);
    }
  }

  if (rc1[11] != bf1[11])
  {
    bf1[11] = rc1[11];
    if(rc1[11]=="1")
    {
      Firebase.setInt("state_gas", 1);
    }
    else
    {
      Firebase.setInt("state_gas", 0);
    }
  }

  if (rc1[12] != bf1[12])
  {
    bf1[12] = rc1[12];
    if(rc1[12]=="1")
    {
      Firebase.setInt("state_fire", 1);
    }
    else
    {
      Firebase.setInt("state_fire", 0);
    }
  }
}

void Get_data_Firebase2(void)
{
  bf2[0]=Firebase.getString("control_led_LVR");
  if (bf2[0]!=bf1[0])
  {
    if(bf2[0]=="1") mySerial.write('A');
    else mySerial.write('a');
  }
  int x=1;

  bf2[1]=Firebase.getString("control_led_KCR");
  if (bf2[1]!=bf1[1])
  {
    if(bf2[1]=="1") mySerial.write('B');
    else mySerial.write('b');
  }
  
  bf2[2]=Firebase.getString("control_led_BedR");
  if (bf2[2]!=bf1[2])
  {
    if(bf2[2]=="1") mySerial.write('C');
    else mySerial.write('c');
  }

  bf2[3]=Firebase.getString("control_led_BaR");
  if (bf2[3]!=bf1[3])
  {
    if(bf2[3]=="1") mySerial.write('D');
    else mySerial.write('d');
  }
  
  bf2[4]=Firebase.getString("control_led_garage");
  if (bf2[4]!=bf1[4])
  {
    if(bf2[4]=="1") mySerial.write('E');
    else mySerial.write('e');
  }
/*
  bf2[5]=Firebase.getString("control_fan_LVR");
  if (bf2[5]!=bf1[5])
  {
    if(bf2[5]=="1") mySerial.write('F');
    else mySerial.write('f');
  }
  
  bf2[6]=Firebase.getString("control_fan_LVR");
  if (bf2[6]!=bf1[6])
  {
    if(bf2[6]=="1") mySerial.write('G');
    else mySerial.write('g');
  }

  bf2[7]=Firebase.getString("control_fan_BedR");
  if (bf2[7]!=bf1[7])
  {
    if(bf2[7]=="1") mySerial.write('H');
    else mySerial.write('h');
  }
  
  bf2[8]=Firebase.getString("control_door_garage");
  if (bf2[8]!=bf1[8])
  {
    if(bf2[8]=="1") mySerial.write('I');
    else mySerial.write('i');
  }
*/
  bf2[9]=Firebase.getString("control_clothes");
  if (bf2[9]!=bf1[9])
  {
    if(bf2[9]=="1") mySerial.write('J');
    else mySerial.write('j');
  }

  bf2[10]=Firebase.getString("control_window");
  if (bf2[10]!=bf1[10])
  {
    if(bf2[10]=="1") mySerial.write('K');
    else mySerial.write('k');
  }
}
