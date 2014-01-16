int setRelay(){
  int curIndex=0;
  for(int n=0;n<RELAY_TIMES;n++){
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
  boolean loadSetting=EEPROM.readByte(eepromAdrRelay);
  String sArr = "";
  char t_char[sizeof(s_relayVal)];
    // Read String from EEPROM and Convert to Relay
    if(loadSetting==overwrite){
      EEPROM.readBlock<char>(eepromRelay, t_char, sizeof(s_relayVal));
      String str(t_char);
      sArr= str;
    }else{
      s_relayVal.toCharArray(t_char,sizeof(s_relayVal));
      EEPROM.updateBlock(eepromRelay, t_char,sizeof(s_relayVal));
      sArr=s_relayVal;
    }
  writeRelayArr(sArr,0);
  EEPROM.updateByte(eepromAdrRelay, overwrite);
}

void writeRelayArr(String sArr,int Index){
    
    for(int n=0;n<RELAY_TIMES;n++){
      // Split String in singel Structs
      String nStruc = slitString(sArr,',',n);
      // Split out Time
      String nTime = slitString(nStruc,'=',0);
      // Split Value
      String nVal = slitString(nStruc,'=',1);
      // create ts
      String nTSh = slitString(nTime,':',0);
      String nTSm = slitString(nTime,':',1);
      String nTSs = slitString(nTime,':',2);
      int nTS = get_ts(nTSh.toInt(),nTSm.toInt(),nTSs.toInt());
      // Copy to LightArray
      relay_channels[n].time=nTS;
      relay_channels[n].state=nVal.toInt();
    }
}
