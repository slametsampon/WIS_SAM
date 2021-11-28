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
#include "utility.h"

const String SENSOR_FILE_CFG = "sensors.cfg";

class SensorHT
{
public:
  SensorHT(String);
  void attachSensor(DHT *);
  void attachFileSystem(FileSystem *);
  void attachTempParam(AccessParamHT *);
  void attachHumidParam(AccessParamHT *);
  void setParam();
  String getParam();
  void setTime(struct tm);
  boolean execute(unsigned long); //sampling periode ms
  String getValues();             //return string in format Json for Temp, Humidity
  logsheetData getValuesHT();     //return H,T
  void info();

private:
  String _id;
  float _prevT, _prevH; //temperature and humidity
  unsigned long _prevMilli;

  FileSystem *_localStorage;
  DHT *_dht;
  AccessParamHT *_tempParam, *_humidParam;
  void _setDefaultParam();
  void _setFileParam(String);
  void _getSensorValue();
  void _displaySerialHT();
};
#endif