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
    _attrSelected = _HOUR;
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
    // template:
    oled.u8g2.clearBuffer();
    oled.u8g2.setFontMode(1);                   // Transparent
    oled.u8g2.setFont(u8g2_font_logisoso24_tf); // alternative font:  oled.u8g2_font_10x20_mf
    oled.displayCenterText(oled.lcdData.xPos100/3-1, oled.lcdData.yPos50, ":");
    oled.displayCenterText(oled.lcdData.xPos100*2/3+1, oled.lcdData.yPos50, ":");
    // hour,minutes,soconds:
    oled.displayCenterText(oled.lcdData.xPos25-8, oled.lcdData.yPos50, String(data.hour()));
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos50, String(data.minute()));
    oled.displayCenterText(oled.lcdData.xPos75+8, oled.lcdData.yPos50, String(data.second()));

    // date:
    oled.u8g2.setFont(u8g2_font_6x13_mn);
    oled.displayCenterText(oled.lcdData.xPos50, oled.lcdData.yPos75, String(data.day()) + " / " + String(data.month()) + " / " + String(data.year()));
    if(_isConfig){
        showAttributeConfig();
    }
    oled.u8g2.sendBuffer();
}
void CorDS3231::add(uint16_t inc)
{
    DateTime dat;
    uint16_t val;
    switch (_attrSelected)
    {
    case _HOUR:
        val =data.hour() + inc;
        if(inc>0 && val==24){val=0;}
        if(inc<0 && val==-1){val=24;}
        dat = DateTime(data.year(), data.month(), data.day(), val, data.minute(), data.second());
        break;
    case _MINUTE:
        val = data.minute() + inc;
        if(inc>0 && val==60){val=0;}
        if(inc<0 && val==-1){val=59;}
        dat = DateTime(data.year(), data.month(), data.day(), data.hour(), val, data.second());
        break;
    case _DAY :
        val=data.day() + inc;
        if(inc>0 && val==32){val=1;}
        if(inc<0 && val==0){val=0;}
        dat = DateTime(data.year(), data.month(), val, data.hour(), data.minute(), data.second());
        break;
    case _MONTH :
        val= data.month() + inc;
        if(inc>0 && val==13){val=1;}
        if(inc<0 && val==0){val=12;}        
        dat = DateTime(data.year(),val, data.day(), data.hour(), data.minute(), data.second());
        break;
    case _YEAR :
        val=data.year() + inc;
        dat = DateTime(val, data.month(), data.day(), data.hour(), data.minute(), data.second());
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
void CorDS3231::showAttributeConfig()
{
    int x, y, w, h;
    switch (_attrSelected)
    {
    case _HOUR:
       x = 1;
        y = 2;
        w = oled.lcdData.xPos100*1/3-4;
        h = oled.lcdData.yPos50 + 2;
        break;
    case _MINUTE:
        x = oled.lcdData.xPos100*1/3+2;
        y = 2;
        w = oled.lcdData.xPos100*1/3-3;
        h = oled.lcdData.yPos50 + 2;
        break;
    case _DAY:
        x = oled.lcdData.xPos25 - 12;
        y = oled.lcdData.yPos50 + 2;
        w = 10*2;
        h = 18;
        break;
    case _MONTH:
        x = oled.lcdData.xPos50 - 14;
        y = oled.lcdData.yPos50 +2;
        w = 10*2-4;
        h = 18;
        break;
    case _YEAR:
        x = oled.lcdData.xPos50 + 12;
        y = oled.lcdData.yPos50 + 2;
        w = 10*4-4;
        h = 18;
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
void CorDS3231::selectNextAttribute()
{
    switch (_attrSelected)
    {
    case _HOUR:
       _attrSelected=_MINUTE;
        break;
     case _MINUTE:
        _attrSelected=_DAY;
        break;
     case _DAY:
        _attrSelected=_MONTH;
        break;
    case _MONTH:
        _attrSelected=_YEAR;
        break;
    case _YEAR:
        _attrSelected=_HOUR;
        break;
    }
}