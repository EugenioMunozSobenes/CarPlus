/**
 * @file CorDS3231.h
 * @brief Displays real time weather information.
 *          in the electronic design implements the DS3231 CLOCK module.
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */

#include <Arduino.h>
#include "CorDS3231.h"
#include "CorU8G2/CorU8G2.h"
#include "RTClib.h"


CorDS3231::CorDS3231(CorU8G2 *pantalla) : oled(*pantalla) 
{

}
bool CorDS3231::begin(TwoWire *wireInstance )
{
   return  rtc.begin(wireInstance);
}
void CorDS3231::welcome(void)
{
    oled.u8g2.clearBuffer();
    oled.u8g2.setFont(u8g2_font_6x10_tf);
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos50, "Welcome CorDS3231");
    oled.u8g2.sendBuffer();
}
void CorDS3231::readAndShow(void)
{
    data = rtc.now(); 
    long a = data.hour() * 360 + data.minute() * 60 + data.second();
    if (data.secondstime() - _lastNow.secondstime() > 0)
    {
        showClock();
        _lastNow = data;
    }
}
void CorDS3231::showClock(void)
{
    oled.u8g2.clearBuffer();
    oled.u8g2.setFontMode(1);                   // Transparent
    oled.u8g2.setFont(u8g2_font_logisoso24_tf); // alternative font:  oled.u8g2_font_10x20_mf
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos50, String(data.hour()) + ":" + String(data.minute()) + ":" + String(data.second()));
    oled.u8g2.setFont(u8g2_font_6x13_mn);
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos75 - 2, String(data.day()) + " / " + String(data.month()) + " / " + String(data.year()));
    oled.u8g2.sendBuffer();
}