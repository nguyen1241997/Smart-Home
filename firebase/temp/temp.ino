#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

SoftwareSerial mySerial(2,16); // RX, TX, D0-PA3, D4

//const String wifiName = "nguyen124";
//const String password = "12345678";
//const String wifiName = "CanHo Truong Thinh";
//const String password = "truongthinh25";
const String wifiName = " Cafe Hoang Mai";
const String password = "12345678";

int temp=0, humi=0, gas=0;
String _temp, _humi, _gas;

String inputString = "";
String inputString2 = "";

String bf2[15];

void ham(void);
void data(void);
void Get_data_Firebase(void);

void setup() {
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

  //Firebase.begin("test-firebase-7ea12.firebaseio.com","MQxzVY549uNdHQnE8bqJCDraNCmKNjgCETrTFdfN");
  //Firebase.begin("abcd-16d4e-default-rtdb.firebaseio.com","163B4as5rapvVvdkxcOJAcnhQ4Yu7DkZiZdizqaV");
  //Firebase.setInt("LEDStatus",0);
  Firebase.begin("smart-home-a4a54.firebaseio.com", "M5Jax2jJQnVnnWPeJegIrJtWhMbZwtI0wts0ihKy");

  int i;
  for(i=0;i<15;i++)
  {
    bf2[i]="0";
  }

}

String a;
void loop()
{
  inputString = "";
  inputString2 = "";
  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();
    inputString2 += inChar;
  }

  data();
  ham();
  Get_data_Firebase();
  
  Serial.println(".");
  delay(200);
}


void test_firebase(void)
{

  a = Firebase.getString("hi");
  if (Firebase.failed())
  {
    Serial.println("Fail");
    Serial.println(Firebase.error());
  }
  else
  {
    Serial.println(a);
  }

  temp = random(20, 40);
  humi = random(50, 80);
  Serial.println(temp);
  Serial.println(humi);
  Firebase.setFloat("temp", temp);
  Firebase.setInt("humi", humi);
}

void ham(void)
{
  if (inputString.indexOf('M') >= 0)
  {
    Firebase.setInt("state_led_LVR", 1);
  }
  else if (inputString.indexOf('m') >= 0)
  {
    Firebase.setInt("state_led_LVR", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_led_KCR", 1);
  }
  else if (inputString.indexOf('n') >= 0)
  {
    Firebase.setInt("state_led_KCR", 0);
  }
  
  if (inputString.indexOf('O') >= 0)
  {
    Firebase.setInt("state_led_BedR", 1);
  }
  else if (inputString.indexOf('o') >= 0)
  {
    Firebase.setInt("state_led_BedR", 0);
  }

  if (inputString.indexOf('P') >= 0)
  {
    Firebase.setInt("state_led_BaR", 1);
  }
  else if (inputString.indexOf('p') >= 0)
  {
    Firebase.setInt("state_led_BaR", 0);
  }

  if (inputString.indexOf('Q') >= 0)
  {
    Firebase.setInt("state_led_garage", 1);
  }
  else if (inputString.indexOf('q') >= 0)
  {
    Firebase.setInt("state_led_garage", 0);
  }

  if (inputString.indexOf('R') >= 0)
  {
    Firebase.setInt("state_fan_LVR", 1);
  }
  else if (inputString.indexOf('r') >= 0)
  {
    Firebase.setInt("state_fan_LVR", 0);
  }

  if (inputString.indexOf('S') >= 0)
  {
    Firebase.setInt("state_fan_KCR", 1);
  }
  else if (inputString.indexOf('s') >= 0)
  {
    Firebase.setInt("state_fan_KCR", 0);
  }

  if (inputString.indexOf('T') >= 0)
  {
    Firebase.setInt("state_fan_BedR", 1);
  }
  else if (inputString.indexOf('t') >= 0)
  {
    Firebase.setInt("state_fan_BedR", 0);
  }

  if (inputString.indexOf('U') >= 0)
  {
    Firebase.setInt("state_door_garage", 1);
  }
  else if (inputString.indexOf('u') >= 0)
  {
    Firebase.setInt("state_door_garage", 0);
  }

  if (inputString.indexOf('V') >= 0)
  {
    Firebase.setInt("state_clothes", 1);
  }
  else if (inputString.indexOf('v') >= 0)
  {
    Firebase.setInt("state_clothes", 0);
  }

  if (inputString.indexOf('W') >= 0)
  {
    Firebase.setInt("state_window", 1);
  }
  else if (inputString.indexOf('w') >= 0)
  {
    Firebase.setInt("state_window", 0);
  }

  if (inputString.indexOf('X') >= 0)
  {
    Firebase.setInt("state_gas", 1);
  }
  else if (inputString.indexOf('x') >= 0)
  {
    Firebase.setInt("state_gas", 0);
  }

  if (inputString.indexOf('Y') >= 0)
  {
    Firebase.setInt("state_fire", 1);
  }
  else if (inputString.indexOf('y') >= 0)
  {
    Firebase.setInt("state_fire", 0);
  }
}

void data(void)
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

void Get_data_Firebase(void)
{
  if(Firebase.getString("control_led_LVR")!= bf2[0])
  {
    bf2[0]=Firebase.getString("control_led_LVR");
    if(bf2[0]=="1") mySerial.write('A');
    else mySerial.write('a');
  }

  if(Firebase.getString("control_led_KCR")!=bf2[1])
  {
    bf2[1]=Firebase.getString("control_led_KCR");
    if(bf2[1]=="1") mySerial.write('B');
    else mySerial.write('b');
  }  

  if(Firebase.getString("control_led_BedR")!=bf2[2])
  {
    bf2[2]=Firebase.getString("control_led_BedR");
    if(bf2[2]=="1") mySerial.write('C');
    else mySerial.write('c');
  }

  if(Firebase.getString("control_led_BaR")!=bf2[3])
  {
    bf2[3]=Firebase.getString("control_led_BaR");
    if(bf2[3]=="1") mySerial.write('D');
    else mySerial.write('d');
  }

  if(Firebase.getString("control_led_garage")!=bf2[4])
  {
    bf2[4]=Firebase.getString("control_led_garage");
    if(bf2[4]=="1") mySerial.write('E');
    else mySerial.write('e');
  }

}
