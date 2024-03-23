/**
 * @file CorMeteorology.cpp
 * @brief Displays real time weather information.
 *          in the electronic design implements the BMP280 and AM2320 sensors.
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#include "CorMeteorology/CorMeteorology.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <AM2320.h>
#include "CorU8G2/CorU8G2.h"
#include "CorMeteorology.h"

CorMeteorology::CorMeteorology(CorU8G2 *pantalla) : oled(*pantalla), th(&Wire)
{
}
void CorMeteorology::begin(void)
{
    bmp.begin(_BMP_I2C_ADDRESS);
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Mode of operation */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering */
                    Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */
}
void CorMeteorology::readAndShow(void)
{
    read();
    if (previousData.altitude != data.altitude || previousData.humidity != data.humidity || previousData.pressure != data.pressure || previousData.temperature != data.temperature)
    {
        show();
        previousData.altitude = data.altitude;
        previousData.humidity = data.humidity;
        previousData.pressure = data.pressure;
        previousData.temperature = data.temperature;
    }
}
void CorMeteorology::read(void)
{
    switch (th.Read())
    {
    case 2:
        dataSetZero();
        Serial.println(F(" CRC failed"));
        break;
    case 1:
        // digitalWrite(2, 1);
        dataSetZero();
        Serial.println(F(" Sensor offline"));
        break;
    case 0:
        // GOOD!!!
        data.altitude = bmp.readAltitude(1021);
        data.humidity = th.Humidity;
        data.pressure = bmp.readPressure();
        data.temperature = th.cTemp;
        break;
    }
}
void CorMeteorology::dataSetZero(void)
{
    data.altitude = data.humidity = data.pressure = data.temperature = 0;
}
void CorMeteorology::show(void)
{
    // Layout
    oled.u8g2.clearBuffer();
    oled.u8g2.setFontMode(1); // Transparent
    oled.u8g2.setFont(u8g2_font_squeezed_r7_tr);
    oled.displayCenterText(oled.lcdData.xPos25, 8, "Temperature  C");
    oled.displayCenterText(oled.lcdData.xPos75, 8, "Humedity  %");
    oled.displayCenterText(oled.lcdData.xPos25, oled.lcdData.yPos50 + 8, "Bar pressure  hPa");
    oled.displayCenterText(oled.lcdData.xPos75, oled.lcdData.yPos50 + 8, "Altitude  m");
    oled.u8g2.drawLine(0, oled.lcdData.yPos50 - 1, oled.lcdData.xPos100, oled.lcdData.yPos50 - 1);
    oled.u8g2.drawLine(oled.lcdData.xPos50, 0, oled.lcdData.xPos50, oled.lcdData.yPos100);

    // Show data
    oled.u8g2.setFont(u8g2_font_9x18B_mf);
    oled.displayCenterText(oled.lcdData.xPos25, oled.lcdData.yPos50 - 6, (String(data.temperature, 1) + "\xB0").c_str());
    oled.displayCenterText(oled.lcdData.xPos75, oled.lcdData.yPos50 - 6, String(data.humidity, 1).c_str());
    oled.displayCenterText(oled.lcdData.xPos25, oled.lcdData.yPos100 - 6, String(data.pressure, 0).c_str());
    oled.displayCenterText(oled.lcdData.xPos75, oled.lcdData.yPos100 - 6, String(data.altitude, 0).c_str()); // default: 1013.25
    oled.u8g2.sendBuffer();
}
