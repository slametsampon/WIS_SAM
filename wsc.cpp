#include "wsc.h"
#include "utility.h"

WSC::WSC(String id) : _id(id) {}

void WSC::init(int pinLevel, int pinPump)
{
    _levelSensor = pinLevel;
    _pump = pinPump;

    pinMode(_levelSensor, INPUT_PULLUP);
    pinMode(_pump, OUTPUT);
}

int WSC::getStatus()
{
    if (_status)
        return EMPTY;
    else
        return FULL;
}

void WSC::execute()
{
    _status = this->_getSensorLevel();

    //level is empty
    if (_status)
        digitalWrite(_pump, HIGH); //activate pump
    else
        digitalWrite(_pump, LOW); //stop pump, full already
}

void WSC::info()
{
    Serial.println("WSC::info()");
    displaySerial("_id", _id);
    displaySerial("_levelSensor", _levelSensor);
    displaySerial("_pump", _pump);
}

boolean WSC::_getSensorLevel()
{
    boolean status = false;
    if (SIMULATION)
    {
        //get random
        if (random(2) >= 1)
            status = true;
    }
    else
    {
        //since pin is pullup input, high = not active, low = active
        if (!digitalRead(_levelSensor))
            status = true;
    }

    return status;
}
