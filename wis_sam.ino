
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
#include "RTClib.h"

#include "wis_sam.h"
#include "sensorHT.h"
#include "utility.h"
#include "node.h"
#include "logsheet.h"

String loginSts = "FIRST_TIME";
unsigned long samplingTime = 0;
unsigned long prevMilli = 0;

//software RTC - simulation
RTC_Millis rtc;

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
void setupRtc();
boolean setupLittleFS();
void setupSensorHT();
void setupLogsheet();
void setupNode();
void startWIFI_AP();
void startWiFiClient();
void startWiFiMulti();

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }

    //setup LittleFS - local storage
    if (!setupLittleFS())
        return;

    localStorage.listFilesInDir("/");

    //setup RTC
    setupRtc();

    /*KINDLY CONCERN OF ORDER SETUP BELLOW*/
    samplingTime = DEFAULT_SAMPLING_TIME;
    setupSensorHT();
    setupLogsheet();
    /*--------------*/
    /*
    setupNode();

    // Start WiFi
    if (AP_MODE)
        startWIFI_AP();
    //else startWiFiClient();
    else
        startWiFiMulti();

    // Start server
    server.begin();
*/
}

// the loop function runs over and over again forever
void loop()
{
    //sensorHT
    if (sensorHT.execute(samplingTime))
        logsheet.setTime(getTime());

    //logsheet
    logsheet.execute(samplingTime);

    //node
    //control storage tank
}

//functions
boolean setupLittleFS()
{
    displaySerial("setup()", "LittleFS.begin()");
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return false;
    }
    return true;
}

void setupSensorHT()
{
    displaySerial("setup()", "setupSensorHT()");
    dhtSensor.begin();
    sensorHT.attachHumidParam(&humidParam);
    sensorHT.attachTempParam(&tempParam);
    sensorHT.attachSensor(&dhtSensor);
    sensorHT.attachFileSystem(&localStorage);
    sensorHT.setParam();
    sensorHT.info();
}

void setupLogsheet()
{
    displaySerial("setup()", "setupLogsheet()");
    logsheet.attachFileSystem(&localStorage);
    logsheet.attachSensor(&sensorHT);
    logsheet.info();
}

void setupNode()
{
    displaySerial("setup()", "setupNode()");
    node.attachFileSystem(&localStorage);
    node.attachParam(&nodeParam);
    node.init(D4);
    node.info();
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

struct tm getTime()
{
    struct tm tmstruct;
    if (NTP_AVAILABLE)
    {
        Serial.println("Contacting Time Server");
        configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
        delay(2000);
        tmstruct.tm_year = 0;
        getLocalTime(&tmstruct, 5000);

        //set offset to 1900, month +1
        tmstruct.tm_year += 1900;
        tmstruct.tm_mon += 1;
    }
    else
    {
        Serial.println("get time from RTC");
        DateTime now = rtc.now();
        tmstruct.tm_year = now.year();
        tmstruct.tm_mon = now.month();
        tmstruct.tm_mday = now.day();
        tmstruct.tm_hour = now.hour();
        tmstruct.tm_min = now.minute();
        tmstruct.tm_sec = now.second();
    }

    return tmstruct;
}

//NTP
bool getLocalTime(struct tm *info, uint32_t ms)
{
    uint32_t count = ms / 10;
    time_t now;

    time(&now);
    localtime_r(&now, info);

    if (info->tm_year > (2016 - 1900))
    {
        return true;
    }

    while (count--)
    {
        delay(10);
        time(&now);
        localtime_r(&now, info);
        if (info->tm_year > (2016 - 1900))
        {
            return true;
        }
    }
    return false;
}

//setup RTC
void setupRtc()
{
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}
