// Handle Serial input
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

void serialHandler(String input){
  int fIndex = slitString(input,'#',0).toInt();
  String fValue= slitString(input,'#',1);
      char t_char[sizeof(slitString(input,'#',2))];
    int adr;
  
  switch(fIndex){
    // Time to set => Format 1#DD/MM/YYYY/H/i
    case 1:
      rtc.adjust(slitString(fValue,'/',2).toInt(),slitString(fValue,'/',1).toInt(),slitString(fValue,'/',0).toInt(),slitString(fValue,'/',3).toInt(),slitString(fValue,'/',4).toInt(),0);
    break;
    // Light Chanel => Format 2#0#0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0
    case 2:
      writeLightArr(slitString(input,'#',2),fValue.toInt());
      adr = eepromLight +(sizeof(s_lightVal)*fValue.toInt());
      slitString(input,'#',2).toCharArray(t_char,sizeof(s_lightVal));
      EEPROM.updateBlock(adr, t_char,sizeof(s_lightVal));
      EEPROM.updateByte(eepromAdrLight, overwrite);
    break;
    // Dosing => Format 3#1=NPK=10:00=5=60
    case 3:
       writePumpArr(slitString(input,'#',2),fValue.toInt());
      adr = eepromDosing +(sizeof(s_dosingVal)*fValue.toInt());
      slitString(input,'#',2).toCharArray(t_char,sizeof(s_dosingVal));
      EEPROM.updateBlock(adr, t_char,sizeof(s_dosingVal));
      EEPROM.updateByte(eepromAdrDosing, overwrite);
    break;
    // Temp => Format 4#30
    case 4:
      coolingTemp=fValue.toInt();
      EEPROM.updateInt(eepromTemp, coolingTemp);
      EEPROM.updateByte(eepromAdrTemp, overwrite);
    break;
    // PH => Format 5#7.00
    case 5:
      char p_char[sizeof(fValue)];
      fValue.toCharArray(p_char,sizeof(s_lightVal));
      phValue=atof(p_char);
      EEPROM.updateDouble(eepromPH, phValue);
      EEPROM.updateByte(eepromAdrPH, overwrite);
    break;
    // RELAY => Format 6#0:00:00=0,11:00:00=1,13:00:00=0,20:00:00=1,22:00:00=0,23:30:00=1
    case 6:
      writeRelayArr(slitString(input,'#',1),0);
      char r_char[sizeof(fValue)];
      fValue.toCharArray(r_char,sizeof(s_lightVal));
      EEPROM.updateBlock(eepromRelay,  r_char,sizeof(s_relayVal));
      EEPROM.updateByte(eepromAdrRelay, overwrite);
    break;
  }
    
}

//Help function for slitting array
String slitString(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


