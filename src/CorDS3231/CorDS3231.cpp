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
    statusConfig = _HOUR;
}
bool CorDS3231::begin(TwoWire *wireInstance)
{
    return rtc.begin(wireInstance);
} 
void CorDS3231::welcome(void)
{
    oled.u8g2.clearBuffer();
    // oled.u8g2.setFont(u8g2_font_6x10_tf);
    oled.u8g2.setFont(u8g2_font_6x10_tf);
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos50, "Welcome CorDS3231");
    oled.u8g2.sendBuffer();
}
void CorDS3231::readAndShow(void)
{
    data = rtc.now();
    if (data.secondstime() - _lastNow.secondstime() > 0)
    {
        showClock();
        _lastNow = data;
    }
}
void CorDS3231::showClock(void)
{
    //template:
    oled.u8g2.clearBuffer();
    oled.u8g2.setFontMode(1);                   // Transparent
    oled.u8g2.setFont(u8g2_font_logisoso24_tf); // alternative font:  oled.u8g2_font_10x20_mf
    oled.displayCenterText(oled.lcdData.xPos100/3-1, oled.lcdData.yPos50, ":");
    oled.displayCenterText(oled.lcdData.xPos100*2/3+1, oled.lcdData.yPos50, ":");
    //hour,minutes,soconds:
    oled.displayCenterText(oled.lcdData.xPos25-2, oled.lcdData.yPos50, String(data.hour()));
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos50, String(data.minute()));
    oled.displayCenterText(oled.lcdData.xPos75+8, oled.lcdData.yPos50, String(data.second()));

    //date:
    oled.u8g2.setFont(u8g2_font_6x13_mn);
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos75, String(data.day()) + " / " + String(data.month()) + " / " + String(data.year()));
    showAttributeConfig(true);
    
    //showAttributeConfig(true);
    // statusConfig=_DAY; showAttributeConfig(true);
    // statusConfig=_MONTH; showAttributeConfig(true);
    // statusConfig=_YEAR; showAttributeConfig(true);
    oled.u8g2.sendBuffer();
}
void CorDS3231::add(dateType attribute, uint16_t inc)
{
    DateTime dat;
    switch (attribute)
    {
    case _HOUR:
        dat = DateTime(data.year(), data.month(), data.day(), data.hour() + inc, data.minute(), data.second());
        break;
    case _MINUTES /* Minute */:
        dat = DateTime(data.year(), data.month(), data.day(), data.hour() + inc, data.minute(), data.second());
        break;
    case _DAY /* day */:
        dat = DateTime(data.year(), data.month(), data.day() + inc, data.hour(), data.minute(), data.second());
        break;
    case _MONTH /* month */:
        dat = DateTime(data.year(), data.month() + inc, data.day(), data.hour(), data.minute(), data.second());
        break;
    case _YEAR /* year */:
        dat = DateTime(data.year() + inc, data.month(), data.day(), data.hour(), data.minute(), data.second());
        break;
    default:
        dat = data;
        break;
    }
    rtc.adjust(dat);
}

void CorDS3231::adjust(int year, int month, int day, int hour, int minute)
{
    rtc.adjust(DateTime(year, month, day, hour, minute, 0));
}
void CorDS3231::showAttributeConfig(bool view)
{
    int x, y, w, h;
    switch (statusConfig)
    {
    case _HOUR:
       x = 1;
        y = 2;
        w = oled.lcdData.xPos100*1/3-4;
        h = oled.lcdData.yPos50 + 2;
        break;
    case _MINUTES:
        x = oled.lcdData.xPos100*1/3+2;
        y = 2;
        w = oled.lcdData.xPos100*1/3-3;
        h = oled.lcdData.yPos50 + 2;
        break;
    case _DAY:
        x = oled.lcdData.xPos25 - 35;
        y = oled.lcdData.yPos75 + 4;
        w = 40;
        h = 2;
        break;
    case _MONTH:
        /* code */
        break;
    case _YEAR:
        x = oled.lcdData.xPos75 - 40;
        y = oled.lcdData.yPos75 + 4;
        break;
    default:
        x = 0;
        y = 0;
        w = 2;
        h = 2;
        break;
    }

    oled.u8g2.drawFrame(x, y, w, h);
}