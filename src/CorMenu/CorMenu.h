
#ifndef _CorMenu_H_
#define _CorMenu_H_

#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_GFX.h>
#include <GEM_u8g2.h>


class CorMenu
{

public:
    U8G2 u8g2;
    GEM_u8g2 menu;
    CorMenu(U8G2 *oledDisplay);
    void begin(void);
    void show(void);

private:

};

#endif
