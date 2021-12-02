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
#include "constants.h"

const int USER_NAME = 0;
const int USER_PASSWORD = 1;
const int USER_EMAIL = 2;
const int USER_LEVEL = 3;

const int LEVEL_ENGINEER = 0;
const int LEVEL_OPERATOR = 9;

const int ALFA_EMA = 80;

const int PARAM_HT_VALUE = 0;
const int PARAM_HT_LOW_RANGE = 1;
const int PARAM_HT_HIGH_RANGE = 2;
const int PARAM_HT_LOW_LIMIT = 3;
const int PARAM_HT_HIGH_LIMIT = 4;
const int PARAM_HT_ALFA_EMA = 5;
const int PARAM_HT_ALARM = 6;

const int NO_ALARM = 0;
const int HIGH_ALARM = 1;
const int LOW_ALARM = 2;

const int PARAM_NODE_ID = 0;
const int PARAM_NODE_MODE_OPR = 1;
const int PARAM_NODE_STATUS = 2;
const int PARAM_NODE_MODE = 3;
const int PARAM_NODE_CYCLIC = 4;
const int PARAM_NODE_SET_ON_DELAY = 5;
const int PARAM_NODE_SET_ON_DURATION = 6;
const int PARAM_NODE_ACC_ON_DELAY = 7;
const int PARAM_NODE_ACC_ON_DURATION = 8;

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

class AccessParamHT
{

public:
    AccessParamHT(String);
    String getJson();
    String getStatus();
    paramHT get();
    float get(int);
    void set(paramHT);
    void set(int, float);
    void info();

private:
    String _id;
    paramHT _paramHT;

}; //end of class

typedef struct oDeDu
{
    unsigned long onDelay;
    unsigned long onDuration;
} oDeDu;

typedef struct paramNode
{
    int id;      // 0 - 99
    int modeOpr; //(Auto, Manual-one, Manual-cyc, Manual-con)
    int status;  //(Idle, Wait, Active)
    int mode;    //(Manual = 0, Auto = 1, Repeater = 2)
    int cyclic;  // (One Shoot = 0, Cyclic = 1)
    oDeDu setting;
    oDeDu acc;
} paramNode;

typedef struct nodeStatus
{
    int mode;   //(Auto, Manual-one, Manual-cyc, Manual-con)
    int status; //(Idle, Wait, Active)
    unsigned long onDelay;
    unsigned long onDuration;
} nodeStatus;

class AccessParamNode
{

public:
    AccessParamNode(String);
    String getJson();
    String getStatus();
    paramNode get();
    int get(int);
    void set(paramNode);
    void set(int, int);
    void info();

private:
    String _id;
    paramNode _paramNode;

}; //end of class

#endif
