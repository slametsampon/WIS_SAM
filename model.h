/*
  Model.h - Library untuk Model
  Ini adalah bagian dari MVC (Model View Control) pattern design
  Dibuat Oleh : Sam Feb 15, 2021
  JSON

StaticJsonDocument<96> doc;

doc["id"] = "Smoke-1";
doc["unit"] = "%";
doc["value"] = 51.5;
doc["highRange"] = 100;
doc["lowRange"] = 0;
doc["highLimit"] = 80;
doc["lowLimit"] = 40;
doc["alfaEma"] = 75.5;
doc["increment"] = 1.1;

serializeJson(doc, output);

? Stream& input;

StaticJsonDocument<192> doc;

DeserializationError error = deserializeJson(doc, input);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

const char* id = doc["id"]; // "Smoke-1"
const char* unit = doc["unit"]; // "%"
float value = doc["value"]; // 51.5
int highRange = doc["highRange"]; // 100
int lowRange = doc["lowRange"]; // 0
int highLimit = doc["highLimit"]; // 80
int lowLimit = doc["lowLimit"]; // 40
float alfaEma = doc["alfaEma"]; // 75.5
float increment = doc["increment"]; // 1.1
*/
#ifndef model_h
#define model_h

#include <Arduino.h>
#include <ArduinoJson.h>

const int USER_NAME = 0;
const int USER_PASSWORD = 1;
const int USER_EMAIL = 2;
const int USER_LEVEL = 3;

const int LEVEL_ENGINEER = 0;
const int LEVEL_OPERATOR = 9;

typedef struct userData
{
    String username; //engineer
    String password; //123456
    String email;    //exampla@example.com
    int level;
} userData;

class AccesUser
{

public:
    AccesUser(String);
    String getJson();
    userData get();
    String get(int);
    void set(userData);
    void set(int, String);
    void info();

private:
    String _id;
    userData _userData;

}; //end of class

typedef struct logsheetData
{
    String time; //HH:00:00 or DD_MM_YY
    float temperature;
    float humidity;
} logsheetData;

typedef struct paramHT
{
    String unit; //unit
    float value;
    float highRange;
    float lowRange;
    float highLimit; //for alarm high
    float lowLimit;  //for alarm low
    float alfaEma;   //alfa for EMA Filter (0 - 100) in percentage
    int alarm;
} paramHT;

typedef struct paramNode
{
    int prev;   // 0 - 99
    int id;     // 0 - 99
    int next;   // 0 - 99
    int mode;   //(Manual = 0, Auto = 1, Repeater = 2)
    int cyclic; // (One Shoot = 0, Cyclic = 1)
    unsigned long onDelay;
    unsigned long onDuration;
} paramNode;

typedef struct nodeStatus
{
    int mode;   //(Auto, Manual-one, Manual-cyc, Manual-con)
    int status; //(Idle, Wait, Active)
    unsigned long onDelay;
    unsigned long onDuration;
} nodeStatus;

#endif
