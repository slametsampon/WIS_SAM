#include "wsc.h"

WSC::WSC(String id) : _id(id) {}

void WSC::info()
{
    Serial.println("WSC::info()");
    Serial.print("_id : ");
    Serial.println(_id);
}

void WSC::execute()
{
}
