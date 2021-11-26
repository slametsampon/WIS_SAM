#include "displayOled.h"

DisplayOled::DisplayOled(String id) : _id(id) {}

void DisplayOled::attachOled(Adafruit_SSD1306 *oled)
{
    _oled = oled;
}

void DisplayOled::displayHT(float t, float h)
{

    int tInt = t * 100;
    t = tInt / 100.0; //get 2 digits

    int hInt = h * 100;
    h = hInt / 100.0; //get 2 digits

    _oled->clearDisplay();
    _oled->setTextSize(1);

    _oled->setTextColor(WHITE);
    _oled->setCursor(0, 0);
    _oled->print("Humidity ");
    _oled->setCursor(1, 11);
    _oled->print(h);
    _oled->print(" %");

    _oled->setCursor(1, 29);
    _oled->print("Temperatur");
    _oled->setCursor(1, 40);
    _oled->print(t);
    _oled->print(" *C ");
    _oled->display();
}
