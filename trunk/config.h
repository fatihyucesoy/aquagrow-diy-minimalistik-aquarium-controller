
#define PUMPCOUNTS 6      // Number Pumps
#define LIGHT_CHANEL 8    
#define LIGHT_VALUES 10
#define RELAY_TIMES 6
#define KEYPADI2C 0x21
// PIN MAPPING
#define rx 2
#define tx 3
//#define KC1 2
//#define KC2 3
//#define KC3 4
#define FEEDER 5
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


// Aufbau: 10 werte mit je Uhrzeit = % Helligkeit getrennt mit ,
String s_lightVal ="0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0,23:30=0,23:32=0";
// Aufbau Aktive (ja:1, nein:0) = Name = Uhrzeit = ml Menge = Kalibrierung pumpe (ml/Minute)
String s_dosingVal = "1=NPK=10:00=5=60";
// Aufbau: 6 Zeiten mit anschalten(1) oder ausschalten (0) getrennt mit , 
String s_relayVal ="0:00:00=0,11:00:00=1,13:00:00=0,20:00:00=1,22:00:00=0,23:30:00=1";
// Aufbau: 2 Uhrzeiten mit Anzahl Klicks getrennt mit , 
String s_feederVal ="0:00=2,11:00=1";

// Temperatur ab Wann Lüfter an gehen (Ganzzahl)
int coolingTemp = 30;
// ph Weert wo Relay schaltet (decimalzahl mit . "punkt")
double phValue = 7.00;

// Soll PH im Display angezeigt werden
boolean show_ph = false;
// Soll 2tes relay als Steckdose benutzt werden
boolean use_relay = false;



// Ab hier sind keine Standart einstellungen. Nur anpassen wenn ihr wisst was ihr macht
boolean overwrite=1;

// Neu zuordnung der Pins für Dosierung
int dosingPins[]={DOSE1,DOSE2,DOSE3};

// Pumpen Einstellungen mit zuordnung der Pins
PUMP dosing[] = {  
{0,0,"",dosingPins[0],false,0,0},
{0,0,"",dosingPins[0],false,0,0},
{0,0,"",dosingPins[1],false,0,0}, 
{0,0,"",dosingPins[1],false,0,0}, 
{0,0,"",dosingPins[2],false,0,0}, 
{0,0,"",dosingPins[2],false,0,0}
};

// Hier NIX ändern!!!!
LIGHT light_channels[LIGHT_CHANEL][LIGHT_VALUES]={
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
        {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}
};

RELAY relay_channels[]={
                {0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
              };
              
FEED feeder_channels[]={
                {0,0},{0,0}
              };
