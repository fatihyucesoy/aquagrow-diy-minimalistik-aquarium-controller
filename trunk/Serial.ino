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
  int fIndex = slitString(input,':',0).toInt();
  String fValue= slitString(input,':',1);
  switch(fIndex){
    // Time to set => Format DD/MM/YYYY/H/i
    case 1:
      rtc.adjust(slitString(fValue,'/',2).toInt(),slitString(fValue,'/',1).toInt(),slitString(fValue,'/',0).toInt(),slitString(fValue,'/',3).toInt(),slitString(fValue,'/',4).toInt(),0);
    break;
    // Light Chanel
    case 2:
    break;
    // Dosing
    case 3:
    break;
    // Temp
    case 4:
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


