/**
 * @file CorU8G2.h
 * @brief Encapsulation for OLED display handling
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#ifndef _CorU8G2_H_
#define _CorU8G2_H_

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define _SDA 15 /* GPIO15 */
#define _SCL 4  /* GPIO04 */

typedef struct
{
  int8_t activeScreen = 0; //_LEN_SCREEN;  // vista que inicia
  int xPos100;
  int xPos75;
  int xPos50;
  int xPos25;
  int yPos100;
  int yPos75;
  int yPos50;
  int yPos25;
} lcd_t;

class CorU8G2
{

public:
  lcd_t lcdData;
  U8G2 u8g2;
  void welcome(void);
  void displayCenterText(int x, int y, String text);
  CorU8G2(U8G2 *oledDisplay);

private:
};

#endif