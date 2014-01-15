void setDosing(int Value){
  resetPumps();
    for( int i=0;i <= PUMPCOUNTS; i++){
      if(dosing[i].active!=0){
            
        if(rtc.daystamp>=dosing[i].time && digitalRead(dosing[i].pinAddr)==LOW && dosing[i].status==false){
          t.pulse(dosing[i].pinAddr, 60 * dosing[i].mldosing * 1000 / dosing[i].mlperminute, LOW);
          dosing[i].status=true;
        }
      }
    }
}

void resetPumps(){
  if(rtc.daystamp>200){
    pumpReset=true;
  }
  if(rtc.daystamp<100 && pumpReset==true){
    pumpReset=false;
    for( int i=0;i <= PUMPCOUNTS; i++){
        dosing[i].status=false;
    }
  }
}

void setPumpSettings(PUMP dosing[]){
  // Check loading of stored Light Values
  boolean loadSetting=EEPROM.readByte(1);
  String sArr = "";
      char t_char[sizeof(s_dosingVal)];
  for(int i=0;i<PUMPCOUNTS;i++){
    // Read String from EEPROM and Convert to light
    int adr = eepromDosing +(sizeof(s_dosingVal)*i);
    if(loadSetting==overwrite){
      EEPROM.readBlock<char>(adr, t_char, sizeof(s_dosingVal));
      String str(t_char);
      sArr= str;
      
    }else{
      s_dosingVal.toCharArray(t_char,sizeof(s_dosingVal));
      EEPROM.updateBlock(adr, t_char,sizeof(s_dosingVal));
      sArr=s_dosingVal;
    }
    
    // Split out Time
    String nActive = slitString(sArr,'=',0);
    // Split out Time
    String nName = slitString(sArr,'=',1);
    // Split out Time
    String nTime = slitString(sArr,'=',2);
    // Split Value
    String nVal = slitString(sArr,'=',3);
    // Split mlMin
    String nML = slitString(sArr,'=',4);
    // create ts
    String nTSh = slitString(nTime,':',0);
    String nTSm = slitString(nTime,':',1);
    int nTS = get_ts(nTSh.toInt(),nTSm.toInt(),0);
    // Copy to dosingArray
    dosing[i].active=nActive.toInt();
    dosing[i].name=nName;
    dosing[i].time=nTS;
    dosing[i].mldosing=nVal.toInt();
    dosing[i].mlperminute=nML.toInt();
  }
}
  
