#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
 
void setup () {
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime("Dec 03 2018", "08:52:36"));
    RTC.adjust(DateTime(__DATE__, __TIME__));
    //Jan 18 2021//03:52:36

  }
}
void loop () {
    DateTime now = RTC.now(); 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); 
    delay(1000);
    /*Serial.print(__DATE__);
    Serial.print("//");
    Serial.println(__TIME__);*/
}
