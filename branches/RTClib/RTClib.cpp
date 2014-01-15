// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#include <Wire.h>
#include "RTClib.h"
#ifdef __AVR__
 #include <avr/pgmspace.h>
 #define WIRE Wire
#else
 #define PROGMEM
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #define WIRE Wire1
#endif

#define DS1307_ADDRESS 0x68
#define SECONDS_PER_DAY 86400L

#define SECONDS_FROM_1970_TO_2000 946684800

#if (ARDUINO >= 100)
 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
#else
 #include <WProgram.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// utility code, some of this could be exposed in the DateTime API if needed
/*
const uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

// number of days since 2000/01/01, valid for 2001..2099
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}
static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

*/
////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second
/*
DateTime::DateTime (uint32_t t) {
  t -= SECONDS_FROM_1970_TO_2000;    // bring to 2000 timestamp from 1970

    ss = t % 60;
    t /= 60;
    mm = t % 60;
    t /= 60;
    hh = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (yOff = 0; ; ++yOff) {
        leap = yOff % 4 == 0;
        if (days < 365 + leap)
            break;
        days -= 365 + leap;
    }
    for (m = 1; ; ++m) {
        uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
        if (leap && m == 2)
            ++daysPerMonth;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }
    d = days + 1;
}
DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
}

DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint32_t daySec) {
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
	ds = daySec;
}
*/

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

/*
// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
DateTime::DateTime (const char* date, const char* time) {
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (date[0]) {
        case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
        case 'F': m = 2; break;
        case 'A': m = date[2] == 'r' ? 4 : 8; break;
        case 'M': m = date[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    d = conv2d(date + 4);
    hh = conv2d(time);
    mm = conv2d(time + 3);
    ss = conv2d(time + 6);
}
*/
/*
uint8_t DateTime::dayOfWeek() const {    
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}
*/
////////////////////////////////////////////////////////////////////////////////
// RTC_DS1307 implementation

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

uint8_t RTC_DS1307::begin(void) {
  return 1;
}


#if (ARDUINO >= 100)
/*
uint8_t RTC_DS1307::isrunning(void) {
  WIRE.beginTransmission(DS1307_ADDRESS);
  WIRE.write(0);
  WIRE.endTransmission();

  WIRE.requestFrom(DS1307_ADDRESS, 1);
  uint8_t ss = WIRE.read();
  return !(ss>>7);
}
*/
/*
void RTC_DS1307::adjust(const DateTime& dt) {
    WIRE.beginTransmission(DS1307_ADDRESS);
    WIRE.write(0);
    WIRE.write(bin2bcd(dt.second()));
    WIRE.write(bin2bcd(dt.minute()));
    WIRE.write(bin2bcd(dt.hour()));
    WIRE.write(bin2bcd(0));
    WIRE.write(bin2bcd(dt.day()));
    WIRE.write(bin2bcd(dt.month()));
    WIRE.write(bin2bcd(dt.year() - 2000));
    WIRE.write(0);
    WIRE.endTransmission();
}
*/
void RTC_DS1307::adjust(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    WIRE.beginTransmission(DS1307_ADDRESS);
    WIRE.write(0);
    WIRE.write(bin2bcd(sec));
    WIRE.write(bin2bcd(min));
    WIRE.write(bin2bcd(hour));
    WIRE.write(bin2bcd(0));
    WIRE.write(bin2bcd(day));
    WIRE.write(bin2bcd(month));
    WIRE.write(bin2bcd(year - 2000));
    WIRE.write(0);
    WIRE.endTransmission();
}

void RTC_DS1307::now() {

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
	
  WIRE.beginTransmission(DS1307_ADDRESS);
  WIRE.write(0);	
  WIRE.endTransmission();

  WIRE.requestFrom(DS1307_ADDRESS, 7);
  second = bcd2bin(WIRE.read() & 0x7F);
  minute = bcd2bin(WIRE.read());
  hour = bcd2bin(WIRE.read());
  WIRE.read();
  day = bcd2bin(WIRE.read());
  month = bcd2bin(WIRE.read());
  year = bcd2bin(WIRE.read()) + 2000;
  
  // Daystaamp Calc 
  daystamp = hour;
  daystamp *= 60;
  daystamp += minute;
  daystamp *= 60;
  daystamp += second;
}

#else
/*
uint8_t RTC_DS1307::isrunning(void) {
  WIRE.beginTransmission(DS1307_ADDRESS);
  WIRE.send(0);	
  WIRE.endTransmission();

  WIRE.requestFrom(DS1307_ADDRESS, 1);
  uint8_t ss = WIRE.receive();
  return !(ss>>7);
}
*/
/*
void RTC_DS1307::adjust(const DateTime& dt) {
    WIRE.beginTransmission(DS1307_ADDRESS);
    WIRE.send(0);
    WIRE.send(bin2bcd(dt.second()));
    WIRE.send(bin2bcd(dt.minute()));
    WIRE.send(bin2bcd(dt.hour()));
    WIRE.send(bin2bcd(0));
    WIRE.send(bin2bcd(dt.day()));
    WIRE.send(bin2bcd(dt.month()));
    WIRE.send(bin2bcd(dt.year() - 2000));
    WIRE.send(0);
    WIRE.endTransmission();
}

DateTime RTC_DS1307::now() {
  WIRE.beginTransmission(DS1307_ADDRESS);
  WIRE.send(0);	
  WIRE.endTransmission();
  
  WIRE.requestFrom(DS1307_ADDRESS, 7);
  uint8_t ss = bcd2bin(WIRE.receive() & 0x7F);
  uint8_t mm = bcd2bin(WIRE.receive());
  uint8_t hh = bcd2bin(WIRE.receive());
  WIRE.receive();
  uint8_t d = bcd2bin(WIRE.receive());
  uint8_t m = bcd2bin(WIRE.receive());
  uint16_t y = bcd2bin(WIRE.receive()) + 2000;
  
  // Daystaamp Calc  
  uint32_t ds;
  ds = hh;
  ds *= 60;
  ds += mm;
  ds *= 60;
  ds += ss;
  
  return DateTime (y, m, d, hh, mm, ss, ds);
}
*/
#endif

/*
////////////////////////////////////////////////////////////////////////////////
// RTC_Millis implementation

long RTC_Millis::offset = 0;

void RTC_Millis::adjust(const DateTime& dt) {
    offset = dt.unixtime() - millis() / 1000;
}

DateTime RTC_Millis::now() {
  return (uint32_t)(offset + millis() / 1000);
}
*/

////////////////////////////////////////////////////////////////////////////////
