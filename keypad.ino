

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
  int chanel;
  boolean s_Light=false,lVal=false;
    switch (keypad.getState()){
    case PRESSED:
        if (key == '1') {
          s_Light=true;
          lVal=true;
          chanel=0;
        }else if (key == '2') {
          s_Light=true;
          lVal=true;
          chanel=1;
        }else if (key == '3') {
          s_Light=true;
          lVal=true;
          chanel=2;
        }else if (key == '4') {
          s_Light=true;
          lVal=true;
          chanel=3;
        }else if (key == '5') {
          s_Light=true;
          lVal=true;
          chanel=4;
        }else if (key == '6') {
          s_Light=true;
          lVal=true;
          chanel=5;
        }else if (key == '7') {
          s_Light=true;
          lVal=true;
          chanel=6;
        }else if (key == '8') {
          s_Light=true;
          lVal=true;
          chanel=7;
        }else if (key == '9') {
          manualLight=false;
        }else if (key == '0') {
          s_cal();    //calibrate to a pH of 7
        }else if (key == 'A') {
          t.pulse(dosing[0].pinAddr,60000 / dosing[0].mlperminute, LOW);
        }else if (key == 'B') {
          t.pulse(dosing[1].pinAddr,60000 / dosing[1].mlperminute, LOW);
        }else if (key == 'C') {
          t.pulse(dosing[2].pinAddr,60000 / dosing[2].mlperminute, LOW);
        }else if (key == 'D') {
        }else if (key == '*') {
          f_cal();    //calibrate to a pH of 4
        }else if (key == '#') {
          t_cal();    //calibrate to a pH of 10
        }else 
        break;
        
        
    case HOLD:
        if (key == '1') {
          s_Light=true;
          chanel=0;
        }else if (key == '2') {
          s_Light=true;
          chanel=1;
        }else if (key == '3') {
          s_Light=true;
          chanel=2;
        }else if (key == '4') {
          s_Light=true;
          chanel=3;
        }else if (key == '5') {
          s_Light=true;
          chanel=4;
        }else if (key == '6') {
          s_Light=true;
          chanel=5;
        }else if (key == '7') {
          s_Light=true;
          chanel=6;
        }else if (key == '8') {
          s_Light=true;
          chanel=7;
        }else if (key == '9') {
          manualLight=false;
        }
        break;
    }
    if(s_Light && lVal){
      setLED(chanel*2,2024);
      setLED(chanel*2+1,2024);
      manualLight=true;
    }else if(s_Light && !lVal){
      setLED(chanel*2,0);
      setLED(chanel*2+1,0);
      manualLight=true;
    } 
}
