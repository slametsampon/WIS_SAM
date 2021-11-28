
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

#include "wis_sam.h"
#include "sensorHT.h"
#include "utility.h"
#include "node.h"
#include "logsheet.h"

String loginSts = "FIRST_TIME";
unsigned long samplingTime = 0;

AccesUser accessEngineer("accessEngineer");
AccesUser accessOperator("accessOperator");
AccesUser activeUser("activeUser");

AccessParamHT tempParam("Temperature Parameter");
AccessParamHT humidParam("Humidity Parameter");
AccessParamNode nodeParam("Node Parameter");

DHT dhtSensor(DHTPIN, DHTTYPE);
SensorHT sensorHT("sensor HT");
FileSystem localStorage("local storage");
Node node("node");
Logsheet logsheet("logsheet");

ESP8266WiFiMulti wifiMulti; // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//functions prototype
void setupLittleFS();
void setupSensorHT();
void setupLogsheet();
void startWIFI_AP();
void startWiFiClient();
void startWiFiMulti();

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }

    /*KINDLY CONCERN OF ORDER SETUP BELLOW*/
    setupLittleFS();
    setupSensorHT();
    setupLogsheet();
    /*--------------*/

    // Start WiFi
    if (AP_MODE)
        startWIFI_AP();
    //else startWiFiClient();
    else
        startWiFiMulti();

    // Start server
    server.begin();
}

// the loop function runs over and over again forever
void loop()
{
}

//functions
void setupLittleFS()
{
    displaySerial("setup()", "LittleFS.begin()");
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    localStorage.listFilesInDir("/");
}

void setupSensorHT()
{
    displaySerial("setup()", "setupSensorHT()");
    dhtSensor.begin();
    sensorHT.setParam();
    sensorHT.attachSensor(&dhtSensor);
    sensorHT.attachFileSystem(&localStorage);
    sensorHT.attachHumidParam(&humidParam);
    sensorHT.attachTempParam(&tempParam);
    sensorHT.info();
}

void setupLogsheet()
{
    displaySerial("setup()", "setupLogsheet()");
    logsheet.attachFileSystem(&localStorage);
    logsheet.attachSensor(&sensorHT);
    logsheet.info();
}

void startWiFiMulti()
{                                      // Try to connect to some given access points. Then wait for a connection
    wifiMulti.addAP(SSID1, PASSWORD1); // add Wi-Fi networks you want to connect to
    wifiMulti.addAP(SSID2, PASSWORD2);
    wifiMulti.addAP(SSID3, PASSWORD3);
    wifiMulti.addAP(SSID4, PASSWORD4);

    Serial.println("Connecting");
    while (wifiMulti.run() != WL_CONNECTED)
    { // Wait for the Wi-Fi to connect
        delay(250);
        Serial.print('.');
    }
    Serial.println("\r\n");
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID()); // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.print(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
    Serial.println("\r\n");
}

void startWiFiClient()
{
    Serial.println("Connecting to " + (String)SSID1);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID1, PASSWORD1);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWIFI_AP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID1, PASSWORD1);
    Serial.println("AP started");
    Serial.println("IP address: " + WiFi.softAPIP().toString());
}
