#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(4, 5); // RX, TX
 
void setup()
{
  Serial.begin(9600);
 
  Serial.println("Da san sang");
 
  mySerial.begin(9600);
}
 
void loop()
{
  if (mySerial.available() > 0)
  {
    Serial.println("HI");
    delay(20);
    while (mySerial.available() > 0)
    {
      char ch = mySerial.read();
      Serial.write(ch);
      delay(3);
    }
    Serial.println();
  }
}
