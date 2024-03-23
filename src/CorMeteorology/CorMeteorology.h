/**
 * @file CorMeteorology.h
 * @brief Displays real time weather information.
 *          in the electronic design implements the BMP280 and AM2320 sensors.
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#ifndef _CorMeteorology_H_
#define _CorMeteorology_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <AM2320.h>
#include "CorU8G2/CorU8G2.h"

#define _BMP_I2C_ADDRESS 0x76 /* I2C 76 */

typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float altitude;
} MeteoData_t;

class CorMeteorology
{
private:
    Adafruit_BMP280 bmp;
    AM2320 th;
    MeteoData_t previousData;
    void dataSetZero(void);

public:
    CorMeteorology(CorU8G2 *pantalla);
    CorU8G2 oled;
    MeteoData_t data;
    void begin(void);
    void readAndShow(void);
    void read(void);
    void show(void);
};

#endif