/**
 * @file CorMPU6050.cpp
 * @brief inclination control
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#include "CorMPU6050.h"
#include "CorU8G2/CorU8G2.h"

CorMPU6050::CorMPU6050(CorU8G2 *pantalla) : oled(*pantalla)
{
}
void CorMPU6050::begin()
{

  mpu.begin(_MPU6050_I2C_ADDRESS, &Wire, 0); // (i2c_address, &Wire, 0)
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  read();
}
void CorMPU6050::welcome()
{
  oled.u8g2.clearBuffer();
  oled.u8g2.setFont(u8g2_font_6x10_tf);
  oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos50, "Welcome CorMPU6050");
  oled.u8g2.sendBuffer();
}

void CorMPU6050::read()
{
  sensors_event_t a, g, temp;
  mpuData.dt = (millis() - mpuData.tiempo_prev) / 1000.0;
  mpuData.tiempo_prev = millis();
  mpu.getEvent(&a, &g, &temp);
  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;
  // Calculate angles with accelerometer
  mpuData.accel_ang_x = -atan(ay / sqrt(ax * ax + az * az)) * (180.0 / M_PI);
  mpuData.accel_ang_y = -atan(-ax / sqrt(ay * ay + az * az)) * (180.0 / M_PI);
  // Calculate angle of rotation with gyroscope and complementary filter
  mpuData.ang_x = 0.80 * (mpuData.ang_x_prev + (g.gyro.x / 131) * mpuData.dt) + 0.20 * mpuData.accel_ang_x;
  mpuData.ang_y = 0.80 * (mpuData.ang_y_prev + (g.gyro.y / 131) * mpuData.dt) + 0.20 * mpuData.accel_ang_y;
  mpuData.ang_x_prev = mpuData.ang_x;
  mpuData.ang_y_prev = mpuData.ang_y;
}
void CorMPU6050::showFlightForm(void)
{

  oled.u8g2.clearBuffer();
  oled.u8g2.setFontMode(1); // Transparent
  oled.u8g2.drawCircle(oled.lcdData.xPos100 - _RADIO_ROLL - 4, oled.lcdData.yPos50, _RADIO_ROLL);
  oled.u8g2.drawLine(oled.lcdData.xPos100 - _RADIO_ROLL - 4, oled.lcdData.yPos50 - _RADIO_ROLL, oled.lcdData.xPos100 - _RADIO_ROLL - 4, oled.lcdData.yPos50 + _RADIO_ROLL);
  oled.u8g2.drawLine(oled.lcdData.xPos100 - 2 * _RADIO_ROLL - 4, oled.lcdData.yPos50, oled.lcdData.xPos100 - 4, oled.lcdData.yPos50);

  oled.u8g2.drawLine(0, oled.lcdData.yPos50 - 1, oled.lcdData.xPos50 - 5, oled.lcdData.yPos50 - 1);
  oled.u8g2.drawLine(oled.lcdData.xPos50 - 5, 0, oled.lcdData.xPos50 - 5, oled.lcdData.yPos100);

  oled.u8g2.setFont(u8g2_font_squeezed_r7_tr);
  oled.displayCenterText(oled.lcdData.xPos25, 8, "Pitch");
  oled.displayCenterText(oled.lcdData.xPos25, oled.lcdData.yPos50 + 8, "Roll");

  oled.u8g2.setFont(u8g2_font_6x10_tf);
  oled.u8g2.drawStr(0, oled.lcdData.yPos50 - 2, (String(alertPitch) + "\xB0").c_str());
  oled.u8g2.drawStr(0, oled.lcdData.yPos100, (String(alertRoll) + "\xB0").c_str());

  // DATA
  float longHorizont = _RADIO_ROLL + 2;
  float xx = longHorizont * cos((-mpuData.ang_x) * M_PI / 180.0);
  float xy = longHorizont * sin((-mpuData.ang_x) * M_PI / 180.0);
  float h = _RADIO_ROLL * sin(mpuData.ang_y * M_PI / 180.0);
  int X1 = oled.lcdData.xPos100 - _RADIO_ROLL - 4 - xx;
  int Y1 = oled.lcdData.yPos50 + xy + h;
  int X2 = oled.lcdData.xPos100 - _RADIO_ROLL - 4 + xx;
  int Y2 = oled.lcdData.yPos50 - xy + h;
  oled.u8g2.setDrawColor(2);
  oled.u8g2.drawLine(X1, Y1, X2, Y2);
  oled.u8g2.drawLine(X1, Y1 + 1, X2, Y2 + 1);
  oled.u8g2.setFont(u8g2_font_10x20_me);
  oled.displayCenterText(oled.lcdData.xPos25, oled.lcdData.yPos50 - 6, String(mpuData.ang_y + mpuData.offset_y, 0).c_str());
  oled.displayCenterText(oled.lcdData.xPos25, oled.lcdData.yPos100 - 6, String(-mpuData.ang_x + mpuData.offset_x, 0).c_str());
  oled.u8g2.sendBuffer();
}
void CorMPU6050::showCarForm(void)
{
  oled.u8g2.clearBuffer();
  // Layout
  oled.u8g2.setFontMode(1); // Transparent
  oled.u8g2.setFont(u8g2_font_6x10_tf);
  oled.displayCenterText(oled.lcdData.xPos25, 10, "Roll");
  oled.displayCenterText(oled.lcdData.xPos75, 10, "Pitch");
  oled.u8g2.drawLine(0, 12, oled.lcdData.xPos100, 12);

  oled.u8g2.drawCircle(oled.lcdData.xPos25, oled.lcdData.yPos75, 32, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
  oled.u8g2.setDrawColor(2);
  oled.u8g2.drawFrame(oled.lcdData.xPos100 - 10, 14, 10, oled.lcdData.yPos100 - 15);
  oled.u8g2.drawLine(oled.lcdData.xPos100 - 10, 15 + (oled.lcdData.yPos100 - 15) / 2, oled.lcdData.xPos100, 15 + (oled.lcdData.yPos100 - 15) / 2);

  oled.u8g2.setFont(u8g2_font_6x10_tf);
  oled.u8g2.drawStr(oled.lcdData.xPos75, oled.lcdData.yPos100, (String(alertPitch) + "\xB0").c_str());
  oled.u8g2.drawStr(oled.lcdData.xPos25, oled.lcdData.yPos100, (String(alertRoll) + "\xB0").c_str());

  // Show data
  oled.u8g2.setFont(u8g2_font_logisoso24_tf);
  oled.displayCenterText(oled.lcdData.xPos25, oled.lcdData.yPos75, String(abs(mpuData.ang_x) + mpuData.offset_x, 0)); // Roll
  oled.displayCenterText(oled.lcdData.xPos75, oled.lcdData.yPos75, String(abs(mpuData.ang_y) + mpuData.offset_y, 0)); // Pitch

  float x = 30 * cos(M_PI / 2 + mpuData.ang_x * M_PI / 180.0);
  float y = -30 * sin(M_PI / 2 + mpuData.ang_x * M_PI / 180.0);
  float h = mpuData.ang_y * (oled.lcdData.yPos100 - 15) / 180;

  oled.u8g2.drawTriangle(oled.lcdData.xPos25 - 2, oled.lcdData.yPos75, oled.lcdData.xPos25 + 2, oled.lcdData.yPos75, oled.lcdData.xPos25 + x, oled.lcdData.yPos75 + y);
  oled.u8g2.drawBox(oled.lcdData.xPos100 - 9, 15, 8, (oled.lcdData.yPos100 - 15) / 2 + h);
  oled.u8g2.sendBuffer();
}
void CorMPU6050::readAndShowCarForm(void)
{
  read();
  showCarForm();
}
void CorMPU6050::readAndShowFlightForm(void)
{
  read();
  showFlightForm();
}
void CorMPU6050::setAlerts(int roll, int pitch)
{
  alertRoll = roll;
  alertPitch = pitch;
}
void CorMPU6050::setZero()
{
  mpuData.offset_y = -mpuData.ang_y;
  mpuData.offset_x = mpuData.ang_x;
}
bool CorMPU6050::checkAlertPitch()
{
  return alertPitch < abs(mpuData.ang_y);
}
bool CorMPU6050::checkAlertRoll()
{
  return alertRoll < abs(mpuData.ang_x);
}