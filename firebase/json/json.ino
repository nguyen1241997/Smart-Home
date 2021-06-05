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
  
  if (inputString.indexOf('O') >= 0)
  {
    Firebase.setInt("state_led_BedR", 1);
  }
  else if (inputString.indexOf('o') >= 0)
  {
    Firebase.setInt("state_led_BedR", 0);
  }

  if (inputString.indexOf('P') >= 0)
  {
    Firebase.setInt("state_led_BaR", 1);
  }
  else if (inputString.indexOf('p') >= 0)
  {
    Firebase.setInt("state_led_BaR", 0);
  }

  if (inputString.indexOf('Q') >= 0)
  {
    Firebase.setInt("state_led_garage", 1);
  }
  else if (inputString.indexOf('q') >= 0)
  {
    Firebase.setInt("state_led_garage", 0);
  }

  if (inputString.indexOf('R') >= 0)
  {
    Firebase.setInt("state_fan_LVR", 1);
  }
  else if (inputString.indexOf('r') >= 0)
  {
    Firebase.setInt("state_fan_LVR", 0);
  }

  if (inputString.indexOf('S') >= 0)
  {
    Firebase.setInt("state_fan_KCR", 1);
  }
  else if (inputString.indexOf('s') >= 0)
  {
    Firebase.setInt("state_fan_KCR", 0);
  }

  if (inputString.indexOf('T') >= 0)
  {
    Firebase.setInt("state_fan_BedR", 1);
  }
  else if (inputString.indexOf('t') >= 0)
  {
    Firebase.setInt("state_fan_BedR", 0);
  }

  if (inputString.indexOf('U') >= 0)
  {
    Firebase.setInt("state_door_garage", 1);
  }
  else if (inputString.indexOf('u') >= 0)
  {
    Firebase.setInt("state_door_garage", 0);
  }

  if (inputString.indexOf('V') >= 0)
  {
    Firebase.setInt("state_clothes", 1);
  }
  else if (inputString.indexOf('v') >= 0)
  {
    Firebase.setInt("state_clothes", 0);
  }

  if (inputString.indexOf('W') >= 0)
  {
    Firebase.setInt("state_window", 1);
  }
  else if (inputString.indexOf('w') >= 0)
  {
    Firebase.setInt("state_window", 0);
  }

  if (inputString.indexOf('X') >= 0)
  {
    Firebase.setInt("state_gas", 1);
  }
  else if (inputString.indexOf('x') >= 0)
  {
    Firebase.setInt("state_gas", 0);
  }

  if (inputString.indexOf('Y') >= 0)
  {
    Firebase.setInt("state_fire", 1);
  }
  else if (inputString.indexOf('y') >= 0)
  {
    Firebase.setInt("state_fire", 0);
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

}
