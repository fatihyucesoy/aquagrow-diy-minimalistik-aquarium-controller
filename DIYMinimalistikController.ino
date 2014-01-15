#include <Wire.h>
#include <RTClib.h>
#include <PCA9685.h>
#include <avr/pgmspace.h>
#include <EEPROMex.h>
#include <Timer.h>
#include <OneWire.h>
#include <MemoryFree.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "structs.h"
/*
String lightVal[] ={
    "0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0",
    "0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0",
    "0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0",
    "0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0",
    "0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0",
    "0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0",
    "0:01=0,10:30=0,11:00=10,11:30=0,21:00=0,21:10=0,22:00=0,23:30=0", //Rot
    "0:01=2,10:30=2,14:30=0,20:00=30,21:30=0,22:00=20,23:30=10,23:55=2"  // Blau
};*/
String s_lightVal ="0:01=0,11:00=0,13:00=50,20:00=50,22:00=0,23:30=0,23:30=0,23:32=0";
/*
String dosingVal[] ={
    "1=EI=8:30=5=60",
    "1=Eisen=11:00=2=60",
    "1=Flowgrow=13:00=2=60",
    "1=Easycarbo=16:00=5=60"
};*/
String s_dosingVal = "1=NPK=10:00=5=60";

boolean overwrite=1;

int coolingTemp = 30;
boolean show_ph = false;


#define rx 2
#define tx 3
#define PIN_TEMP 13  // Temperatur
#define PIN_PWM 12  // PWM PIN    // Lüfter
#define DOSE1 11  // Dosierpumpe 
#define DOSE2 10      // PWM PIN    // Dosierpumpe 
#define DOSE3 9      // Dosierpumpe 
#define DOSE4 8      // Dosierpumpe 
#define PUMPCOUNTS 3      // Number Pumps 

int dosingPins[PUMPCOUNTS]={DOSE1,DOSE2,DOSE3};

PUMP dosing[PUMPCOUNTS] = { 
{0,0,"",dosingPins[0],false,0,0},
{0,0,"",dosingPins[1],false,0,0}, 
{0,0,"",dosingPins[2],false,0,0}
};

Timer t;
PCA9685 ledDriver; 
RTC_DS1307 rtc;
OneWire  ds(PIN_TEMP);
SoftwareSerial PHserial(rx, tx);
LiquidCrystal_I2C lcd(0x20);  // Set the LCD I2C address
int stringStart, stringStop = 0;
int scrollCursor = 16;
String lightPercent= "";

LIGHT light_channels[8][8];

boolean manualLight=false;
boolean pumpReset=false;


PROGMEM prog_uint16_t pwmtable[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,
          3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,13,13,13,
          13,13,14,14,14,14,14,15,15,15,15,15,16,16,16,16,17,17,17,17,17,18,18,18,18,19,19,19,19,19,20,20,20,20,21,21,21,21,22,22,22,22,23,23,23,23,24,24,24,25,25,25,25,26,26,26,26,27,
          27,27,28,28,28,28,29,29,29,30,30,30,31,31,31,31,32,32,32,33,33,33,34,34,34,35,35,35,36,36,36,37,37,37,38,38,38,39,39,39,40,40,40,41,41,42,42,42,43,43,43,44,44,44,45,45,46,46,
          46,47,47,47,48,48,49,49,49,50,50,51,51,51,52,52,53,53,53,54,54,55,55,56,56,56,57,57,58,58,59,59,59,60,60,61,61,62,62,63,63,63,64,64,65,65,66,66,67,67,68,68,69,69,69,70,70,71,
          71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,84,84,85,85,86,86,87,87,88,88,89,89,90,91,91,92,92,93,93,94,94,95,96,96,97,97,98,98,99,100,100,101,101,
          102,102,103,104,104,105,105,106,107,107,108,108,109,110,110,111,112,112,113,113,114,115,115,116,116,117,118,118,119,120,120,121,122,122,123,124,124,125,126,126,127,127,128,129,
          129,130,131,132,132,133,134,134,135,136,136,137,138,138,139,140,140,141,142,143,143,144,145,145,146,147,148,148,149,150,150,151,152,153,153,154,155,156,156,157,158,159,159,160,
          161,162,162,163,164,165,165,166,167,168,168,169,170,171,171,172,173,174,175,175,176,177,178,179,179,180,181,182,183,183,184,185,186,187,187,188,189,190,191,192,192,193,194,195,
          196,197,197,198,199,200,201,202,203,203,204,205,206,207,208,209,209,210,211,212,213,214,215,216,216,217,218,219,220,221,222,223,224,224,225,226,227,228,229,230,231,232,233,234,
          235,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,
          277,278,279,280,281,282,283,284,285,286,287,288,289,290,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,311,312,313,314,315,316,317,318,319,320,321,323,
          324,325,326,327,328,329,330,331,333,334,335,336,337,338,339,340,342,343,344,345,346,347,348,350,351,352,353,354,355,356,358,359,360,361,362,363,365,366,367,368,369,371,372,373,
          374,375,377,378,379,380,381,383,384,385,386,387,389,390,391,392,393,395,396,397,398,400,401,402,403,405,406,407,408,410,411,412,413,415,416,417,418,420,421,422,424,425,426,427,
          429,430,431,433,434,435,436,438,439,440,442,443,444,446,447,448,449,451,452,453,455,456,457,459,460,461,463,464,465,467,468,469,471,472,474,475,476,478,479,480,482,483,484,486,
          487,489,490,491,493,494,496,497,498,500,501,502,504,505,507,508,510,511,512,514,515,517,518,519,521,522,524,525,527,528,530,531,532,534,535,537,538,540,541,543,544,545,547,548,
          550,551,553,554,556,557,559,560,562,563,565,566,568,569,571,572,574,575,577,578,580,581,583,584,586,587,589,590,592,593,595,597,598,600,601,603,604,606,607,609,610,612,614,615,
          617,618,620,621,623,625,626,628,629,631,633,634,636,637,639,640,642,644,645,647,649,650,652,653,655,657,658,660,661,663,665,666,668,670,671,673,675,676,678,679,681,683,684,686,
          688,689,691,693,694,696,698,699,701,703,705,706,708,710,711,713,715,716,718,720,721,723,725,727,728,730,732,733,735,737,739,740,742,744,746,747,749,751,753,754,756,758,760,761,
          763,765,767,768,770,772,774,775,777,779,781,783,784,786,788,790,792,793,795,797,799,801,802,804,806,808,810,811,813,815,817,819,821,822,824,826,828,830,832,833,835,837,839,841,
          843,845,846,848,850,852,854,856,858,859,861,863,865,867,869,871,873,875,876,878,880,882,884,886,888,890,892,894,896,897,899,901,903,905,907,909,911,913,915,917,919,921,923,925,
          927,929,930,932,934,936,938,940,942,944,946,948,950,952,954,956,958,960,962,964,966,968,970,972,974,976,978,980,982,984,986,988,990,992,994,996,998,1000,1003,1005,1007,1009,1011,
          1013,1015,1017,1019,1021,1023,1025,1027,1029,1031,1033,1035,1038,1040,1042,1044,1046,1048,1050,1052,1054,1056,1058,1061,1063,1065,1067,1069,1071,1073,1075,1078,1080,1082,1084,1086,
          1088,1090,1092,1095,1097,1099,1101,1103,1105,1107,1110,1112,1114,1116,1118,1120,1123,1125,1127,1129,1131,1134,1136,1138,1140,1142,1144,1147,1149,1151,1153,1155,1158,1160,1162,1164,
          1167,1169,1171,1173,1175,1178,1180,1182,1184,1187,1189,1191,1193,1196,1198,1200,1202,1205,1207,1209,1211,1214,1216,1218,1220,1223,1225,1227,1230,1232,1234,1236,1239,1241,1243,1246,
          1248,1250,1252,1255,1257,1259,1262,1264,1266,1269,1271,1273,1276,1278,1280,1283,1285,1287,1290,1292,1294,1297,1299,1301,1304,1306,1308,1311,1313,1316,1318,1320,1323,1325,1327,1330,
          1332,1335,1337,1339,1342,1344,1347,1349,1351,1354,1356,1359,1361,1363,1366,1368,1371,1373,1376,1378,1380,1383,1385,1388,1390,1393,1395,1397,1400,1402,1405,1407,1410,1412,1415,1417,
          1420,1422,1425,1427,1429,1432,1434,1437,1439,1442,1444,1447,1449,1452,1454,1457,1459,1462,1464,1467,1469,1472,1474,1477,1480,1482,1485,1487,1490,1492,1495,1497,1500,1502,1505,1507,
          1510,1513,1515,1518,1520,1523,1525,1528,1530,1533,1536,1538,1541,1543,1546,1549,1551,1554,1556,1559,1562,1564,1567,1569,1572,1575,1577,1580,1582,1585,1588,1590,1593,1596,1598,1601,
          1603,1606,1609,1611,1614,1617,1619,1622,1625,1627,1630,1633,1635,1638,1641,1643,1646,1649,1651,1654,1657,1659,1662,1665,1667,1670,1673,1676,1678,1681,1684,1686,1689,1692,1695,1697,
          1700,1703,1705,1708,1711,1714,1716,1719,1722,1725,1727,1730,1733,1736,1738,1741,1744,1747,1749,1752,1755,1758,1760,1763,1766,1769,1772,1774,1777,1780,1783,1786,1788,1791,1794,1797,
          1800,1802,1805,1808,1811,1814,1816,1819,1822,1825,1828,1831,1833,1836,1839,1842,1845,1848,1851,1853,1856,1859,1862,1865,1868,1871,1873,1876,1879,1882,1885,1888,1891,1894,1896,1899,
          1902,1905,1908,1911,1914,1917,1920,1923,1926,1928,1931,1934,1937,1940,1943,1946,1949,1952,1955,1958,1961,1964,1967,1970,1972,1975,1978,1981,1984,1987,1990,1993,1996,1999,2002,2005,
          2008,2011,2014,2017,2020,2023,2026,2029,2032,2035,2038,2041,2044,2047,2050,2053,2056,2059,2062,2065,2068,2071,2074,2077,2080,2083,2086,2090,2093,2096,2099,2102,2105,2108,2111,2114,
          2117,2120,2123,2126,2129,2132,2136,2139,2142,2145,2148,2151,2154,2157,2160,2163,2166,2170,2173,2176,2179,2182,2185,2188,2191,2195,2198,2201,2204,2207,2210,2213,2217,2220,2223,2226,
          2229,2232,2235,2239,2242,2245,2248,2251,2254,2258,2261,2264,2267,2270,2274,2277,2280,2283,2286,2290,2293,2296,2299,2302,2306,2309,2312,2315,2319,2322,2325,2328,2331,2335,2338,2341,
          2344,2348,2351,2354,2357,2361,2364,2367,2370,2374,2377,2380,2384,2387,2390,2393,2397,2400,2403,2407,2410,2413,2416,2420,2423,2426,2430,2433,2436,2440,2443,2446,2450,2453,2456,2460,
          2463,2466,2470,2473,2476,2480,2483,2486,2490,2493,2496,2500,2503,2506,2510,2513,2517,2520,2523,2527,2530,2533,2537,2540,2544,2547,2550,2554,2557,2561,2564,2567,2571,2574,2578,2581,
          2584,2588,2591,2595,2598,2602,2605,2609,2612,2615,2619,2622,2626,2629,2633,2636,2640,2643,2647,2650,2653,2657,2660,2664,2667,2671,2674,2678,2681,2685,2688,2692,2695,2699,2702,2706,
          2709,2713,2716,2720,2723,2727,2730,2734,2738,2741,2745,2748,2752,2755,2759,2762,2766,2769,2773,2777,2780,2784,2787,2791,2794,2798,2801,2805,2809,2812,2816,2819,2823,2827,2830,2834,
          2837,2841,2845,2848,2852,2855,2859,2863,2866,2870,2874,2877,2881,2884,2888,2892,2895,2899,2903,2906,2910,2914,2917,2921,2925,2928,2932,2936,2939,2943,2947,2950,2954,2958,2961,2965,
          2969,2972,2976,2980,2983,2987,2991,2995,2998,3002,3006,3009,3013,3017,3021,3024,3028,3032,3036,3039,3043,3047,3051,3054,3058,3062,3066,3069,3073,3077,3081,3084,3088,3092,3096,3099,
          3103,3107,3111,3115,3118,3122,3126,3130,3134,3137,3141,3145,3149,3153,3156,3160,3164,3168,3172,3176,3179,3183,3187,3191,3195,3199,3202,3206,3210,3214,3218,3222,3226,3229,3233,3237,
          3241,3245,3249,3253,3257,3260,3264,3268,3272,3276,3280,3284,3288,3292,3295,3299,3303,3307,3311,3315,3319,3323,3327,3331,3335,3339,3343,3346,3350,3354,3358,3362,3366,3370,3374,3378,
          3382,3386,3390,3394,3398,3402,3406,3410,3414,3418,3422,3426,3430,3434,3438,3442,3446,3450,3454,3458,3462,3466,3470,3474,3478,3482,3486,3490,3494,3498,3502,3506,3510,3514,3518,3522,
          3526,3530,3535,3539,3543,3547,3551,3555,3559,3563,3567,3571,3575,3579,3583,3588,3592,3596,3600,3604,3608,3612,3616,3620,3624,3629,3633,3637,3641,3645,3649,3653,3658,3662,3666,3670,
          3674,3678,3682,3687,3691,3695,3699,3703,3707,3711,3716,3720,3724,3728,3732,3737,3741,3745,3749,3753,3758,3762,3766,3770,3774,3779,3783,3787,3791,3795,3800,3804,3808,3812,3817,3821,
          3825,3829,3834,3838,3842,3846,3851,3855,3859,3863,3868,3872,3876,3880,3885,3889,3893,3897,3902,3906,3910,3915,3919,3923,3928,3932,3936,3940,3945,3949,3953,3958,3962,3966,3971,3975,
          3979,3984,3988,3992,3997,4001,4005,4010,4014,4018,4023,4027,4031,4036,4040,4045,4049,4053,4058,4062,4066,4071,4075,4080,4084,4088,4093};

//WasserTemperatur
float temperatur;

unsigned long last_print = 0;
unsigned int switch_print=0;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// ph
char ph_data[20];
char computerdata[20];
byte received_from_computer=0;
byte received_from_sensor=0;
byte arduino_only=0;
byte startup=0;
float ph=0;
byte string_received=0;
const int maxAllowedWrites = 1000;
const int memBase          = 4;
int eepromLight= memBase;
int eepromDosing= eepromLight+(sizeof(s_lightVal)*8);
int eepromTemp= eepromDosing+(sizeof(s_dosingVal)*PUMPCOUNTS);

          
void setup() {

  // start reading from position memBase (address 0) of the EEPROM. Set maximumSize to EEPROMSizeUno 
  // Writes before membase or beyond EEPROMSizeUno will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMemPool(memBase, EEPROMSizeUno);
  // Set maximum allowed writes to maxAllowedWrites. 
  // More writes will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMaxAllowedWrites(maxAllowedWrites);
  
  if(EEPROM.readByte(2)==overwrite){
    coolingTemp=EEPROM.readInt(eepromTemp);
  }else{
    EEPROM.updateInt(eepromTemp, coolingTemp);
    EEPROM.updateByte(2, overwrite);
  }
  
  setLightSettings(light_channels);
  setPumpSettings(dosing);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  PHserial.begin(38400);
  
  
  Serial.begin(9600);    // Ausgabe am PC
  
  pinMode( PIN_PWM,OUTPUT);  // PWM PIN    // Lüfter
  
  for (int i=0;i<=PUMPCOUNTS;i++){
    pinMode(dosing[i].pinAddr, OUTPUT);
  }
  
  digitalWrite(PIN_PWM, LOW);
  
  Wire.begin();           // Wire must be started!
  rtc.begin();

  
  ledDriver.begin(B000000);  // Address pins A5-A0 set to B111000
  ledDriver.init();
  lcd.begin(16,2);               // initialize the lcd 
  byte Celsius[8] = {B11100,B10100,B11100,B0000,B00000,B00000,B00000,B00000};
  lcd.createChar(0, Celsius);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    serialHandler(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  rtc.now();
  t.update();
  setDosing(0);
  if(PHserial.available() > 0){
   received_from_sensor=PHserial.readBytesUntil(13,ph_data,20);
   ph_data[received_from_sensor]=0;
   string_received=1;
  } 
  if(arduino_only==1){
    Arduino_Control();
  }
  
  if (millis() - last_print > 1000) {
    last_print = millis();
    switch_print++;
    
    // Display ausgabe Obere Zeile
    lcd.setCursor ( 0, 0 ); 
    lcd.print (retTime());
    if (switch_print <= 5){
      switch_print = millis();
      lcd.setCursor ( 9, 0 ); 
      lcd.print ("C=");
      lcd.print (getTemp());
      lcd.write ((uint8_t)0);
    }else if (switch_print < 10 && show_ph==true){
      lcd.setCursor ( 9, 0 ); 
      lcd.print ("ph=");
      lcd.print (ph_data);
    }else{
      switch_print=0;
    }
      
    
    // Display ausgabe Untere Zeile
    lcd.setCursor(scrollCursor, 1);
    lcd.print(lightPercent.substring(stringStart,stringStop));
    if(stringStart == 0 && scrollCursor > 0){
      scrollCursor--;
      stringStop++;
    } else if (stringStart == stringStop){
      stringStart = stringStop = 0;
      scrollCursor = 16;
    } else if (stringStop == lightPercent.length() && scrollCursor == 0) {
      stringStart++;
    } else {
      stringStart++;
      stringStop++;
    }
  
    if(!manualLight){
      setLight();
    }
//    Serial.println(freeMemory());
    temperatur = getTemp();
    if(coolingTemp+2.0 < temperatur){
        analogWrite(PIN_PWM,255);
    }else if(coolingTemp-2.0 > temperatur){
        analogWrite(PIN_PWM,0);
    }
  }            
}

float getTemp(){
  byte present = 0;
  //byte type_s = 0;
  byte data[12];
  byte addr[8];
  ds.search(addr);
      
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for (byte i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  
  //byte cfg = (data[4] & 0x60);
  return (float)raw / 16.0; //celsius;
}
