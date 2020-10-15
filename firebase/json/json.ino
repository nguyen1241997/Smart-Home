#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

const String wifiName = "CanHo Truong Thinh";
const String password = "truongthinh25";

StaticJsonBuffer<256> jb;
JsonObject& obj = jb.createObject();

int temp,humi;

void setup()
{
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

  Firebase.stream("getData");
  
  obj["university"]=String("Ho Chi Minh City University of Technology");

}

void loop()
{
  if(Firebase.available())
  {
    FirebaseObject fb = Firebase.readEvent();
    Serial.println(fb.getString("path"));
    Serial.println(fb.getInt("data"));
  }
}
