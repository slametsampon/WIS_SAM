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

class SensorHT
{
public:
    SensorHT(String);
    void attachSensor(DHT *);
    String getCfgParameter();
    void setTempParam(param);
    void setHumidParam(param);
    void setTime(struct tm);
    void execute(unsigned long); //sampling periode ms
    String getValues();          //return string in format Json for Temp, Humidity
    void info();

private:
    String _id;
    float _prevT, _prevH; //temperature and humidity
    unsigned long _prevMilli, _samplingTime;
    DHT *_dht;
    param _tempParam, _humidParam;
};
#endif