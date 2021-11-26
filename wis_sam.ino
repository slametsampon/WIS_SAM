
/*
WIS - SAM - Wireless Irrigation System - Semanding Agrifarm Makmur
by : Slamet
Nov '21
*/

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFiMulti.h>

#include "model.h"
#include "wis_sam.h"
#include "oDeDu.h"
#include "sensorHT.h"
#include "utility.h"

String loginSts = "FIRST_TIME";
unsigned long samplingTime = 0;

AccesUser accessEngineer("accessEngineer");
AccesUser accessOperator("accessOperator");
AccesUser activeUser("activeUser");

DHT dhtSensor(DHTPIN, DHTTYPE);
SensorHT sensorHT("sensor HT");
FileSystem localStorage("local storage");

ESP8266WiFiMulti wifiMulti; // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }

    // Initialize LittleFS
    Serial.println("Begin LittleFS");
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    sensorHT.attachSensor(&dhtSensor);
}

// the loop function runs over and over again forever
void loop()
{
}
