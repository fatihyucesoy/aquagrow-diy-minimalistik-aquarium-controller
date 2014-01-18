void setFeeding(){
  resetFeeting();
    for( int i=0;i <= 2; i++){
      if(rtc.daystamp>=feeder_channels[i].time && feeder_channels[i].status==false){
        t.oscillate(FEEDER, 2000, LOW,feeder_channels[i].pushed);
        feeder_channels[i].status=true;
      }
    }
}

void resetFeeting(){
  if(rtc.daystamp>200){
    feederReset=true;
  }
  if(rtc.daystamp<100 && pumpReset==true){
    feederReset=false;
    for( int i=0;i <= 2; i++){
        feeder_channels[i].status=false;
    }
  }
}
void setFeedingSettings(){
  // Check loading of stored Light Values
  boolean loadSetting=EEPROM.readByte(eepromAdrFeeding);
  String sArr = "";
  char t_char[sizeof(s_feederVal)];
    // Read String from EEPROM and Convert to Relay
    if(loadSetting==overwrite){
      EEPROM.readBlock<char>(eepromFeeding, t_char, sizeof(s_feederVal));
      String str(t_char);
      sArr= str;
    }else{
      s_relayVal.toCharArray(t_char,sizeof(s_feederVal));
      EEPROM.updateBlock(eepromFeeding, t_char,sizeof(s_feederVal));
      sArr=s_feederVal;
    }
  writeFeedingArr(sArr);
  EEPROM.updateByte(eepromAdrFeeding, overwrite);
}

void writeFeedingArr(String sArr){
    
    for(int n=0;n<2;n++){
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
      feeder_channels[n].time=nTS;
      feeder_channels[n].pushed=nVal.toInt();
    }
}
