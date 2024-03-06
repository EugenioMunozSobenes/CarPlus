#ifndef _CorDS3231_H_
#define _CorDS3231_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include "CorU8G2/CorU8G2.h"

class CorDS3231
{
private:
  RTC_DS3231 rtc;
  DateTime _lastNow;
  float _actualTemperature;
  float _previousTemperature;
  float _humedity = 0;

public:
  CorU8G2 oled;
  CorDS3231(CorU8G2 *pantalla);
  DateTime data;
  void begin(void);
  void welcome(void);
  void readAndShow(void);
  void showClock(void);
};

#endif