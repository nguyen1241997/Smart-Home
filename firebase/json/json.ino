#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(4,5); // RX, TX

const String wifiName = "nguyen124";
const String password = "12345678";

int temp,humi;
char bf[20];
int i=0;
char x=50;

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.print("Connecting");
  WiFi.begin(wifiName,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected");

  Firebase.begin("smart-home-a4a54.firebaseio.com","M5Jax2jJQnVnnWPeJegIrJtWhMbZwtI0wts0ihKy");
}

void loop()
{
  if (mySerial.available() > 0)
  {
    Serial.println("HI");
    //delay(20);
    while (mySerial.available() > 0)
    {
      //delele buffer
      for(i=0;i<20;i++)
      {
        bf[i]=0;
      }
      
      char ch = mySerial.read();

      //temparature
      if (ch=='a') 
      {
        i=0;
        while(1)
        {
          ch = mySerial.read();
          Serial.print(ch);
          if (ch=='z') break;
          bf[i]= ch-48;
          i++;                  
        }
        temp = bf[0]*10 + bf[1];
        Serial.println();
      }

      //humidity
      if (ch=='b') 
      {
        i=0;
        while(1)
        {
          ch = mySerial.read();
          Serial.print(ch);
          if (ch=='y') break;
          bf[i]= ch-48;
          i++;                  
        }
        humi = bf[0]*10 + bf[1];
        Serial.println();
      }
    }
  }
  

  Firebase.setInt("temparature",temp);
  Firebase.setInt("humidity",humi);



  if(Firebase.getString("control_door")=="\"1\"")
  {
    mySerial.write('a');
  }
  else
  {
    mySerial.write('A');
  }

  if(Firebase.getString("control_fan_BedR")=="\"1\"")
  {
    mySerial.write('b');
  }
  else
  {
    mySerial.write('B');
  }

  if(Firebase.getString("control_fan_KCR")=="\"1\"")
  {
    mySerial.write('c');
  }
  else
  {
    mySerial.write('C');
  }

  if(Firebase.getString("control_fan_LVR")=="\"1\"")
  {
    mySerial.write('e');
  }
  else
  {
    mySerial.write('E');
  }

  if(Firebase.getString("control_led_BaR")=="\"1\"")
  {
    mySerial.write('f');
  }
  else
  {
    mySerial.write('F');
  }

  if(Firebase.getString("control_led_BedR")=="\"1\"")
  {
    mySerial.write('g');
  }
  else
  {
    mySerial.write('G');
  }

  if(Firebase.getString("control_led_KCR")=="\"1\"")
  {
    mySerial.write('h');
  }
  else
  {
    mySerial.write('H');
  }

  if(Firebase.getString("control_led_LVR")=="\"1\"")
  {
    mySerial.write('i');
  }
  else
  {
    mySerial.write('I');
  }

  if(Firebase.getString("control_led_garage")=="\"1\"")
  {
    mySerial.write('j');
  }
  else
  {
    mySerial.write('J');
  }

  if(Firebase.getString("control_window")=="\"1\"")
  {
    mySerial.write('k');
  }
  else
  {
    mySerial.write('K');
  }

  

  /*

  Serial.println(Firebase.getString("led1Control"));

  

  //led 1 control
  if(Firebase.getString("led1Control")=="\"1\"")
  {
    Firebase.setInt("led1State",1);
    digitalWrite(D1,HIGH);
    Serial.println("led 1 on");
  }
  else
  {
    Firebase.setInt("led1State",0);
    digitalWrite(D1,LOW);
    Serial.println("led 1 off");
  }

  //led 2 control
  if(Firebase.getString("led2Control")=="\"1\"")
  {
    Firebase.setInt("led2State",1);
    digitalWrite(D2,HIGH);
    Serial.println("led 2 on");
  }
  else
  {
    Firebase.setInt("led2State",0);
    digitalWrite(D2,LOW);
    Serial.println("led 2 off");
  }
  */
  
  delay(20);
}
