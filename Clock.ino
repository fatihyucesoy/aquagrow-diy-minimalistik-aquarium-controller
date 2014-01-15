uint32_t get_ts(uint8_t h, uint8_t m, uint8_t s){
  uint32_t t;
  t = h;
  t *= 60;
  t += m;
  t *= 60;
  t += s;
  return t;
}

String retTime(){
  int t = rtc.daystamp;
 int ss = t % 60;
    t /= 60;
 int   mm = t % 60;
    t /= 60;
 int   hh = t % 24;

  String sReturn;
  
    sReturn = "";
    sReturn += hh;
    sReturn += ":";
  if (mm < 10) {      // correct minute if necessary
    sReturn += "0";
    sReturn += mm;
  }else{
    sReturn += mm;
  }
    sReturn += ":";
  if (ss < 10) {      // correct second if necessary
    sReturn += "0";
    sReturn += ss;
  }else{
    sReturn += ss;
  }
  return sReturn;
}
