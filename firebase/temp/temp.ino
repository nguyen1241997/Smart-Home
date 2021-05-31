#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>


#include <ESP8266WiFi.h>

//const String wifiName = "nguyen124";
//const String password = "12345678";
const String wifiName = "CanHo Truong Thinh";
const String password = "truongthinh25";

int temp, humi;
String _temp, _humi;

String inputString = "";

void setup() {

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

}

String a;
void loop()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '_') inputString = "";
    /*
      if (inChar == '-')
      {
      Serial.print("Data received: ");
      Serial.println(inputString);
      //inputString = "";
      }
    */
  }

  Serial.print("Data received: ");
  Serial.println(inputString);

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

  Firebase.setInt("temparature", temp);
  Firebase.setInt("humidity", humi);

  //---------------
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

  Serial.println(".");
  delay(5000);
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
  
  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_led_BedR", 1);
  }
  else if (bf[2] == 'o')
  {
    Firebase.setInt("state_led_BedR", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_led_BaR", 1);
  }
  else if (bf[3] == 'p')
  {
    Firebase.setInt("state_led_BaR", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_led_garage", 1);
  }
  else if (bf[4] == 'q')
  {
    Firebase.setInt("state_led_garage", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_fan_LVR", 1);
  }
  else if (bf[5] == 'r')
  {
    Firebase.setInt("state_fan_LVR", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_fan_KCR", 1);
  }
  else if (bf[6] == 's')
  {
    Firebase.setInt("state_fan_KCR", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_fan_BedR", 1);
  }
  else if (bf[7] == 't')
  {
    Firebase.setInt("state_fan_BedR", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_door_garage", 1);
  }
  else if (bf[8] == 'u')
  {
    Firebase.setInt("state_door_garage", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_clothes", 1);
  }
  else if (bf[9] == 'v')
  {
    Firebase.setInt("state_clothes", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_window", 1);
  }
  else if (bf[10] == 'w')
  {
    Firebase.setInt("state_window", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_gas", 1);
  }
  else if (bf[11] == 'x')
  {
    Firebase.setInt("state_gas", 0);
  }

  if (inputString.indexOf('N') >= 0)
  {
    Firebase.setInt("state_fire", 1);
  }
  else if (bf[12] == 'y')
  {
    Firebase.setInt("state_fire", 0);
  }
}
