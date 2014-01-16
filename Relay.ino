int setRelay(){
  int curIndex=0;
  for(int n=0;n<8;n++){
    if(relay_channels[n].time < rtc.daystamp){
      curIndex=n;
    }else{
  }
}

    if(relay_channels[curIndex].state == true && digitalRead(RELAY2)==HIGH && use_relay){
        digitalWrite(RELAY2,LOW);
    }else if(relay_channels[curIndex].state == false && digitalRead(RELAY2)==LOW && use_relay){
        digitalWrite(RELAY2,HIGH);
    }
}

void setRelaySettings(){
  // Check loading of stored Light Values
  boolean loadSetting=EEPROM.readByte(4);
  String sArr = "";
  char t_char[sizeof(s_relayVal)];
  for(int i=0;i<6;i++){
    // Read String from EEPROM and Convert to light
    int adr = eepromRelay;
    if(loadSetting==overwrite){
      EEPROM.readBlock<char>(adr, t_char, sizeof(s_relayVal));
      String str(t_char);
      sArr= str;
    }else{
      s_relayVal.toCharArray(t_char,sizeof(s_relayVal));
      EEPROM.updateBlock(adr, t_char,sizeof(s_relayVal));
      sArr=s_relayVal;
    }
  writeRelayArr(sArr,i);
  }
  EEPROM.updateByte(4, overwrite);
}

void writeRelayArr(String sArr,int Index){
    
    for(int n=0;n<6;n++){
      // Split String in singel Structs
      String nStruc = slitString(sArr,',',n);
      // Split out Time
      String nTime = slitString(nStruc,'=',0);
      // Split Value
      String nVal = slitString(nStruc,'=',1);
      // create ts
      String nTSh = slitString(nTime,':',0);
      String nTSm = slitString(nTime,':',1);
      int nTS = get_ts(nTSh.toInt(),nTSm.toInt(),0);
      // Copy to LightArray
      relay_channels[Index].time=nTS;
      relay_channels[Index].state=nVal.toInt();
    }
}
