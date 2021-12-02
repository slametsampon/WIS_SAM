/*
  constants.h - Library for DHT wrapper
  By : Sam Dec 02, 2021

*/

#ifndef constants_h
#define constants_h

const boolean SIMULATION = true;
const boolean DEBUG = true;

const int DEFAULT_SAMPLING_TIME = 30000; //milli second
const int BLINK_NORMAL = 1000;           //milli second
const int BLINK_WARNING = 250;           //milli second
const int BLINK_ERROR = 125;             //milli second

enum ST_STATUS
{
    FULL,
    EMPTY
};

#endif