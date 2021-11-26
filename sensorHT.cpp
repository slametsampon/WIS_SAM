#include "sensorHT.h"

SensorHT::SensorHT(String id) : _id(id) {}

void SensorHT::attachSensor(DHT *dht)
{
    _dht = dht;
}

void SensorHT::setTempParam(paramHT tempParam)
{
    _tempParam = tempParam;
}

void SensorHT::setHumidParam(paramHT humidParam)
{
    _humidParam = humidParam;
}
