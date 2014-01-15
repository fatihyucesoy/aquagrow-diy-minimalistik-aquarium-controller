// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup () {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(2013,8,1,22,00,00); // year, month, day, hour, min, sec
}

void loop () {
    rtc.now();
    
    Serial.print(rtc.year, DEC);
    Serial.print('/');
    Serial.print(rtc.month, DEC);
    Serial.print('/');
    Serial.print(rtc.day, DEC);
    Serial.print(' ');
    Serial.print(rtc.hour, DEC);
    Serial.print(':');
    Serial.print(rtc.minute, DEC);
    Serial.print(':');
    Serial.print(rtc.second, DEC);
    Serial.println();
    
    Serial.print('Seconds of day: ');
    Serial.print(rtc.daystamp, DEC);
    
    Serial.println();
    delay(1000);
}