#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

SoftwareSerial mySerial(2,16); // RX, TX, D0-PA3, D4

const String wifiName = " Cafe Hoang Mai";
const String password = "12345678";

int temp=0, humi=0, gas=0;
String _temp, _humi, _gas;

String inputString = "";
String inputString2 = "";

String bf2[15], bf1[15];
int rc[15], rc1[15];

void send_firebase(void);
void receive_uart(void);
void Get_data_Firebase2(void);

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
    rc[i]=0;
    rc1[i]=0;
  }
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
  if (inputString.indexOf('M') >= 0)
  {
    rc1[0]=1;
  }
  else if (inputString.indexOf('m') >= 0)
  {
    rc1[0]=0;
  }

  if (inputString.indexOf('N') >= 0)
  {
    rc1[1]=1;
  }
  else if (inputString.indexOf('n') >= 0)
  {
    rc1[1]=0;
  }

  if (inputString.indexOf('O') >= 0)
  {
    rc1[2]=1;
  }
  else if (inputString.indexOf('o') >= 0)
  {
    rc1[2]=0;
  }

  if (inputString.indexOf('P') >= 0)
  {
    rc1[3]=1;
  }
  else if (inputString.indexOf('p') >= 0)
  {
    rc1[3]=0;
  }

  if (inputString.indexOf('Q') >= 0)
  {
    rc1[4]=1;
  }
  else if (inputString.indexOf('q') >= 0)
  {
    rc1[4]=0;
  }

  if (inputString.indexOf('R') >= 0)
  {
    rc1[5]=1;
  }
  else if (inputString.indexOf('r') >= 0)
  {
    rc1[5]=0;
  }

  if (inputString.indexOf('S') >= 0)
  {
    rc1[6]=1;
  }
  else if (inputString.indexOf('s') >= 0)
  {
    rc1[6]=0;
  }

  if (inputString.indexOf('T') >= 0)
  {
    rc1[7]=1;
  }
  else if (inputString.indexOf('t') >= 0)
  {
    rc1[7]=0;
  }

  if (inputString.indexOf('U') >= 0)
  {
    rc1[8]=1;
  }
  else if (inputString.indexOf('u') >= 0)
  {
    rc1[8]=0;
  }

  if (inputString.indexOf('V') >= 0)
  {
    rc1[9]=1;
  }
  else if (inputString.indexOf('v') >= 0)
  {
    rc1[9]=0;
  }

  if (inputString.indexOf('W') >= 0)
  {
    rc1[10]=1;
  }
  else if (inputString.indexOf('w') >= 0)
  {
    rc1[10]=0;
  }

  if (inputString.indexOf('X') >= 0)
  {
    rc1[11]=1;
  }
  else if (inputString.indexOf('x') >= 0)
  {
    rc1[11]=0;
  }

  if (inputString.indexOf('Y') >= 0)
  {
    rc1[12]=1;
  }
  else if (inputString.indexOf('y') >= 0)
  {
    rc1[12]=0;
  }

  //send data to firebase
  if (rc1[0] != rc[0])
  {
    rc[0] = rc1[0];
    if(rc1[0]==1)
    {
      Firebase.setInt("state_led_LVR", 1);
    }
    else
    {
      Firebase.setInt("state_led_LVR", 0);
    }
  }

  if (rc1[1] != rc[1])
  {
    rc[1] = rc1[1];
    if(rc1[1]==1)
    {
      Firebase.setInt("state_led_KCR", 1);
    }
    else
    {
      Firebase.setInt("state_led_KCR", 0);
    }
  }

  if (rc1[2] != rc[2])
  {
    rc[2] = rc1[2];
    if(rc1[2]==1)
    {
      Firebase.setInt("state_led_BedR", 1);
    }
    else
    {
      Firebase.setInt("state_led_BedR", 0);
    }
  }

  if (rc1[3] != rc[3])
  {
    rc[3] = rc1[3];
    if(rc1[3]==1)
    {
      Firebase.setInt("state_led_BaR", 1);
    }
    else
    {
      Firebase.setInt("state_led_BaR", 0);
    }
  }

  if (rc1[4] != rc[4])
  {
    rc[4] = rc1[4];
    if(rc1[4]==1)
    {
      Firebase.setInt("state_led_garage", 1);
    }
    else
    {
      Firebase.setInt("state_led_garage", 0);
    }
  }

  if (rc1[5] != rc[5])
  {
    rc[5] = rc1[5];
    if(rc1[5]==1)
    {
      Firebase.setInt("state_fan_LVR", 1);
    }
    else
    {
      Firebase.setInt("state_fan_LVR", 0);
    }
  }

  if (rc1[6] != rc[6])
  {
    rc[6] = rc1[6];
    if(rc1[6]==1)
    {
      Firebase.setInt("state_fan_KCR", 1);
    }
    else
    {
      Firebase.setInt("state_fan_KCR", 0);
    }
  }

  if (rc1[7] != rc[7])
  {
    rc[7] = rc1[7];
    if(rc1[7]==1)
    {
      Firebase.setInt("state_fan_BedR", 1);
    }
    else
    {
      Firebase.setInt("state_fan_BedR", 0);
    }
  }

  if (rc1[8] != rc[8])
  {
    rc[8] = rc1[8];
    if(rc1[8]==1)
    {
      Firebase.setInt("state_door_garage", 1);
    }
    else
    {
      Firebase.setInt("state_door_garage", 0);
    }
  }

  if (rc1[9] != rc[9])
  {
    rc[9] = rc1[9];
    if(rc1[9]==1)
    {
      Firebase.setInt("state_clothes", 1);
    }
    else
    {
      Firebase.setInt("state_clothes", 0);
    }
  }

  if (rc1[10] != rc[10])
  {
    rc[10] = rc1[10];
    if(rc1[10]==1)
    {
      Firebase.setInt("state_window", 1);
    }
    else
    {
      Firebase.setInt("state_window", 0);
    }
  }

  if (rc1[11] != rc[11])
  {
    rc[11] = rc1[11];
    if(rc1[11]==1)
    {
      Firebase.setInt("state_gas", 1);
    }
    else
    {
      Firebase.setInt("state_gas", 0);
    }
  }

  if (rc1[12] != rc[12])
  {
    rc[12] = rc1[12];
    if(rc1[12]==1)
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
    bf1[0]=bf2[0];
    if(bf2[0]=="1") mySerial.write('A');
    else mySerial.write('a');
  }

  bf2[1]=Firebase.getString("control_led_KCR");
  if (bf2[1]!=bf1[1])
  {
    bf1[1]=bf2[1];
    if(bf2[1]=="1") mySerial.write('B');
    else mySerial.write('b');
  }
  
  bf2[2]=Firebase.getString("control_led_BedR");
  if (bf2[2]!=bf1[2])
  {
    bf1[2]=bf2[2];
    if(bf2[2]=="1") mySerial.write('C');
    else mySerial.write('c');
  }

  bf2[3]=Firebase.getString("control_led_BaR");
  if (bf2[3]!=bf1[3])
  {
    bf1[3]=bf2[3];
    if(bf2[3]=="1") mySerial.write('D');
    else mySerial.write('d');
  }
  
  bf2[4]=Firebase.getString("control_led_garage");
  if (bf2[4]!=bf1[4])
  {
    bf1[4]=bf2[4];
    if(bf2[4]=="1") mySerial.write('E');
    else mySerial.write('e');
  }

  bf2[5]=Firebase.getString("control_fan_LVR");
  if (bf2[5]!=bf1[5])
  {
    bf1[5]=bf2[5];
    if(bf2[5]=="1") mySerial.write('F');
    else mySerial.write('f');
  }
  
  bf2[6]=Firebase.getString("control_fan_LVR");
  if (bf2[6]!=bf1[6])
  {
    bf1[6]=bf2[6];
    if(bf2[6]=="1") mySerial.write('G');
    else mySerial.write('g');
  }

  bf2[7]=Firebase.getString("control_fan_BedR");
  if (bf2[7]!=bf1[7])
  {
    bf1[7]=bf2[7];
    if(bf2[7]=="1") mySerial.write('H');
    else mySerial.write('h');
  }
  
  bf2[8]=Firebase.getString("control_door_garage");
  if (bf2[8]!=bf1[8])
  {
    bf1[8]=bf2[8];
    if(bf2[8]=="1") mySerial.write('I');
    else mySerial.write('i');
  }

  bf2[9]=Firebase.getString("control_clothes");
  if (bf2[9]!=bf1[9])
  {
    bf1[9]=bf2[9];
    if(bf2[9]=="1") mySerial.write('J');
    else mySerial.write('j');
  }

  bf2[10]=Firebase.getString("control_clothes");
  if (bf2[10]!=bf1[10])
  {
    bf1[10]=bf2[10];
    if(bf2[10]=="1") mySerial.write('K');
    else mySerial.write('k');
  }
}
