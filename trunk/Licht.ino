void setLight(){
  lightPercent="";
    for( int i=0;i <  8; i++){
        uint16_t c_PWM = PWM_Licht(i);
        
        // 40° = 100% ~ 45/40*100-110=2,5
        float cTemp = temperatur*100/coolingTemp-110;
        if(cTemp>0.0){
          c_PWM= uint16_t(float(c_PWM/(100+cTemp*4))*100);
        }        
        
        if(c_PWM<0){
          c_PWM=0;
        }else if(c_PWM>4095){
          c_PWM=4095;
        }
        int lP = c_PWM/100/4095;
        lightPercent+=i;
        lightPercent+=":";
        lightPercent+=lP;
        lightPercent+=" ";
          
        setLED(i*2,uint16_t(c_PWM/2));
        setLED(i*2+1,uint16_t(c_PWM/2));
    }
}


void setLED(uint8_t channel, uint16_t Value){
          
          uint16_t n_PWM;  
          if(Value>2048){Value=Value/2;}
          // read back a 2-byte int
          n_PWM = pgm_read_word(&pwmtable[Value]);
          /*
  Serial.print("channel ");
  Serial.println(channel);
  Serial.print("   Value ");
  Serial.print(Value);
  Serial.print("   n_PWM ");
  Serial.println(n_PWM);
  */
          ledDriver.writeLED(channel,0, n_PWM);
}


int PWM_Licht(int lightIndex){
  
  int curIndex=0;
  for(int n=0;n<8;n++){
    if(light_channels[lightIndex][n].time < rtc.daystamp){
      curIndex=n;
    }else{
  }
}
/*
  Serial.print("index ");
  Serial.println(lightIndex);
  Serial.print("daystamp ");
  Serial.println(light_channels[lightIndex][curIndex].time);
  Serial.print("curIndex ");
  Serial.println(curIndex);
*/

  uint16_t Max,Min,pwm=4095;
  int dimTime,f,p;
  float pastSeconds;
  uint32_t Start,Ende;
  uint8_t oMin,oMax;
    if(curIndex ==7 ){
        Start = light_channels[lightIndex][7].time;
        Ende = light_channels[lightIndex][0].time;
        oMin = light_channels[lightIndex][7].level;
        oMax = light_channels[lightIndex][0].level;
        
        pastSeconds = rtc.daystamp-Start+0.5;    // vergangene Sekunden ~1616Sek ~ 27min
        dimTime= get_ts(24,0,0) - Start + Ende;
      
    }else if(curIndex>=1 || light_channels[lightIndex][0].time < rtc.daystamp){
        Start = light_channels[lightIndex][curIndex].time;
        Ende = light_channels[lightIndex][curIndex+1].time;
        oMin = light_channels[lightIndex][curIndex].level;
        oMax = light_channels[lightIndex][curIndex+1].level;
        
        pastSeconds = rtc.daystamp-Start+0.5;    // vergangene Sekunden ~1616Sek ~ 27min
        dimTime=Ende - Start;
    }else{
        Start = light_channels[lightIndex][7].time;
        Ende = light_channels[lightIndex][0].time;
        oMin = light_channels[lightIndex][7].level;
        oMax = light_channels[lightIndex][0].level;
      
        pastSeconds = get_ts(24,0,0)-Start + rtc.daystamp+0.5; 
        dimTime= get_ts(24,0,0)-Start + Ende;
    }
      
        
        Min=uint16_t(pwm-(pwm/100*oMin));  // 0%=4095-(4095/100*0) = 
        Max=uint16_t(pwm-(pwm/100*oMax)); // 80% von 4095-(4095/100*80) sind 819
//  Serial.print("Min ");
//  Serial.println(Min);
//  Serial.print("Max ");
//  Serial.println(Max);

        if(Min==Max){
            return Min;
        }  
        f= dimTime/int(Max-Min);    // 1800/2800=0,64
        p = pastSeconds/f;    // 1616 / 0,64=2525   
        pwm=Min+p;
 /*
  Serial.println();
  Serial.print("index ");
  Serial.println(lightIndex);
  Serial.print("Start ");
  Serial.println(Start);
  Serial.print("oMax ");
  Serial.println(oMax);  
  Serial.print("pastSeconds ");
  Serial.println(pastSeconds);
  Serial.print("dimTime ");
  Serial.println(dimTime);
  Serial.print("f ");
  Serial.println(f);
  Serial.print("uint16_t(Max-Min) ");
  Serial.println(uint16_t(Max-Min));
  Serial.print("Min ");
  Serial.println(Min);
  Serial.print("p ");
  Serial.println(p);
  Serial.print("Starten ");
  Serial.println(pwm);
  */
      return pwm;    // Im Nofall ausschalten...
  /*      
  Serial.println();
  Serial.print("index ");
  Serial.println(lightIndex);
  Serial.print("Start ");
  Serial.println(Start);
  Serial.print("oMax ");
  Serial.println(oMax);  
  Serial.print("pastSeconds ");
  Serial.println(pastSeconds);
  Serial.print("dimTime ");
  Serial.println(dimTime);
  Serial.print("f ");
  Serial.println(f);
  Serial.print("uint16_t(Max-Min) ");
  Serial.println(uint16_t(Max-Min));
  Serial.print("Min ");
  Serial.println(Min);
  Serial.print("p ");
  Serial.println(p);
  Serial.print("Starten ");
  Serial.println(pwm);
*/
/*
  Serial.print("index ");
  Serial.println(lightIndex);
  Serial.print("daystamp ");
  Serial.println(rtc.daystamp);
  Serial.print("Start ");
  Serial.println(Start);
  Serial.print("Dim_in ");
  Serial.println(Dim_in);
  Serial.print("oMax ");
  Serial.println(oMax);
  
  Serial.print("s ");
  Serial.println(s);
  Serial.print("f ");
  Serial.println(f);
  Serial.print("uint16_t(Dim_in*60) ");
  Serial.println(uint16_t(Dim_in*60));
  Serial.print("uint16_t(Max-Min) ");
  Serial.println(uint16_t(Max-Min));
  Serial.print("Min ");
  Serial.println(Min);
  Serial.print("p ");
  Serial.println(p);
  Serial.print("Starten ");
  Serial.println(pwm);
  */
    
}

void setLightSettings(){
  // Check loading of stored Light Values
  boolean loadSetting=EEPROM.readByte(0);
  String sArr = "";
      char t_char[sizeof(s_lightVal)];
  for(int i=0;i<8;i++){
    // Read String from EEPROM and Convert to light
    int adr = eepromLight +(sizeof(s_lightVal)*i);
    if(loadSetting==overwrite){
      EEPROM.readBlock<char>(adr, t_char, sizeof(s_lightVal));
      String str(t_char);
      sArr= str;
    }else{
      s_lightVal.toCharArray(t_char,sizeof(s_lightVal));
      EEPROM.updateBlock(adr, t_char,sizeof(s_lightVal));
      sArr=s_lightVal;
    }
    writeLightArr(sArr,i);
  }
  EEPROM.updateByte(0, overwrite);
}

void writeLightArr(String sArr,int Index){
    
    for(int n=0;n<8;n++){
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
      light_channels[Index][n].time=nTS;
      light_channels[Index][n].level=nVal.toInt();
    }
}
