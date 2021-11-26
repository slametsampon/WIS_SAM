#include "model.h"

//AccessParam - Class
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

    String strJson = this->getJson();
    Serial.println(strJson);
}
