#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

#include <SPI.h>
#include <MFRC522.h>

/*
SDA - D2
SCK - D5
MOSI - D7
MISO - D6
RST - D1
D0-D4
3.3 - 3.3V
GND - GND
 */

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance/object.

int variable = 0;


unsigned long uidDec, uidDecTemp; // hien thi so UID dang thap phan
byte bCounter, readBit;
unsigned long ticketNumber;

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Show your card:");
}
void loop() 
{  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  Serial.println();
  
  if (content.substring(1) == "F9 1F A5 96") //change UID of the card that you want to give access
  {
    Serial.println(" Authorized Access ");
    Serial.println(" Welcome to my channel Mostly Programing ");
    Serial.println(" Learn Programming and Coding Skills ");
    delay(1000);
  }
  else
  {
    Serial.println(" Access Denied ");
    delay(3000);
  }
  
  /*
  uidDec = 0;

  // Hien thi so UID cua the
  Serial.println("Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec*256+uidDecTemp;
  } 
  Serial.println(uidDec);

  delay(1000);
  */
} 
