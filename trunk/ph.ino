void Arduino_Control(){
  if(startup==0){ 
  PHserial.print("e\r");
  delay(50); 
  PHserial.print("e\r");
  delay(50);
  startup=1; 
 } 
  delay(800); 
  PHserial.print("R\r");
  if(string_received==1){
  ph=atof(ph_data);
  string_received=0;} 
} 
void s_cal(){
  PHserial.print("s\r");
} 
 
//here are some functions you might find useful
//these functions are not enabled
//calibrate to a pH of 7
//send the "s" command to calibrate to a pH of 7.00
void f_cal(){ 
  PHserial.print("f\r");
}
//calibrate to a pH of 4
//send the "f" command to calibrate to a pH of 4.00
void t_cal(){ 
  PHserial.print("t\r");
}
//calibrate to a pH of 10
//send the "t" command to calibrate to a pH of 10.00
void phFactoryDefault(){
  PHserial.print("X\r");
} 
void read_info(){
  PHserial.print("I\r");
}
void phSetLEDs(byte enabled){
  if(enabled){
    PHserial.print("L1\r");
  }else{
    PHserial.print("L0\r");
  }
}
