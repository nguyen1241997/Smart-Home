#include <SPI.h> //calling Serial Peripheral Interface (SPI) Library
#include <Ethernet.h> // calling temperature ethernet shield library
#include <dht11.h> // calling temperature sensor library
#include "rfid.h" //calling RFID sensor library
#define mq2Pin 1 // MQ-2 module attach to A1 on MEGA2560
#define mqMax 700 // gas pressure maximum number
#define DHT11PIN 34 // DHT11 attach to digital pin 34 on controller board
#define photocellPin 0 // photocell attach to A0 on MEGA2560
#define cellMax 800 // photocell maximum number
#define cellMin 200 // photocell minimum number
DHT11 DHT11;
RFID rfid;
uchar serNum[5];
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:91
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x2D }; //MAC address of the controller
byte gateway[] = { 192, 168, 0, 1 }; // <-------ROUTERS IP Address to
byte subnet[] = { 255, 255, 255, 0 }; // <------- Router subnet address
IPAddress ip(192,168,0,199); //set the IP address
EthernetServer server(80); //port 80 for HTTP (transferring port)
#define RELAY_CH1 45 //pin 45 of Arduino defined as a RELAY_CH1
#define RELAY_CH2 43 // ```43`````````````````````````````````````````````````2
#define RELAY_CH3 41 // ````41````````````````````````````````````````````````3
#define RELAY_CH4 39 // `````39```````````````````````````````````````````````4
#define RELAY_CH5 37 // `````37```````````````````````````````````````````````5
#define RELAY_CH6 35 // ``````35``````````````````````````````````````````````6
#define RELAY_CH7 33 // ```````33`````````````````````````````````````````````7
#define RELAY_CH8 30 // ```````30`````````````````````````````````````````````8
#define RELAY_CH81 31 // ``````31`````````````````````````````````````````````9
#define pirPin 36 //PIR attach to pin 36
#define ledPin 38 //LED attach to pin 38
#define RELAY_CH9 42 // rfid relay attach to pin42
#define rfPin 40 //rfid LED attach to pin 4092
#define RELAY_CH10 44 // LED attach to pin 44#define RELAY_CH11 46 //Gas presure to
relay int3 =>Fire Alarm
#define RELAY_CH12 48 //temp => AC
#define uint8 unsigned char //initialize character
#define uint16 unsigned int // initialize unsigned character
#define uint32 unsigned long // initialize unsigned character
int pirValue = 0; //initialization – set PIR value to 0
int pirState = LOW; // initialization – set PIR status to OFF
String readString; //define a string
void setup() {
pinMode(RELAY_CH1, OUTPUT); // Arduino’s pin defines RELAY_CH1 as output
digitalWrite(RELAY_CH1, LOW); // LOW initial value to RELAY_CH1
pinMode(RELAY_CH2, OUTPUT); // Arduino’s pin defines RELAY_CH2 as output
digitalWrite(RELAY_CH2, LOW); // LOW initial value to RELAY_CH2
pinMode(RELAY_CH3, OUTPUT); // Arduino’s pin defines RELAY_CH3 as output
digitalWrite(RELAY_CH3, LOW); // LOW initial value to RELAY_CH3
pinMode(RELAY_CH4, OUTPUT); // Arduino’s pin defines RELAY_CH4 as output
digitalWrite(RELAY_CH4, LOW); // LOW initial value to RELAY_CH4
pinMode(RELAY_CH5, OUTPUT); // Arduino’s pin defines RELAY_CH4 as output93
digitalWrite(RELAY_CH5, LOW); // LOW initial value to RELAY_CH5
pinMode(RELAY_CH6, OUTPUT); //Arduino’s pin defines RELAY_CH6 as output
digitalWrite(RELAY_CH6, LOW); // LOW initial value to RELAY_CH6
pinMode(RELAY_CH7, OUTPUT); //Arduino’s pin defines RELAY_CH7 as output
digitalWrite(RELAY_CH7, LOW); // LOW initial value to RELAY_CH7
pinMode(RELAY_CH8, OUTPUT); //Arduino’s pin defines RELAY_CH8 as output
digitalWrite(RELAY_CH8, LOW); // LOW initial value to RELAY_CH8
// Start serial communications and hold until port gets leased:
Serial.begin(9600);
while (!Serial) {
; }
// beginning of the network connection and the server:
Ethernet.begin(mac, ip); // checks if the MAC and IP are matched
server.begin(); // set to work as server
Serial.print("server is at "); //print on LCD
Serial.println(Ethernet.localIP()); //pind on LCD
rfid.begin(2, 3, 4, 5, 6, 7); //rfid connected to those digital pin of Arduino94
delay(100); //delay
rfid.init(); //reading the rfid library
pinMode(pirPin, INPUT); // Arduino’s pin defines pirPin as Input
pinMode(ledPin, OUTPUT); // Arduino’s pin defines ledPin as output
pinMode(RELAY_CH81, OUTPUT); // Arduino’s pin defines RELAY_CH81 as output
digitalWrite(RELAY_CH81, LOW); // LOW initial value to RELAY_CH81
pinMode(RELAY_CH9, OUTPUT); // Arduino’s pin defines RELAY_CH9 as output
digitalWrite(RELAY_CH9, LOW); // LOW initial value to RELAY_CH9
pinMode(RELAY_CH10, OUTPUT); // Arduino’s pin defines RELAY_CH10 as output
pinMode(RELAY_CH11, OUTPUT); // Arduino’s pin defines RELAY_CH11 as output
digitalWrite(RELAY_CH11, LOW); // LOW initial value to RELAY_CH11
pinMode(RELAY_CH12, OUTPUT); // Arduino’s pin defines RELAY_CH12 as output
digitalWrite(RELAY_CH12, LOW); // LOW initial value to RELAY_CH12
}
void loop() { EthernetClient client = server.available(); // listening for incoming clients
if (client) {
Serial.println("new client"); // print on dispaly
boolean currentLineIsBlank = true; //checks if its blank it terminated the connection95
while (client.connected()) { // which connection is established do the below commmands
if (client.available()) { //check id the client is available
char c = client.read() //assign the read value to c which is a “character”
if (readString.length() < 100) { //check how long is the character string
readString += c; //reading next string
Serial.print(c); // print the value on the screen
}
Serial.write(c);
if (c == '\n' && currentLineIsBlank) {
// refer a typical http reply header
client.println("HTTP/1.1 200 OK"); //checks if the connection is stablished then print on dispaly
client.println("Content-Type: text/html"); //print on display
client.println("Connection: close"); // the communication will be terminated once response is
completed.
client.println("Refresh: 5"); // re-fresh the connection routinely each five second
client.println(); //prin on diplay
client.println("<!DOCTYPE HTML>"); //html coding header command
client.println("<html>"); //html coding header command
client.println("<HEAD>"); //html coding header command96
client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");//
client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'
/>");
client.println("</HEAD>"); //start programming XML for web based app
client.println("<body bgcolor=\"#D0D0D0\">"); //Background color
src=\"https://lh3.googleusercontent.com/-
C6BoJrRUFko/UEUFeCwkvdI/AAAAAAAAAOc/E7gcYvPV6r4/s960/Logo.jpg\"
/></center></h4>");
client.print("<center> <p> <h1>Welcome to Arduino Home Control V0.25 </h1></p> ");
//Display message in web broewser
client.println("<center>"); //start tables
client.println("<table border=\"5\">"); //set table border
client.println("<tr>"); //table command
if (digitalRead(RELAY_CH1)) // check the relay in the web browser app
{
client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device 1.</p><p
style=\"font-family:arial;color:green;font-size:35px;\">ON</p> </td>");
} // XML styling for buttons belong to relay 1 when it is ON
else {97
client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device 1.</p><p
style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
} // XML styling for buttons belong to relay 1 when it is OFF
if (digitalRead(RELAY_CH2))
{ client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device 2.</p><p
style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
} // XML styling for buttons belong to relay 2 when it is ON
else { client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device
2.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>"); // XML styling
for buttons belong to relay 2 when it is OFF
}
if (digitalRead(RELAY_CH3))
{ client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 3.</p><p
style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
// XML styling for buttons belong to relay 3 when it is ON }
else { client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device
3.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>"); // XML styling
for buttons belong to relay 3 when it is OFF }
if (digitalRead(RELAY_CH4))98
{ client.print("<td><p style=\"font-family:arial;color:black;fontsize:26px;\">Device 4.</p><p style=\"font-family:arial;color:green;fontsize:35px;\">ON</p></td>");
// XML styling for buttons belong to relay 4 when it is ON }
else { client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device
4.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
// XML styling for buttons belong to relay 4 when it is OFF }
if (digitalRead(RELAY_CH5))
{ client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 5.</p><p
style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
// XML styling for buttons belong to relay 5 when it is ON }
else
{
client.print("<td><p style=\"font-family:arial;color:black;fontsize:26px;\">Device 5.</p><p style=\"font-family:arial;color:red;fontsize:35px;\">OFF</p></td>");
// XML styling for buttons belong to relay 5 when it is OFF }
if (digitalRead(RELAY_CH6))99
{ client.print("<td><p style=\"font-family:arial;color:black;fontsize:26px;\">Device 6.</p><p style=\"font-family:arial;color:green;fontsize:35px;\">ON</p></td>");
// XML styling for buttons belong to relay 6 when it is ON }
else { client.print("<td><p style=\"font-family:arial;color:black;fontsize:26px;\">Device 6.</p><p style=\"font-family:arial;color:red;fontsize:35px;\">OFF</p></td>");
// XML styling for buttons belong to relay 6 when it is OFF }
if (digitalRead(RELAY_CH7))
{ client.print("<td><p style=\"font-family:arial;color:black;fontsize:26px;\">Device 7.</p><p style=\"font-family:arial;color:green;fontsize:35px;\">ON</p></td>");
// XML styling for buttons belong to relay 7 when it is ON }
else
{ client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 7.</p><p
style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
// XML styling for buttons belong to relay 7 when it is OFF }
if (digitalRead(RELAY_CH8))
{ client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 8.</p><p
style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");100
// XML styling for buttons belong to relay 8 when it is ON }
else { client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device
8.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
// XML styling for buttons belong to relay 8 when it is OFF }
client.println("</tr>");
client.println("</table>");
client.println("</center>");
client.println("<br />");
/* ------------------Relay—Control---Code------------- */
client.println("<a href=\"/?relay1on\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\"; color:red ;>Device 1 ON </font> </button> </a> ");
client.println("<a href=\"/?relay1off\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 1 OFF </font> </button> </a> <br />");
client.println("<br />");
client.println("<a href=\"/?relay2on\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 2 ON </font> </button> </a> ");
client.println("<a href=\"/?relay2off\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 2 OFF </font> </button> </a> <br />");
client.println("<br />");101
client.println("<a href=\"/?relay3on\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 3 ON </font> </button> </a> ");
client.println("<a href=\"/?relay3off\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 3 OFF </font> </button> </a> <br />");
client.println("<br />");
client.println("<a href=\"/?relay4on\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 4 ON </font> </button> </a> ");
client.println("<a href=\"/?relay4off\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 4 OFF </font> </button> </a> <br />");
client.println("<br />");
client.println("<a href=\"/?relay5on\"\"> <button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 5 ON </font> </button> </a> ");
client.println("<a href=\"/?relay5off\"\"><button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 5 OFF </font> </button> </a> <br />");
client.println("<br />");
client.println("<a href=\"/?relay6on\"\"><button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 6 ON </font> </button> </a> ");
client.println("<a href=\"/?relay6off\"\"><button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 6 OFF </font> </button> </a> <br />");
client.println("<br />");102
client.println("<a href=\"/?relay7on\"\"><button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 7 ON </font> </button> </a> ");
client.println("<a href=\"/?relay7off\"\"><button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 7 OFF </font> </button> </a> <br />");
client.println("<br />");
client.println("<a href=\"/?relay8on\"\"><button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 8 ON </font> </button> </a> ");
client.println("<a href=\"/?relay8off\"\"><button style=\"width:360px;height:120px\"> <font
size=\"7\">Device 8 OFF </font> </button> </a> <br />");
client.println("<br />");
/* -----from here control Arduino pins by Networks shield begins------- */
if(readString.indexOf("?relay1on") >0)//checks relay for ON status
{
digitalWrite(RELAY_CH1, HIGH); // // set pin relay ch1’s pin to High
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
client.println("<br />");
}
else{
if(readString.indexOf("?relay1off") >0)// checks relay for OFF status103
{
digitalWrite(RELAY_CH1, LOW); // set pin relay ch1’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");
}
}
if(readString.indexOf("?relay2on") >0) // checks relay for ON status
{
digitalWrite(RELAY_CH2, HIGH); // set pin relay ch2’s pin to High
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
client.println("<br />");
}
else{
if(readString.indexOf("?relay2off") >0) // checks relay for OFF status
{
digitalWrite(RELAY_CH2, LOW); // set pin relay ch2’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");
}104
}
if(readString.indexOf("?relay3on") >0) // checks relay for ON status
{ digitalWrite(RELAY_CH3, HIGH); // set pin relay ch3’s pin to HIGH
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
client.println("<br />");
}
else{
if(readString.indexOf("?relay3off") >0) // checks relay for OFF status
{
digitalWrite(RELAY_CH3, LOW); // set pin relay ch3’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");
}
}
if(readString.indexOf("?relay4on") >0) // checks relay for ON status
{
digitalWrite(RELAY_CH4, HIGH); // set pin relay ch4’s pin to HIGH
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");105
client.println("<br />");
}
else{
if(readString.indexOf("?relay4off") >0) // checks relay for OFF status
{
digitalWrite(RELAY_CH4, LOW); // set pin relay ch4’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");
}
}
if(readString.indexOf("?relay5on") >0) // checks relay for ON status
{
digitalWrite(RELAY_CH5, HIGH); // set pin relay ch5’s pin to HIGH
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
client.println("<br />");
}
else{
if(readString.indexOf("?relay5off") >0) // checks relay for OFF status106
{
digitalWrite(RELAY_CH5, LOW); // set pin relay ch5’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");
}
}
if(readString.indexOf("?relay6on") >0) // checks relay for ON status
{
digitalWrite(RELAY_CH6, HIGH); // set pin relay ch6’s pin to HIGH
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
client.println("<br />");
}
else{
if(readString.indexOf("?relay6off") >0) // checks relay for OFF status
{
digitalWrite(RELAY_CH6, LOW); // set pin relay ch6’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");107
}
}
if(readString.indexOf("?relay7on") >0) // checks relay for ON status
{
digitalWrite(RELAY_CH7, HIGH); // set pin relay ch7’s pin to HIGH
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
client.println("<br />");
}
else{
if(readString.indexOf("?relay7off") >0) // checks relay for OFF status
{
digitalWrite(RELAY_CH7, LOW); // set pin relay ch7’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");
}
}
if(readString.indexOf("?relay8on") >0) // checks relay 8 for ON status
{108
digitalWrite(RELAY_CH8, HIGH); // set pin relay ch8’s pin to HIGH
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
client.println("<br />");
}
else{
if(readString.indexOf("?relay8off") >0) // checks relay 8 for OFF status
{
digitalWrite(RELAY_CH8, LOW); // set pin relay ch8’s pin to LOW
client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
client.println("<br />");
}
}
}
client.println("<hr> <p> By <a href=\"www.linkedin.com/pub/ehsan-dolatshahi/9a/321/280/
"></p><p style=\"font-family:arial;color:blue;font-size:20px;\">EHSAN
DOLATSHAHI</p></a>");
readString="";
client.println("</body>");
client.println("</html>");109
break;
}// if ( c == \n )
if (c == '\n') {
currentLineIsBlank = true;
}
else if (c != '\r') {
currentLineIsBlank = false;
}
}
}
delay(1); // provide some time to web browser to get the data .
client.stop(); // suspend the connection
Serial.println("client disonnected"); //disconnect the connection
}
//--------------------------------------------PHOTO CELL-----------------------
int num;
Serial.print("Photocell Value: "); //write photocell value on display
num = analogRead(photocellPin); //reading analog pin belong to photocell110
if(num <= cellMin){
// compare the photocell value to the defined value to minimum lightness
digitalWrite(RELAY_CH9, HIGH); // if it’s dark turn on the relay9
} //turn on lights
else{
digitalWrite(RELAY_CH9, LOW); // if it is light enough, turn off the light.
}
Serial.println(num);
//------------------------------------------END OF PHOTOCELL
//------------------------------------------MQ-2 – GAS SENSOR
int num2;
Serial.print("MQ-2 Value: "); //print gas pressure value on the display
num2 = analogRead(mq2Pin); //read value of the gas pressure from Arduino analog pin
if(num2 >= mqMax){
// compare the value of gas pressure to the normal gas pressure rage
digitalWrite(RELAY_CH11, HIGH);
/* if the value is not in the normal range, turn on the relay11 (fire alarm) */
}else{111
//if(num2 < mqMax){
digitalWrite(RELAY_CH11, LOW);
/* if the value is in the normal range, turn off the relay11 (fire alarm) */
}
Serial.println(num2);
//--------------------------------------END OF MQ-2
//-----------------------------------------TEMP SENSOR
Serial.print("DHT11, \t");
int chk = DHT11.read(DHT11PIN); //read the value returned from temperature sensor
switch (chk) //switch case for sensor availability status
{
case DHTLIB_OK:
Serial.print("OK,\t");
break;
case DHTLIB_ERROR_CHECKSUM:
Serial.print("Checksum error,\t");
break;
case DHTLIB_ERROR_TIMEOUT:112
Serial.print("Time out error,\t");
break;
default:
Serial.print("Unknown error,\t");
break;
}
Serial.println(); // print DATA information on the display
Serial.print("Tem: ");
Serial.print(DHT11.temperature); //display temp value on serial monitor
if(DHT11.temperature >= 29)
//check if the temperature is in the normal room temperature
{
digitalWrite(RELAY_CH12, HIGH);
/* if the value is not in the normal range, turn on the relay12 (fan) */
}else{
digitalWrite(RELAY_CH12, LOW);
/* if the value is in the normal range, turn off the relay12 (fan) */
} // display temperature information on serial display113
Serial.println(" C");
Serial.print("Hum: ");
Serial.print(DHT11.humidity); //print the humidity on serial monitor
Serial.println(" %");
Serial.println();
//-------------------------------------END OF TEMP
//-------------------------------------PIR motion detection
pirValue = digitalRead(pirPin);
// read value of Arduino pin which is connected to the sensor
if (pirValue == HIGH){ // check if there is a motion? = yes
digitalWrite(ledPin, pirValue); // turn on the LED belong to motion detection
digitalWrite(RELAY_CH81, pirValue);
//turn on the relay 81 (lamp) belong to motion detection
if (pirState == LOW){ //check the status of the PIR
pirState = HIGH; //initial value in the local loop
}
}else{
digitalWrite(ledPin, LOW); //turn off the LED belong to PIR114
digitalWrite(RELAY_CH81, pirValue); //turn off relay 81 belong to PIR
if(pirState == HIGH){ //check the status of the PIR
pirState = LOW; //initial value in the local loop
}
}
//--------------------------------------END OF--PIR motion detection
//--------------------------------------RFID
uchar status;
uchar str[MAX_LEN];
status = rfid.request(PICC_REQIDL, str); // read the rfid
if (status != MI_OK) // check the status of rfid
{
return;
}
rfid.showCardType(str);
status = rfid.anticoll(str);
if (status == MI_OK)
{115
Serial.print("The card's number is: ");
// display scanned card number on the serial monitor
memcpy(serNum, str, 5);
rfid.showCardID(serNum);
Serial.println();
// Check if the scanned tag is authorized
uchar* id = serNum;
if( id[0]==0x54 && id[1]==0x26 && id[2]==0xBC && id[3]==0x96 )
{ //check the code of tag
Serial.println("Hello Ehsan!"); //if its match display user name of serial monitor
digitalWrite(rfPin, 1); //rfid led turns on
digitalWrite(RELAY_CH10, 1); //rfid relay on
delay(2000); //wait time
digitalWrite(rfPin, 0); //rfid led off anter 2000 ms
digitalWrite(RELAY_CH10, 0); //rfid rely off after 2000 ms
}
else if(id[0] == 0x15 && id[1] == 0x6F && id[2] == 0x9F && id[3] == 0xAF)
{116
Serial.println("Collect!");
Serial.println();
}
else
{
Serial.println("Hello unkown guy!");
// if the user is not authorized it show this message on the serial monitor
}
}
//-----------------------------------------END OF RFID
}//Loop
