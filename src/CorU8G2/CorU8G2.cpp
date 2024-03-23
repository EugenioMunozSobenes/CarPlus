/**
 * @file CorU8G2.cpp
 * @brief Encapsulation for OLED display handling
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#include "CorU8G2.h"
#include "U8g2lib.h"

CorU8G2::CorU8G2(U8G2 *oledDisplay) : u8g2(*oledDisplay)
{
    u8g2.begin();
    u8g2.setFontDirection(0);

    lcdData.xPos100 = u8g2.getDisplayWidth();
    lcdData.xPos75 = 3 * u8g2.getDisplayWidth() / 4;
    lcdData.xPos50 = u8g2.getDisplayWidth() / 2;
    lcdData.xPos25 = u8g2.getDisplayWidth() / 4;

    lcdData.yPos100 = u8g2.getDisplayHeight();
    lcdData.yPos75 = 3 * lcdData.yPos100 / 4;
    lcdData.yPos50 = lcdData.yPos100 / 2;
    lcdData.yPos25 = lcdData.yPos100 / 4;
};

void CorU8G2::welcome(void)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    displayCenterText(lcdData.xPos50, lcdData.yPos50, "Welcome U8G2");
    u8g2.sendBuffer();
}
void CorU8G2::displayCenterText(int x, int y, String text)
{
    u8g2.drawStr(x - u8g2.getStrWidth(text.c_str()) / 2, y, text.c_str());
}
