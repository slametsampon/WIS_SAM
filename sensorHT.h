/*
  sensorHT.h - Library for DHT wrapper
  By : Sam Sept 04, 2021

  functions : see following function in detail....
*/

#ifndef sensorHT_h
#define sensorHT_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHT.h>

#include "model.h"

const boolean SIMULATION = true;
const int PARAMETER_VALUE = 0;
const int PARAMETER_LOW_RANGE = 1;
const int PARAMETER_HIGH_RANGE = 2;
const int PARAMETER_LOW_LIMIT = 3;
const int PARAMETER_HIGH_LIMIT = 4;
const int PARAMETER_ALFA_EMA = 5;
const int PARAMETER_ALARM = 6;

//commSer header code
const int DATA_OPERATION = 0;
const int DATA_PARAMETER = 1;
const int REMOTE_PARAMETER = 2;
const int REMOTE_OPERATION = 3;

const int NO_ALARM = 0;
const int HIGH_ALARM = 1;
const int LOW_ALARM = 2;

class SensorHT
{
public:
  SensorHT(String);
  void attachSensor(DHT *);
  String getCfgParameter();
  void setTempParam(paramHT);
  void setHumidParam(paramHT);
  void setTime(struct tm);
  void execute(unsigned long); //sampling periode ms
  String getValues();          //return string in format Json for Temp, Humidity
  void info();

private:
  String _id;
  float _prevT, _prevH; //temperature and humidity
  unsigned long _prevMilli, _samplingTime;
  DHT *_dht;
  paramHT _tempParam, _humidParam;
};
#endif