#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(4, 5); // RX, TX

const String wifiName = "nguyen124";
const String password = "12345678";

int temp,humi;
char bf[20];
int i=0;

void setup()
{
  /*
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  */
  
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

  Firebase.begin("test-firebase-7ea12.firebaseio.com","MQxzVY549uNdHQnE8bqJCDraNCmKNjgCETrTFdfN");

  mySerial.begin(9600);
}

void loop()
{
  i=0;
  if (mySerial.available() > 0)
  {
    Serial.println("HI");
    delay(20);
    while (mySerial.available() > 0)
    {
      char ch = mySerial.read();
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
        Serial.println();
      }
    }
  }
  temp = bf[0]*100 + bf[1]*10 +bf[2];
  //temp=111;
  
  //temp=random(20,40);
  //humi=random(50,70);
  Firebase.setInt("temparature",temp);
  //Firebase.setInt("humidity",humi);

  for(i=0;i<20;i++)
  {
    bf[i]=0;
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
  
  delay(2000);
}
