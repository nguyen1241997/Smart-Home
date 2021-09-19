#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1s

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

MFRC522 mfrc522(SS_PIN, RST_PIN);
String content= "";
 
SoftwareSerial mySerial(2,16); // RX, TX, D0-PA3, D4

const String wifiName = "CanHo Truong Thinh";
const String password = "truongthinh25";
//const String wifiName = "nguyen124";
//const String password = "12345678";

int temp,humi,gas;
char bf[20];
String bf2[15];
int i=0;
String a;


void Send_Data_Firebase(void);
void Get_data_Firebase(void);
void RFID(void);

void setup()
{
  SPI.begin();
  mfrc522.PCD_Init();
  
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.print("Connecting");
  WiFi.begin(wifiName,password);
  /*
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  */
  Serial.println();
  Serial.println("Connected");

  Firebase.begin("smart-home-a4a54.firebaseio.com","M5Jax2jJQnVnnWPeJegIrJtWhMbZwtI0wts0ihKy");

  for(i=0;i<15;i++)
  {
    bf2[i]="0";
  }
}

void loop()
{

  //Receive_uart();
  
  //send data to firebase
  /*
  temp=32;
  humi=67;
  gas=1111;
  Firebase.setInt("temparature",temp);
  Firebase.setInt("humidity",humi);
  Firebase.setInt("gas_sensor",gas);

  Get_data_Firebase();
  */
  RFID();
  
  delay(10);
}

void RFID(void)
{
  Serial.println("---------------------");
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();

  if (content.substring(1) == "F9 1F A5 96")
  {
    mySerial.write('L');
    Serial.println("ok");
  }
  else
  {
    mySerial.write('l');
    Serial.println("not");
  }
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

  if(Firebase.getString("control_fan_LVR")!=bf2[5])
  {
    bf2[5]=Firebase.getString("control_fan_LVR");
    if(bf2[5]=="1") mySerial.write('F');
    else mySerial.write('f');
  }

  if(Firebase.getString("control_fan_KCR")!=bf2[6])
  {
    bf2[6]=Firebase.getString("control_fan_KCR");
    if(bf2[6]=="1") mySerial.write('G');
    else mySerial.write('g');
  }

  if(Firebase.getString("control_fan_BedR")!=bf2[7])
  {
    bf2[7]=Firebase.getString("control_fan_BedR");
    if(bf2[7]=="1") mySerial.write('H');
    else mySerial.write('h');
  }

  if(Firebase.getString("control_door_garage")!=bf2[8])
  {
    bf2[8]=Firebase.getString("control_door_garage");
    if(bf2[8]=="1") mySerial.write('I');
    else mySerial.write('i');
  }

  if(Firebase.getString("control_clothes")!=bf2[9])
  {
    bf2[9]=Firebase.getString("control_clothes");
    if(bf2[9]=="1") mySerial.write('J');
    else mySerial.write('j');
  }

  if(Firebase.getString("control_window")!=bf2[10])
  {
    bf2[10]=Firebase.getString("control_window");
    if(bf2[10]=="1") mySerial.write('K');
    else mySerial.write('k');
  }

  if(Firebase.getString("control_warning")!=bf2[11])
  {
    bf2[10]=Firebase.getString("control_warning");
    if(bf2[10]=="0") mySerial.write('0');
  }
}

void Send_Data_Firebase(void)
{
      if (bf[0]=='M')
      {
        Firebase.setInt("state_led_LVR",1);
      }
      else if (bf[0]=='m')
      {
        Firebase.setInt("state_led_LVR",0);
      }

      if (bf[1]=='N')
      {
        Firebase.setInt("state_led_KCR",1);
      }
      else if (bf[1]=='n')
      {
        Firebase.setInt("state_led_KCR",0);
      }

      if (bf[2]=='O')
      {
        Firebase.setInt("state_led_BedR",1);
      }
      else if (bf[2]=='o')
      {
        Firebase.setInt("state_led_BedR",0);
      }

      if (bf[3]=='P')
      {
        Firebase.setInt("state_led_BaR",1);
      }
      else if (bf[3]=='p')
      {
        Firebase.setInt("state_led_BaR",0);
      }

      if (bf[4]=='Q')
      {
        Firebase.setInt("state_led_garage",1);
      }
      else if (bf[4]=='q')
      {
        Firebase.setInt("state_led_garage",0);
      }

      if (bf[5]=='R')
      {
        Firebase.setInt("state_fan_LVR",1);
      }
      else if (bf[5]=='r')
      {
        Firebase.setInt("state_fan_LVR",0);
      }

      if (bf[6]=='S')
      {
        Firebase.setInt("state_fan_KCR",1);
      }
      else if (bf[6]=='s')
      {
        Firebase.setInt("state_fan_KCR",0);
      }

      if (bf[7]=='T')
      {
        Firebase.setInt("state_fan_BedR",1);
      }
      else if (bf[7]=='t')
      {
        Firebase.setInt("state_fan_BedR",0);
      }

      if (bf[8]=='U')
      {
        Firebase.setInt("state_door_garage",1);
      }
      else if (bf[8]=='u')
      {
        Firebase.setInt("state_door_garage",0);
      }

      if (bf[9]=='V')
      {
        Firebase.setInt("state_clothes",1);
      }
      else if (bf[9]=='v')
      {
        Firebase.setInt("state_clothes",0);
      }

      if (bf[10]=='W')
      {
        Firebase.setInt("state_window",1);
      }
      else if (bf[10]=='w')
      {
        Firebase.setInt("state_window",0);
      }

      if (bf[11]=='X')
      {
        Firebase.setInt("state_gas",1);
      }
      else if (bf[11]=='x')
      {
        Firebase.setInt("state_gas",0);
      }

      if (bf[12]=='Y')
      {
        Firebase.setInt("state_fire",1);
      }
      else if (bf[12]=='y')
      {
        Firebase.setInt("state_fire",0);
      }
}

void Receive_uart(void)
{
  if (mySerial.available() > 0)
  {
    while (mySerial.available() > 0)
    {
      //delele buffer
      for(i=0;i<20;i++)
      {
        bf[i]=0;
      }
      
      char ch = mySerial.read();
      Serial.println(ch);

      //temparature
      if (ch=='#') 
      {
        i=0;
        while(1)
        {
          ch = mySerial.read();
          Serial.print(ch);
          if (ch=='$') break;
          bf[i]= ch-48;
          i++;                  
        }
        temp = bf[0]*10 + bf[1];
        Serial.println();
      }

      //humidity
      if (ch=='^') 
      {
        i=0;
        while(1)
        {
          ch = mySerial.read();
          Serial.print(ch);
          if (ch=='*') break;
          bf[i]= ch-48;
          i++;                  
        }
        humi = bf[0]*10 + bf[1];
        Serial.println();
      }

      if (ch=='!') //gas sensor
      {
        i=0;
        while(1)
        {
          ch = mySerial.read();
          Serial.print(ch);
          if (ch=='@') break;
          bf[i]= ch-48;
          i++;                  
        }
        gas = bf[0]*1000 + bf[1]*100 + bf[2]*10 + bf[3];
        Serial.println();
      }

      //gpio
      if (ch=='(') 
      {
        i=0;
        while(1)
        {
          ch = mySerial.read();
          Serial.print(ch);
          if (ch==')') break;
          bf[i]= ch;
          i++;                     
        }
        Serial.println();
      }

      
      //send gpio state to firebase
      Send_Data_Firebase();

    }
  }
}
