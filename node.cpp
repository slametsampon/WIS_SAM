#include "node.h"

Node::Node(String id) : _id(id) {}

void Node::init(int pin)
{
    _irrigationValve = pin;
    pinMode(_irrigationValve, OUTPUT);

    //set default value
    _paramNode.id = 9;
    _paramNode.mode = AUTO;
    _paramNode.cyclic = ONE_SHOOT;
    _paramNode.onDelay = 90;    //minute
    _paramNode.onDuration = 35; //minute
}

void Node::setConfig(paramNode paramNodeData)
{
    _paramNode = paramNodeData;
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
                if ((millis() - _prevOnDelay) > (_paramNode.onDelay * 60 * 1000))
                {
                    _nodeStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _nodeStatus.onDelay = _paramNode.onDelay - _nodeStatus.onDelay;

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_paramNode.onDuration * 60 * 1000))
                    {
                        _nodeStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _nodeStatus.onDuration = _paramNode.onDuration - _nodeStatus.onDuration;

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
                if ((millis() - _prevOnDelay) > (_paramNode.onDelay * 60 * 1000))
                {
                    _nodeStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _nodeStatus.onDelay = _paramNode.onDelay - _nodeStatus.onDelay;

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_paramNode.onDuration * 60 * 1000))
                    {
                        _nodeStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _nodeStatus.onDuration = _paramNode.onDuration - _nodeStatus.onDuration;

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
                if ((millis() - _prevOnDelay) > (_paramNode.onDelay * 60 * 1000))
                {
                    _nodeStatus.onDelay = (millis() - _prevOnDelay) / (1000 * 60);
                    _nodeStatus.onDelay = _paramNode.onDelay - _nodeStatus.onDelay;

                    nodeStatus = ACTIVE;
                    _prevOnDuration = millis();
                }
                else
                {
                    if ((millis() - _prevOnDuration) > (_paramNode.onDuration * 60 * 1000))
                    {
                        _nodeStatus.onDuration = (millis() - _prevOnDuration) / (1000 * 60);
                        _nodeStatus.onDuration = _paramNode.onDuration - _nodeStatus.onDuration;

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

String Node::getConfig()
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

    doc["idNode"] = _paramNode.id;
    doc["mode"] = _paramNode.mode;
    doc["cyclic"] = _paramNode.cyclic;
    doc["onDelay"] = _paramNode.onDelay;
    doc["onDuration"] = _paramNode.onDuration;

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
    int mode = _paramNode.mode;
    int cyclic = _paramNode.cyclic;
    unsigned long onDelay = _paramNode.onDelay;
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