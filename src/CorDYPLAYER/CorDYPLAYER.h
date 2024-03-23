/**
 * @file CorMeteorology.h
 * @brief Displays real time weather information.
 *          in the electronic design implements the DYPLAYER module.
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#ifndef _CorDYPLAYER_H_
#define _CorDYPLAYER_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <DYPlayerArduino.h>


#define _HW_SERIAL 2 /* Serial2 */

#define _VOICE_GRADES "71"
#define _VOICE_ROLL_INCLINATOR "72"
#define _VOICE_PITCH_INCLINATOR "73"
#define _VOICE_HOUR "75"
#define _VOICE_MINUTES "76"
#define _VOICE_TEMPERATURE "80"

class CorDYPLAYER
{
private:
  String voicesBuffer[5];
  // HardwareSerial HwSerial;
  DY::Player player;

public:
  CorDYPLAYER(void);
  void begin(void);
  void sayWelcome(void);
  void sayTheRollInclination(float angle);
  void sayThePitchInclination(float angle);
  void sayTheCurrentTime(uint8_t hour, uint8_t minute);
  void sayTheTemperature(float temperature);
  void sayTheHumidity(float humedity);
  void sayThePressure(float pressure);

  void sendToDYPLAYER(int numberOfWords);
};

#endif