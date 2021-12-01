/*********
 * wsc : Water Storage Control
 * 
 * }
*********/
#ifndef wsc_h
#define wsc_h

#include <Arduino.h>

class WSC
{
public:
    WSC(String);
    void execute(unsigned long); // ms
    void info();

private:
    String _id;
    unsigned long _prevMilli;
};

#endif
