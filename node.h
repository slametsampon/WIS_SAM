
/*********
 * Node :
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
#ifndef node_h
#define node_h

#include <Arduino.h>
#include <ArduinoJson.h>

#include "model.h"
#include "utility.h"

const String NODE_FILE_CFG = "node.cfg";

enum OUTPUT_STATUS
{
    IDLE,
    WAIT,
    ACTIVE
};

enum STATUS_OPR
{
    DUMMY_AUTO,
    MANUAL_ONE,
    MANUAL_CYC,
    MANUAL_CON
};

enum MODE_OPR
{
    AUTO,
    MANUAL,
    STOP
};

enum CYCLIC_OPR
{
    ONE_SHOOT,
    CYCLIC
};

class Node
{
public:
    Node(String);
    void attachFileSystem(FileSystem *);
    void attachParam(AccessParamNode *);
    void setTime(struct tm);
    String getParam();  //JSON format
    String getStatus(); //JSON format
    void init(int);
    void execute(unsigned long); // ms
    void info();

private:
    int _operationLogic();
    void _setDefaultParam();
    void _setFileParam(String);

    boolean _firstRun = true;
    int _irrigationValve;
    String _id;
    unsigned long _prevMilli, _samplingTime, _prevOnDelay, _prevOnDuration;

    AccessParamNode *_nodeParam;
    FileSystem *_localStorage;
};

#endif
