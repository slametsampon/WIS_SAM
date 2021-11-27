#include "model.h"

//AccesUser - Class
AccesUser::AccesUser(String id) : _id(id) {}

String AccesUser::getJson()
{
    /*

    {
      "username":"engineer",
      "password":"123456",
      "email":"engineer@example.com",
      "level":0
    }

  StaticJsonDocument<128> doc;

  doc["username"] = "engineer";
  doc["password"] = "123456";
  doc["email"] = "engineer@example.com";
  doc["level"] = 0;

  serializeJson(doc, output);

  */

    // Get a reference to the root object
    StaticJsonDocument<128> doc;
    String output;

    doc["username"] = _userData.username;
    doc["password"] = _userData.password;
    doc["email"] = _userData.email;
    doc["level"] = _userData.level;

    serializeJson(doc, output);
    return output;
}

void AccesUser::set(userData ud)
{

    _userData = ud;
}

void AccesUser::set(int idUser, String val)
{

    switch (idUser)
    {
    case USER_NAME:
        _userData.username = val;
        break;

    case USER_PASSWORD:
        _userData.password = val;
        break;

    case USER_EMAIL:
        _userData.email = val;
        break;

    case USER_LEVEL:
        _userData.level = val.toInt();
        break;

    default:
        break;
    }
}

userData AccesUser::get()
{

    return _userData;
}

String AccesUser::get(int idUser)
{

    switch (idUser)
    {
    case USER_NAME:
        return _userData.username;
        break;

    case USER_PASSWORD:
        return _userData.password;
        break;

    case USER_EMAIL:
        return _userData.email;
        break;

    case USER_LEVEL:
        return String(_userData.level);
        break;

    default:
        break;
    }
}

void AccesUser::info()
{
    Serial.println("AccesUser::info()");

    Serial.println(this->getJson());
}

//AccessParamHT - Class
AccessParamHT::AccessParamHT(String id) : _id(id) {}

String AccessParamHT::getJson()
{
    /*
  {
    "unit":"%",
    "value":51.5,
    "highRange":100.0,
    "lowRange":0.0,
    "highLimit":80.0,
    "lowLimit":40.0,
    "alfaEma":80.0,
    "alarm":2
  }
  StaticJsonDocument<192> doc;

  doc["unit"] = "%";
  doc["value"] = 51.5;
  doc["highRange"] = 100;
  doc["lowRange"] = 0;
  doc["highLimit"] = 80;
  doc["lowLimit"] = 40;
  doc["alfaEma"] = 80;
  doc["alarm"] = 2;

  serializeJson(doc, output);
  */

    // Get a reference to the root object
    StaticJsonDocument<192> doc;
    String output;

    doc["unit"] = _paramHT.unit;
    doc["value"] = _paramHT.value;
    doc["highRange"] = _paramHT.highRange;
    doc["lowRange"] = _paramHT.lowRange;
    doc["highLimit"] = _paramHT.highLimit;
    doc["lowLimit"] = _paramHT.lowLimit;
    doc["alfaEma"] = _paramHT.alfaEma;
    doc["alarm"] = _paramHT.alarm;

    serializeJson(doc, output);
    return output;
}

String AccessParamHT::getStatus()
{
    /*
  {
    "unit":"%",
    "value":51.5,
    "operationMode":2,
    "alarm":2
  }

  StaticJsonDocument<96> doc;

  doc["unit"] = "%";
  doc["value"] = 51.5;
  doc["alarm"] = 2;

  serializeJson(doc, output);
  */

    StaticJsonDocument<96> doc;
    String output;
    doc["unit"] = _paramHT.unit;
    doc["value"] = _paramHT.value;
    doc["alarm"] = _paramHT.alarm;

    serializeJson(doc, output);
    return output;
}

paramHT AccessParamHT::get()
{
    return _paramHT;
}

float AccessParamHT::get(int idParam)
{
    switch (idParam)
    {
    case PARAM_HT_VALUE:
        return _paramHT.value;
        break;

    case PARAM_HT_LOW_RANGE:
        return _paramHT.lowRange;
        break;

    case PARAM_HT_HIGH_RANGE:
        return _paramHT.highRange;
        break;

    case PARAM_HT_LOW_LIMIT:
        return _paramHT.lowLimit;
        break;

    case PARAM_HT_HIGH_LIMIT:
        return _paramHT.highLimit;
        break;

    case PARAM_HT_ALARM:
        return (float)_paramHT.alarm;
        break;

    case PARAM_HT_ALFA_EMA:
        return _paramHT.alfaEma;
        break;

    default:
        break;
    }
}

void AccessParamHT::set(paramHT dataParam)
{

    _paramHT = dataParam;
}

void AccessParamHT::set(int idParam, float val)
{

    switch (idParam)
    {
    case PARAM_HT_VALUE:
        _paramHT.value = val;
        break;

    case PARAM_HT_LOW_RANGE:
        _paramHT.lowRange = val;
        break;

    case PARAM_HT_HIGH_RANGE:
        _paramHT.highRange = val;
        break;

    case PARAM_HT_LOW_LIMIT:
        _paramHT.lowLimit = val;
        break;

    case PARAM_HT_HIGH_LIMIT:
        _paramHT.highLimit = val;
        break;

    case PARAM_HT_ALARM:
        _paramHT.alarm = (int)val;
        break;

    case PARAM_HT_ALFA_EMA:
        _paramHT.alfaEma = val;
        break;

    default:
        break;
    }
}

void AccessParamHT::info()
{
    Serial.println("AccessParamHT::info()");

    Serial.println(this->getJson());
}
