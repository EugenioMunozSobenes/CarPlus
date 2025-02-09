/**
 * @file main.cpp
 * @brief 4x4 travel support information panel.
 *       The main microcontroller for the sensor orchestration is the ESP32.
 * @see https://www.espressif.com/
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <ezButton.h>
#include "CorU8G2/CorU8G2.h"
#include "CorMPU6050/CorMPU6050.h"
#include "CorDS3231/CorDS3231.h"
#include "CorMeteorology/CorMeteorology.h"
#include "CorDYPLAYER/CorDYPLAYER.h"
#define _SDA 15    /* D15 */
#define _SCL 4     /* D4 */
#define _RED_LED 2 /* D2 */

/*==============================================*/
/* defines buttons and assigns them to the GPIO */
/*==============================================*/

#define _SHORT_PRESS_TIME 1000 // 1000 milliseconds
#define _LONG_PRESS_TIME 1000  // 1000 milliseconds
#define _DEBOUNCE_TIME 50      // set debounce time to 50 milliseconds
#define _NUMBER_THE_OPTIONS 4

uint8_t activeFunction = 1;
bool activeConfig = false;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;

ezButton buttonMenuMinus(13);     /* D13 1000*/
ezButton buttonMenuPlus(32);      /* D32 0001*/
ezButton buttonMenuSpeech(33);    /* D33 0010*/
ezButton buttonMenuConfigure(12); /* D12 0100*/

/*================*/
/* define sensors */
/*================*/

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* scl=D18*/ 18, /* si=D23*/ 23, /* cs=D5*/ 5, /* rs=D21*/ 21, /* rse=D19*/ 19);

CorU8G2 _Pantalla(&u8g2);
CorMPU6050 _Inclinometer(&_Pantalla);
CorDS3231 _Clock(&_Pantalla);
CorMeteorology _Metoro(&_Pantalla);
CorDYPLAYER _player;

unsigned long _SENSOR_SCAN_DELAY = 200;
unsigned long _LAST_TIME_MILLIS = 0;
unsigned long _LAST_TIME_MILLIS_ALERT = 0;
TwoWire WireI2C_B = TwoWire(1);
/*================*/
/*    setup       */
/*================*/
void setup(void)
{
   // Add menu page to menu and set it as current

   Serial.begin(115200);
   Wire.begin(_SDA, _SCL);          //(SDA,SCL)
   WireI2C_B.begin(26, 27, 400000); //(SDA=D26,SCL=D27)

   /*===========================*/
   /* initiates sensor services */
   /*===========================*/
   _Inclinometer.begin();
   _Clock.begin(&WireI2C_B);
   _Metoro.begin();
   _player.begin();

   pinMode(_RED_LED, OUTPUT);
   digitalWrite(_RED_LED, 0);

   /*=========================*/
   /* configure buttons       */
   /*=========================*/

   buttonMenuMinus.setDebounceTime(_DEBOUNCE_TIME);
   buttonMenuPlus.setDebounceTime(_DEBOUNCE_TIME);
   buttonMenuSpeech.setDebounceTime(_DEBOUNCE_TIME);
   buttonMenuConfigure.setDebounceTime(_DEBOUNCE_TIME);

   _player.sayWelcome();
}

/*=====================*/
/* says report         */
/*=====================*/
void sayReport()
{
   switch (activeFunction)
   {
   case 1:
      _Clock.readAndShow();
      _player.sayTheCurrentTime(_Clock.data.hour(), _Clock.data.minute());
      break;
   case 2:
      _Metoro.readAndShow();
      _player.sayTheTemperature(_Metoro.data.temperature);
      break;
   case 3:
      _Inclinometer.readAndShowCarForm();
      _player.sayTheRollInclination(_Inclinometer.mpuData.ang_x);
      _player.sayThePitchInclination(_Inclinometer.mpuData.ang_y);
      break;
   case 4:
      _Inclinometer.readAndShowFlightForm();
      _player.sayTheRollInclination(_Inclinometer.mpuData.ang_x);
      _player.sayThePitchInclination(_Inclinometer.mpuData.ang_y);
   }
   // Serial.println(activeFunction);
}
/*===========================*/
/* button event rutine       */
/*===========================*/
void buttonRutine(ezButton &button)
{
   button.loop();
   if (button.isPressed())
      pressedTime = millis();

   if (button.isReleased())
   {
      releasedTime = millis();
      long pressDuration = releasedTime - pressedTime;

      // SET CONFIG STATUS
      if (pressDuration > _LONG_PRESS_TIME && &button == &buttonMenuConfigure)
      {
         if (activeConfig == false)
         {
            _Clock._isConfig = true;
            activeConfig = true;
            digitalWrite(_RED_LED, 1);
         }
         else
         {
            _Clock._isConfig = false;
            activeConfig = false;
            digitalWrite(_RED_LED, 0);
         }
      }

      // BOTON CORTO EN MODO NORMAL
      if (activeConfig == false)
      {
         if (pressDuration < _SHORT_PRESS_TIME)
         {
            if (&button == &buttonMenuMinus && activeFunction > 1)
            {
               activeFunction--;
            }
            else if (&button == &buttonMenuPlus && activeFunction < _NUMBER_THE_OPTIONS)
            {
               activeFunction++;
            }
            else if (&button == &buttonMenuSpeech)
            {
               sayReport();
            }
         }
      }
      // BOTON CORTO EN MODO CONFIGURACION
      else
      {
         // Calibrate inclinometer
         if (activeFunction == 3 || activeFunction == 4)
         {
            if (&button == &buttonMenuSpeech)
            {
               _Inclinometer.selectNextAttribute();
            }
            if (&button == &buttonMenuMinus)
            {
               _Inclinometer.addAlertLimit(-1);
            }
            if (&button == &buttonMenuPlus)
            {
               _Inclinometer.addAlertLimit(1);
            }
         }
         // Ajust clock
         if (activeFunction == 1)
         {
            if (&button == &buttonMenuSpeech)
            {
               _Clock.selectNextAttribute();
            }
            if (&button == &buttonMenuMinus)
            {
               _Clock.add(-1);
            }
            if (&button == &buttonMenuPlus)
            {
               _Clock.add(1);
            }
         }
      }
   }
}
void readSensors()
{
   switch (activeFunction)
   {
   case 1:
      _Clock.readAndShow();
      break;
   case 2:
      _Metoro.readAndShow();
      break;
   case 3:
      _Inclinometer.readAndShowCarForm();
      break;
   case 4:
      _Inclinometer.readAndShowFlightForm();
      break;
   }
}
void checkAlertStatus()
{
   if ((millis() - _LAST_TIME_MILLIS_ALERT) > _SENSOR_SCAN_DELAY * 30) /*Sensor reading interval, non blocking delay*/
   {
      // inclinometer
      if (_Inclinometer.checkAlertPitch())
      {
         _player.sayThePitchInclination(_Inclinometer.mpuData.ang_y);
      }
      if (_Inclinometer.checkAlertRoll())
      {
         _player.sayTheRollInclination(_Inclinometer.mpuData.ang_x);
      }

      // temperature
      _LAST_TIME_MILLIS_ALERT = millis();
   }
}

void loop(void)
{

   /*=========================*/
   /* Explore button event    */
   /*=========================*/
   buttonRutine(buttonMenuMinus);
   buttonRutine(buttonMenuPlus);
   buttonRutine(buttonMenuSpeech);
   buttonRutine(buttonMenuConfigure);

   /*===============================*/
   /* display on screen information */
   /*===============================*/
   long pressDuration = releasedTime;
   if ((millis() - _LAST_TIME_MILLIS) > _SENSOR_SCAN_DELAY) /*Sensor reading interval, non blocking delay*/
   {
      readSensors();
      checkAlertStatus();
      _LAST_TIME_MILLIS = millis();
   }
}