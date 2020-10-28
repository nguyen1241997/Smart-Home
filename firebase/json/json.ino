#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

const String wifiName = "CanHo Truong Thinh";
const String password = "truongthinh25";

int temp,humi;

void setup()
{
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  
  Serial.begin(115200);
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
}

void loop()
{
  temp=random(20,40);
  humi=random(50,70);
  Firebase.setInt("temparature",temp);
  Firebase.setInt("humidity",humi);

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
  
  delay(100);
}
