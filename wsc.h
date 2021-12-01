/*********
 * wsc : Water Storage Control
 * 
 * }
*********/
#ifndef wsc_h
#define wsc_h

#include <Arduino.h>

enum ST_STATUS
{
    FULL,
    EMPTY
};

class WSC
{
public:
    WSC(String);
    void init(int);
    int getStatus();
    void execute();
    void info();

private:
    String _id;
    int _status;
    unsigned long _prevMilli;
};

#endif
