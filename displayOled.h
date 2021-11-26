/*
  displayOled.h - Library for DHT wrapper
  By : Sam Sept 04, 2021

  functions : see following function in detail....
*/

#ifndef displayOled_h
#define displayOled_h

#include <Arduino.h>
#include <Wire.h> // Include Wire if you're using I2C
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LittleFS.h>

class DisplayOled
{
public:
    DisplayOled(String);
    void attachOled(Adafruit_SSD1306 *);
    void displayHT(float, float);
    void info();

private:
    String _id;
    Adafruit_SSD1306 *_oled;
};
#endif