
#define PUMPCOUNTS 6      // Number Pumps
#define LIGHT_CHANEL 8    
#define LIGHT_VALUES 10
#define RELAY_TIMES 6      // Number Pumps
#define KEYPADI2C 0x21
// PIN MAPPING
#define rx 2
#define tx 3
//#define KC1 2
//#define KC2 3
//#define KC3 4
//#define KC4 5
#define RELAY2 6
#define RELAY1 7      // PH Steckdose 
#define DOSE4 8      // Dosierpumpe 
#define DOSE3 9      // Dosierpumpe 
#define DOSE2 10      // PWM PIN    // Dosierpumpe 
#define DOSE1 11  // Dosierpumpe 
#define PIN_PWM 12  // PWM PIN    // Lüfter
#define PIN_TEMP 13  // Temperatur
//#define KR1 14
//#define KR2 15
//#define KR3 16
//#define KR4 17


String s_lightVal ="0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0,23:30=0,23:32=0";
String s_dosingVal = "1=NPK=10:00=5=60";
String s_relayVal ="0:00:00=0,11:00:00=1,13:00:00=0,20:00:00=1,22:00:00=0,23:30:00=1";

boolean overwrite=1;

int coolingTemp = 30;
double phValue = 7.00;
boolean show_ph = false;
boolean use_relay = false;




int dosingPins[PUMPCOUNTS]={DOSE1,DOSE2,DOSE3};

PUMP dosing[PUMPCOUNTS] = {  
{0,0,"",dosingPins[0],false,0,0},
{0,0,"",dosingPins[0],false,0,0},
{0,0,"",dosingPins[1],false,0,0}, 
{0,0,"",dosingPins[1],false,0,0}, 
{0,0,"",dosingPins[2],false,0,0}, 
{0,0,"",dosingPins[2],false,0,0}
};
