#include "node.h"

Node::Node(String id) : _id(id) {}

void Node::attachFileSystem(FileSystem *lc)
{
    _localStorage = lc;
}

void Node::attachParam(AccessParamNode *nodeParam)
{
    _nodeParam = nodeParam;
}

void Node::init(int pin)
{
    _irrigationValve = pin;
    pinMode(_irrigationValve, OUTPUT);

    if (SIMULATION)
        this->_setDefaultParam();
    else
        this->_setFileParam("String");
}

void Node::_setFileParam(String fileName)
{
    /*
    {
    "id": 12,
    "mode": 2,
    "cyclic": 1,
    "status": 2,
    "modeOpr": 3,
    "onDelaySet": 45,
    "onDurationSet": 15,
    "onDelayAcc": 45,
    "onDurationAcc": 15
    }
    // Stream& input;

    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, input);

    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
    }

    int id = doc["id"]; // 12
    int mode = doc["mode"]; // 2
    int cyclic = doc["cyclic"]; // 1
    int status = doc["status"]; // 2
    int modeOpr = doc["modeOpr"]; // 3
    int onDelaySet = doc["onDelaySet"]; // 45
    int onDurationSet = doc["onDurationSet"]; // 15
    int onDelayAcc = doc["onDelayAcc"]; // 45
    int onDurationAcc = doc["onDurationAcc"]; // 15
    */
    paramNode dtParam;
    Serial.println("Node::_setFileParam(String fileName)");

    String fullFileName = PATH_ROOT + fileName;

    Serial.print("fullFileName : ");
    Serial.println(fullFileName);

    char fileNameChar[31];
    fullFileName.toCharArray(fileNameChar, 31);

    File file = _localStorage->readStream(fileNameChar);

    if (!file)
        return;
    else
    {

        StaticJsonDocument<256> doc;

        DeserializationError error = deserializeJson(doc, file);

        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }

        JsonObject node = doc["node"];
        //parameter Node
        dtParam.id = node["id"];
        dtParam.mode = node["mode"];
        dtParam.cyclic = node["cyclic"];
        dtParam.status = node["status"];
        dtParam.modeOpr = node["modeOpr"];
        dtParam.setting.onDelay = node["onDelaySet"];
        dtParam.setting.onDuration = node["onDurationSet"];
        dtParam.acc.onDelay = node["onDelayAcc"];
        dtParam.acc.onDuration = node["onDurationAcc"];
        _nodeParam->set(dtParam);

        file.close();
    }
}

void Node::_setDefaultParam()
{

    paramNode dtParam;
    Serial.println("Node::_setDefaultParam()");

    //set default value
    dtParam.id = 9;
    dtParam.mode = AUTO;
    dtParam.cyclic = ONE_SHOOT;
    dtParam.setting.onDelay = 90;    //minute
    dtParam.setting.onDuration = 35; //minute
    dtParam.acc.onDelay = 90;        //minute
    dtParam.acc.onDuration = 35;     //minute
    _nodeParam->set(dtParam);
}

void Node::execute(unsigned long samplingTime)
{
    int nodeStatus = IDLE;
    unsigned long valTime; //on milli Second

    if ((millis() - _prevMilli) > samplingTime)
    {
        _prevMilli = millis();

        //do process here
        int oprMode = this->_operationLogic(); //select operation mode

        switch (oprMode)
        {
        case MANUAL_ONE: //Manual One Shoot
            if (_firstRun)
            {
                _firstRun = false;
                _prevOnDelay = millis();
                nodeStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_nodeParam->get(PARAM_NODE_SET_ON_DELAY) * 60 * 1000))
                {
                    valTime = (millis() - _prevOnDelay) / (1000 * 60);
                    valTime = _nodeParam->get(PARAM_NODE_SET_ON_DELAY) - _nodeParam->get(PARAM_NODE_ACC_ON_DELAY);
                    _nodeParam->set(PARAM_NODE_ACC_ON_DELAY, valTime);

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_nodeParam->get(PARAM_NODE_SET_ON_DURATION) * 60 * 1000))
                    {
                        valTime = (millis() - _prevOnDuration) / (1000 * 60);
                        valTime = _nodeParam->get(PARAM_NODE_SET_ON_DURATION) - _nodeParam->get(PARAM_NODE_ACC_ON_DURATION);
                        _nodeParam->set(PARAM_NODE_ACC_ON_DURATION, valTime);

                        nodeStatus = IDLE;
                        _firstRun = true;
                    }
                }
            }
            break;

        case MANUAL_CYC: //Manual Cyclic
            if (_firstRun)
            {
                _firstRun = false;
                _prevOnDelay = millis();
                nodeStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_nodeParam->get(PARAM_NODE_SET_ON_DELAY) * 60 * 1000))
                {
                    valTime = (millis() - _prevOnDelay) / (1000 * 60);
                    valTime = _nodeParam->get(PARAM_NODE_SET_ON_DELAY) - _nodeParam->get(PARAM_NODE_ACC_ON_DELAY);
                    _nodeParam->set(PARAM_NODE_ACC_ON_DELAY, valTime);

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_nodeParam->get(PARAM_NODE_SET_ON_DURATION) * 60 * 1000))
                    {
                        valTime = (millis() - _prevOnDuration) / (1000 * 60);
                        valTime = _nodeParam->get(PARAM_NODE_SET_ON_DURATION) - _nodeParam->get(PARAM_NODE_ACC_ON_DURATION);
                        _nodeParam->set(PARAM_NODE_ACC_ON_DURATION, valTime);

                        _prevOnDelay = millis();
                        nodeStatus = WAIT;
                    }
                }
            }
            break;

        case MANUAL_CON: //Manual Continuous
            nodeStatus = ACTIVE;
            break;

        case AUTO: //Auto
            if (_firstRun)
            {
                _firstRun = false;
                _prevOnDelay = millis();
                nodeStatus = WAIT;
            }
            else
            {
                if ((millis() - _prevOnDelay) > (_nodeParam->get(PARAM_NODE_SET_ON_DELAY) * 60 * 1000))
                {
                    valTime = (millis() - _prevOnDelay) / (1000 * 60);
                    valTime = _nodeParam->get(PARAM_NODE_SET_ON_DELAY) - _nodeParam->get(PARAM_NODE_ACC_ON_DELAY);
                    _nodeParam->set(PARAM_NODE_ACC_ON_DELAY, valTime);

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_nodeParam->get(PARAM_NODE_SET_ON_DURATION) * 60 * 1000))
                    {
                        valTime = (millis() - _prevOnDuration) / (1000 * 60);
                        valTime = _nodeParam->get(PARAM_NODE_SET_ON_DURATION) - _nodeParam->get(PARAM_NODE_ACC_ON_DURATION);
                        _nodeParam->set(PARAM_NODE_ACC_ON_DURATION, valTime);

                        nodeStatus = IDLE;
                        _firstRun = true;
                    }
                }
            }
            break;

        default:
            break;
        }

        _nodeParam->set(PARAM_NODE_STATUS, nodeStatus);

        if (nodeStatus == ACTIVE)
        {
            //send command to activate irrigation valve
            digitalWrite(_irrigationValve, HIGH);
        }
        else
        {
            //send command to de-activate irrigation valve
            digitalWrite(_irrigationValve, LOW);
        }
    }
}

String Node::getParam()
{
    /*
        {
        "id": 15,
        "mode": 1,
        "cyclic": 0,
        "onDelaySet": 1440,
        "onDurationSet": 180
        }
    */
    String strConfig;
    StaticJsonDocument<192> doc;

    doc["id"] = _nodeParam->get(PARAM_NODE_ID);
    doc["mode"] = _nodeParam->get(PARAM_NODE_MODE);
    doc["cyclic"] = _nodeParam->get(PARAM_NODE_CYCLIC);
    doc["onDelaySet"] = _nodeParam->get(PARAM_NODE_SET_ON_DELAY);
    doc["onDurationSet"] = _nodeParam->get(PARAM_NODE_SET_ON_DURATION);

    serializeJson(doc, strConfig);

    return strConfig;
}

String Node::getStatus()
{
    /*
        {
        "id": 15,
        "mode": 1,
        "status": 0,
        "onDelayAcc": 1440,
        "onDurationAcc": 180
        }
    */
    String strStatus;
    StaticJsonDocument<192> doc;

    doc["id"] = _nodeParam->get(PARAM_NODE_ID);
    doc["mode"] = _nodeParam->get(PARAM_NODE_MODE);
    doc["status"] = _nodeParam->get(PARAM_NODE_STATUS);
    doc["onDelay"] = _nodeParam->get(PARAM_NODE_ACC_ON_DELAY);
    doc["onDuration"] = _nodeParam->get(PARAM_NODE_ACC_ON_DURATION);

    serializeJson(doc, strStatus);
    return strStatus;
}

void Node::info()
{
    Serial.println("Node::info()");
    Serial.print("_id : ");
    Serial.println(_id);

    _nodeParam->info();
}

int Node::_operationLogic()
{
    int mode = _nodeParam->get(PARAM_NODE_MODE);
    int cyclic = _nodeParam->get(PARAM_NODE_CYCLIC);
    unsigned long onDelay = _nodeParam->get(PARAM_NODE_SET_ON_DELAY);
    int oprMode = IDLE;

    switch (mode)
    {
    case MANUAL:
        if (cyclic == ONE_SHOOT)
        {
            oprMode = MANUAL_ONE;
        }
        else if (cyclic == CYCLIC)
        {
            oprMode = MANUAL_CYC;
        }

        //manual continuous
        if (onDelay <= 0)
        {
            oprMode = MANUAL_CON;
        }

        break;

    case AUTO:
        oprMode = AUTO;
        break;

    default:
        break;
    }
    _nodeParam->set(PARAM_NODE_MODE_OPR, oprMode);
    return oprMode;
}