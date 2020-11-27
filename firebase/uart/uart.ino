#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(4,5); // RX, TX

//const String wifiName = "CanHo Truong Thinh 2";
//const String password = "truongthinh25";
const String wifiName = "nguyen124";
const String password = "12345678";

int temp,humi,gas;
char bf[20];
int l=0;
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
    }
  }

  //send data to firebase
  Firebase.setInt("temparature",temp);
  Firebase.setInt("humidity",humi);
  Firebase.setInt("gas_sensor",gas);

  //get data from firebase
  Serial.println("aaaaaaaaaaaaaaa");
  if(Firebase.getString("control_led_LVR")=="\"1\"")
  {
    mySerial.write('A');
  }
  else
  {
    mySerial.write('a');
  }

  if(Firebase.getString("control_led_KCR")=="\"1\"")
  {
    mySerial.write('B');
  }
  else
  {
    mySerial.write('b');
  }
  

  if(Firebase.getString("control_led_BedR")=="\"1\"")
  {
    mySerial.write('C');
  }
  else
  {
    mySerial.write('c');
  }

  if(Firebase.getString("control_led_BaR")=="\"1\"")
  {
    mySerial.write('D');
  }
  else
  {
    mySerial.write('d');
  }
  
  if(Firebase.getString("control_led_garage")=="\"1\"")
  {
    mySerial.write('E');
  }
  else
  {
    mySerial.write('e');
  }

  if(Firebase.getString("control_fan_LVR")=="\"1\"")
  {
    mySerial.write('F');
  }
  else
  {
    mySerial.write('f');
  }

  if(Firebase.getString("control_fan_KCR")=="\"1\"")
  {
    mySerial.write('G');
  }
  else
  {
    mySerial.write('g');
  }

  if(Firebase.getString("control_fan_BedR")=="\"1\"")
  {
    mySerial.write('H');
  }
  else
  {
    mySerial.write('h');
  }

  if(Firebase.getString("control_door_garage")=="\"1\"")
  {
    mySerial.write('I');
  }
  else
  {
    mySerial.write('i');
  }

  if(Firebase.getString("control_clothes")=="\"1\"")
  {
    mySerial.write('J');
  }
  else
  {
    mySerial.write('j');
  }

  if(Firebase.getString("control_window")=="\"1\"")
  {
    mySerial.write('K');
  }
  else
  {
    mySerial.write('k');
  }
  
  Serial.println("bbbbbbbbbbbbbbbb");
  delay(1000);
}
