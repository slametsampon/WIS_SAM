#include "sensorHT.h"

SensorHT::SensorHT(String id) : _id(id) {}

void SensorHT::attachSensor(DHT *dht)
{
    _dht = dht;
}

void SensorHT::setTempParam(param tempParam)
{
    _tempParam = tempParam;
}

void SensorHT::setHumidParam(param humidParam)
{
    _humidParam = humidParam;
}
