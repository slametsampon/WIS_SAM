#include "ic.h"

IC::IC(String id) : _id(id) {}

void IC::addNode(Node *node)
{
    if (_nodeNbr < NODE_MAX)
    {
        _node[_nodeNbr] = node;
    }
}

void IC::info()
{
    Serial.println("IC::info()");
    Serial.print("_id : ");
    Serial.println(_id);
    for (size_t i = 0; i <= _nodeNbr; i++)
    {
        _node[i]->info();
    }
}

void IC::execute(unsigned long samplingTime)
{
    if ((millis() - _prevMilli) > samplingTime)
    {
        _prevMilli = millis();

        //do process here
    }
}
