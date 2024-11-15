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
#include "CorMenu/CorMenu.h"
#define _SDA 15    /* GPIO15 */
#define _SCL 4     /* GPIO04 */
#define _RED_LED 2 /* GPIO02 */

/*==============================================*/
/* defines buttons and assigns them to the GPIO */
/*==============================================*/

#define _SHORT_PRESS_TIME 1000 // 1000 milliseconds
#define _LONG_PRESS_TIME 1000  // 1000 milliseconds
#define _DEBOUNCE_TIME 50      // set debounce time to 50 milliseconds
#define _NUMBER_THE_OPTIONS 4

uint8_t activeOption = 1;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;

ezButton buttonMenuSpeech(12);    /* GPIO12 */
ezButton buttonMenuPlus(13);      /* GPIO13 */
ezButton buttonMenuMinus(32);     /* GPIO32 */
ezButton buttonMenuConfigure(33); /* GPIO33 */

/*================*/
/* define sensors */
/*================*/
U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* scl=*/18, /* si=*/23, /* cs=*/5, /* rs=*/21, /* rse=*/19);

CorU8G2 _Pantalla(&u8g2);
CorMPU6050 _Inclinometro(&_Pantalla);
CorDS3231 _Clock(&_Pantalla);
CorMeteorology _Metoro(&_Pantalla);
CorDYPLAYER _player;
CorMenu _Menu(&u8g2);
unsigned long _SENSOR_SCAN_DELAY = 300;
unsigned long _LAST_TIME_MILLIS = 0;

/*================*/
/*    setup       */
/*================*/
void setup(void)
{

   // Add menu page to menu and set it as current

   Serial.begin(115200);
   Wire.begin(_SDA, _SCL); //(SDA,SCL)

   /*===========================*/
   /* initiates sensor services */
   /*===========================*/
   _Inclinometro.begin();
   _Clock.begin();
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
}

/*=====================*/
/* says report         */
/*=====================*/
void sayReport()
{
   switch (activeOption)
   {
   case 1:
      _Metoro.readAndShow();
      _player.sayTheTemperature(_Metoro.data.temperature);
      break;
   case 2:
      _Clock.readAndShow();
      _player.sayTheCurrentTime(_Clock.data.hour(), _Clock.data.minute());
      break;
   case 3:
      _Inclinometro.readAndShowCarForm();
      _player.sayTheRollInclination(_Inclinometro.mpuData.ang_x);
      _player.sayThePitchInclination(_Inclinometro.mpuData.ang_y);
      break;
   case 4:
      _Inclinometro.readAndShowFlightForm();
      _player.sayTheRollInclination(_Inclinometro.mpuData.ang_x);
      _player.sayThePitchInclination(_Inclinometro.mpuData.ang_y);
   }
   // Serial.println(activeOption);
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

      if (pressDuration < _SHORT_PRESS_TIME)
      {
         if (&button == &buttonMenuConfigure)
         {
            activeOption = 5;
            _Menu.show();
         }
         else if (&button == &buttonMenuMinus)
         {
            activeOption -= (activeOption <= 1) ? 0 : 1;
         }
         else if (&button == &buttonMenuPlus)
         {
            activeOption += (activeOption >= _NUMBER_THE_OPTIONS) ? 0 : 1;
         }
         else if (&button == &buttonMenuSpeech)
         {
            sayReport();
         }
      }
      if (pressDuration > _LONG_PRESS_TIME)
      {
      }
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
   if ((millis() - _LAST_TIME_MILLIS) > _SENSOR_SCAN_DELAY) /*Sensor reading interval, non blocking delay*/
   {
      switch (activeOption)
      {
      case 1:
         _Metoro.readAndShow();
         break;
      case 2:
         _Clock.readAndShow();
         break;
      case 3:
         _Inclinometro.readAndShowCarForm();
         break;
      case 4:
         _Inclinometro.readAndShowFlightForm();
         break;
      case 5:
         _Menu.show();
         break;
      }

      _LAST_TIME_MILLIS = millis();
   }
}