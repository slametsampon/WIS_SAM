
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

#include "constants.h"
#include "wis_sam.h"
#include "sensorHT.h"
#include "utility.h"
#include "logsheet.h"
#include "node.h"
#include "ic.h"
#include "wsc.h"

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

FileSystem localStorage("local storage");
Logsheet logsheet("logsheet");

DHT dhtSensor(DHTPIN, DHTTYPE);
SensorHT sensorHT("sensor HT");
Node node1("node-1");
Node node2("node-2");
Node node3("node-3");
Node node4("node-4");
IC ic("Irrigation Control");

WSC wsc("Water Storage Control");

ESP8266WiFiMulti wifiMulti; // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//functions prototype
void setupRtc();
boolean setupLittleFS();
void setupSensorHT();
void setupLogsheet();
void setupNode();
void setupIC();
void startWIFI_AP();
void startWiFiClient();
void startWiFiMulti();

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
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

    setupNode();
    setupIC();
    setupWSC();

    /*
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

    blinkLED(BLINK_NORMAL);

    //sensorHT
    if (sensorHT.execute(samplingTime))
        logsheet.setTime(getTime());

    //logsheet
    logsheet.execute(samplingTime);

    //control storage tank
    wsc.execute();

    //irrigation control included node execution
    ic.execute(wsc.getStatus(), samplingTime);
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
    node1.attachFileSystem(&localStorage);
    node1.attachParam(&nodeParam);
    node1.init(GROUP1_PIN);
    node1.info();

    node2.attachFileSystem(&localStorage);
    node2.attachParam(&nodeParam);
    node2.init(GROUP2_PIN);
    node2.info();

    node3.attachFileSystem(&localStorage);
    node3.attachParam(&nodeParam);
    node3.init(GROUP3_PIN);
    node3.info();

    node4.attachFileSystem(&localStorage);
    node4.attachParam(&nodeParam);
    node4.init(GROUP4_PIN);
    node4.info();
}

void setupIC()
{
    displaySerial("setup()", "setupIC()");
    ic.addNode(&node1);
    ic.addNode(&node2);
    ic.addNode(&node3);
    ic.addNode(&node4);

    ic.info();
}

void setupWSC()
{
    displaySerial("setup()", "setupWSC()");
    wsc.init(LEVEL_PIN, PUMP_PIN);
    wsc.info();
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

void blinkLED(unsigned long blinkTime)
{
    if ((millis() - prevMilli) >= blinkTime)
    {
        //displaySerial("blinkLED()", (int)blinkTime);
        prevMilli = millis();
        digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);
    }
}
