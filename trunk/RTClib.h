// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#ifndef _RTCLIB_H_
#define _RTCLIB_H_

// RTC based on the DS1307 chip connected via I2C and the Wire library
class RTC_DS1307 {
public:
    static int begin(void);
//    static void adjust(const DateTime& dt);
    static void adjust(int year, int month, int day, int hour, int min, int sec);
//    uint8_t isrunning(void);
    void now();
    
    int daystamp;
};
#endif // _RTCLIB_H_
