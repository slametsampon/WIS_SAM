/*********
 * ic : Irrigation Control
 * 
 * Operation Mode :
 * Auto - (No Delay)
 * Manual - cyclic (On Delay, On Duration)
 * Manual - continuous (On DElay = 0, On Duration)
 * Manual - Off/Idle (On Duration = 0)
 * 
 * data model :
 * paramNode = {
 *  "idNode" : 9,
 *  "mode" : 1, (Stop = 0, Manual = 1, Auto = 2)
 *  "cyclic" : 0, (One Shoot = 0, Cyclic = 1)
 *  "onDelay" : xxxx,
 *  "onDuration" : yyyy
 * }
 * status = {
 *  "status" : 0, (Idle =0, 
 *                Wait = 1, 
 *                Active = 2,
 *                Manual-One = 3,
 *                Manual-Cyc = 4,
 *                Manual-Con = 5,
 *  )
 *  "onDelay" : xxxx,
 *  "onDuration" : yyyy
 * }
*********/
#ifndef ic_h
#define ic_h

#include <Arduino.h>
#include "node.h"

const int NODE_MAX = 4;

class IC
{
public:
    IC(String);
    void addNode(Node *);
    void execute(unsigned long); // ms
    void info();

private:
    String _id;
    int _nodeNbr = 0;
    unsigned long _prevMilli;

    Node *_node[NODE_MAX];
};

#endif
