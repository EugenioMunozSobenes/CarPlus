/**
 * @file CorMPU6050.h
 * @brief inclination control
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#ifndef _CorMPU6050_H_
#define _CorMPU6050_H_

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "CorU8G2/CorU8G2.h"

#define _MPU6050_I2C_ADDRESS 0x68 /* I2C 69 */
#define _RADIO_ROLL 30

typedef struct
{
  unsigned long lastTime = 0;
  long tiempo_prev;
  float accel_ang_x;
  float accel_ang_y;
  float accel_ang_z;
  float dt;
  float ang_x, ang_y;
  float ang_x_prev, ang_y_prev;
  float offset_x=0,offset_y=0;
} mpu6050_t;

// ================
//   MPU6050
// ================
class CorMPU6050
{
public:
  CorU8G2 oled;
  CorMPU6050(CorU8G2 *pantalla);
  mpu6050_t mpuData;
  void begin();
  void showFlightForm(void);
  void showCarForm(void);
  void welcome();
  void readAndShowCarForm(void);
  void readAndShowFlightForm(void);
  void setAlerts(int alertRoll, int alertPitch);
  void setZero();
  bool checkAlertPitch();
  bool checkAlertRoll();
private:
  Adafruit_MPU6050 mpu;
  int alertPitch = 35; // Default value
  int alertRoll = 30;  // Default value
  void read(void);
 
};

#endif