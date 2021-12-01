
/*********
 * Node :
 * 
 *********/
#ifndef node_h
#define node_h

#include <Arduino.h>
#include <ArduinoJson.h>

#include "model.h"
#include "utility.h"

const String NODE_FILE_CFG = "node.cfg";

enum MODE
{
    AUTO,
    MANUAL
};

enum STATUS_OPR
{
    IDLE,
    WAIT,
    ACTIVE
};

enum MODE_OPR
{
    AUTO_OPR,
    MANUAL_ONE,
    MANUAL_CYC,
    MANUAL_CON
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
