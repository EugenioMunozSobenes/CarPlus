#include "CorMenu/CorMenu.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <GEM_u8g2.h>
int number = -512;
bool enablePrint = false;

GEMItem menuItemInt("Number:", number);
GEMItem menuItemBool("Enable print:", enablePrint);
GEMPage menuPageMain("Main Menu");

CorMenu::CorMenu(U8G2 *oledDisplay) : u8g2(*oledDisplay), menu(*oledDisplay, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO)
{
    
}
void CorMenu::begin(void)
{   menu.init();
    menuPageMain.addMenuItem(menuItemInt);
    menuPageMain.addMenuItem(menuItemBool);
    menu.setMenuPageCurrent(menuPageMain);
 

}
void CorMenu::show(void)
{
    menu.drawMenu();

}