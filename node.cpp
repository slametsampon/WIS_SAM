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

void Node::_setFileParam(String)
{
}

void Node::_setDefaultParam()
{

    paramNode dtParam;
    Serial.println("Node::_setDefaultParam()");

    //set default value
    dtParam.id = 9;
    dtParam.mode = AUTO;
    dtParam.cyclic = ONE_SHOOT;
    dtParam.onDelay = 90;    //minute
    dtParam.onDuration = 35; //minute
    _nodeParam->set(dtParam);
}

void Node::execute(unsigned long samplingTime)
{
    int nodeStatus = IDLE;
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
                if ((millis() - _prevOnDelay) > (_nodeParam->get(PARAM_NODE_ON_DELAY) * 60 * 1000))
                {
                    _nodeStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _nodeStatus.onDelay = _nodeParam->get(PARAM_NODE_ON_DELAY) - _nodeStatus.onDelay;

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_nodeParam->get(PARAM_NODE_ON_DURATION) * 60 * 1000))
                    {
                        _nodeStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _nodeStatus.onDuration = _nodeParam->get(PARAM_NODE_ON_DURATION) - _nodeStatus.onDuration;

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
                if ((millis() - _prevOnDelay) > (_nodeParam->get(PARAM_NODE_ON_DELAY) * 60 * 1000))
                {
                    _nodeStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _nodeStatus.onDelay = _nodeParam->get(PARAM_NODE_ON_DELAY) - _nodeStatus.onDelay;

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_nodeParam->get(PARAM_NODE_ON_DURATION) * 60 * 1000))
                    {
                        _nodeStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _nodeStatus.onDuration = _nodeParam->get(PARAM_NODE_ON_DURATION) - _nodeStatus.onDuration;

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
                if ((millis() - _prevOnDelay) > (_nodeParam->get(PARAM_NODE_ON_DELAY) * 60 * 1000))
                {
                    _nodeStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _nodeStatus.onDelay = _nodeParam->get(PARAM_NODE_ON_DELAY) - _nodeStatus.onDelay;

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_nodeParam->get(PARAM_NODE_ON_DURATION) * 60 * 1000))
                    {
                        _nodeStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _nodeStatus.onDuration = _nodeParam->get(PARAM_NODE_ON_DURATION) - _nodeStatus.onDuration;

                        nodeStatus = IDLE;
                        _firstRun = true;
                    }
                }
            }
            break;

        default:
            break;
        }

        _nodeStatus.status = nodeStatus;

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
        "idNode": 15,
        "mode": 1,
        "cyclic": 0,
        "onDelay": 1440,
        "onDuration": 180
        }
    */
    String strConfig;
    StaticJsonDocument<96> doc;

    doc["idNode"] = _nodeParam->get(PARAM_NODE_ID);
    doc["mode"] = _nodeParam->get(PARAM_NODE_MODE);
    doc["cyclic"] = _nodeParam->get(PARAM_NODE_CYCLIC);
    doc["onDelay"] = _nodeParam->get(PARAM_NODE_ON_DELAY);
    doc["onDuration"] = _nodeParam->get(PARAM_NODE_ON_DURATION);

    serializeJson(doc, strConfig);

    return strConfig;
}

String Node::getStatus()
{
    /*
        {
        "mode": 1,
        "status": 0,
        "onDelay": 1440,
        "onDuration": 180
        }
    */
    String strStatus;
    StaticJsonDocument<48> doc;

    doc["mode"] = _nodeStatus.mode;
    doc["status"] = _nodeStatus.status;
    doc["onDelay"] = _nodeStatus.onDelay;
    doc["onDuration"] = _nodeStatus.onDuration;

    serializeJson(doc, strStatus);
    return strStatus;
}

int Node::_operationLogic()
{
    int mode = _nodeParam->get(PARAM_NODE_MODE);
    int cyclic = _nodeParam->get(PARAM_NODE_CYCLIC);
    unsigned long onDelay = _nodeParam->get(PARAM_NODE_ON_DELAY);
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
    _nodeStatus.mode = oprMode;
    return oprMode;
}