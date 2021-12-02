/*********
 * wsc : Water Storage Control
 * 
 * }
*********/
#ifndef wsc_h
#define wsc_h

#include <Arduino.h>
#include "constants.h"

class WSC
{
public:
    WSC(String);
    void init(int, int);
    int getStatus();
    void execute();
    void info();

private:
    String _id;
    int _status, _levelSensor, _pump;
    unsigned long _prevMilli;

    boolean _getSensorLevel();
};

#endif
