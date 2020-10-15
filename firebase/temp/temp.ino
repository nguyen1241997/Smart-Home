#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

const String wifiName = "R&D Elex Lab";
const String password = "77namhai88";

int temp,humi;

void setup() {
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

void loop() {
  temp=random(20,40);
  humi=random(50,80);
  Serial.println(temp);
  Serial.println(humi);
  Firebase.setFloat("temp",temp);
  Firebase.setFloat("humi",humi);
  delay(1000);

}
