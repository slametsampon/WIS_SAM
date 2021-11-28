#include "sensorHT.h"

SensorHT::SensorHT(String id) : _id(id) {}

void SensorHT::attachSensor(DHT *dht)
{
  _dht = dht;
}

void SensorHT::attachFileSystem(FileSystem *lc)
{
  _localStorage = lc;
}

void SensorHT::attachTempParam(AccessParamHT *tempParam)
{
  _tempParam = tempParam;
}

void SensorHT::attachHumidParam(AccessParamHT *humidParam)
{
  _humidParam = humidParam;
}

String SensorHT::getParam()
{
  String output;

  /*
    {
    "Temperature": {
      "unit":"°C",
      "value": 70.86,
      "highRange":50.0,
      "lowRange":0.0,
      "highLimit":40.0,
      "lowLimit":10.0,
      "alfaEma":80.0
    },
    "Humidity": {
      "unit":"%",
      "value": 70.86,
      "highRange":100.0,
      "lowRange":0.0,
      "highLimit":90.0,
      "lowLimit":40.0,
      "alfaEma":80.0
    }
  }

  StaticJsonDocument<384> doc;

  JsonObject Temperature = doc.createNestedObject("Temperature");
  Temperature["unit"] = "°C";
  Temperature["highRange"] = 50;
  Temperature["lowRange"] = 0;
  Temperature["highLimit"] = 40;
  Temperature["lowLimit"] = 10;
  Temperature["alfaEma"] = 80;

  JsonObject Humidity = doc.createNestedObject("Humidity");
  Humidity["unit"] = "%";
  Humidity["highRange"] = 100;
  Humidity["lowRange"] = 0;
  Humidity["highLimit"] = 90;
  Humidity["lowLimit"] = 40;
  Humidity["alfaEma"] = 80;

  serializeJson(doc, output);

  */

  StaticJsonDocument<384> doc;

  paramHT dtParam = _tempParam->get();
  JsonObject Temperature = doc.createNestedObject("Temperature");
  Temperature["unit"] = dtParam.unit;
  Temperature["value"] = dtParam.value;
  Temperature["highRange"] = dtParam.highRange;
  Temperature["lowRange"] = dtParam.lowRange;
  Temperature["highLimit"] = dtParam.highLimit;
  Temperature["lowLimit"] = dtParam.lowLimit;
  Temperature["alfaEma"] = dtParam.alfaEma;

  dtParam = _humidParam->get();
  JsonObject Humidity = doc.createNestedObject("Humidity");
  Humidity["unit"] = dtParam.unit;
  Humidity["value"] = dtParam.value;
  Humidity["highRange"] = dtParam.highRange;
  Humidity["lowRange"] = dtParam.lowRange;
  Humidity["highLimit"] = dtParam.highLimit;
  Humidity["lowLimit"] = dtParam.lowLimit;
  Humidity["alfaEma"] = dtParam.alfaEma;

  serializeJson(doc, output);
  return output;
}

void SensorHT::setParam()
{
  if (SIMULATION)
    this->_setDefaultParam();
  else
    this->_setFileParam(SENSOR_FILE_CFG);
}

boolean SensorHT::execute(unsigned long samplingTime)
{
  boolean active = false;
  unsigned long currMilli = millis();

  if ((currMilli - _prevMilli) >= samplingTime)
  {
    active = true;
    _prevMilli = currMilli;

    //get sensor value H.T
    this->_getSensorValue();

    //display serial H,T
    this->_displaySerialHT();
  }
  return active;
}

void SensorHT::info()
{
  Serial.println("SensorHT::info()");
  Serial.print("_id : ");
  Serial.println(_id);

  _tempParam->info();
  _humidParam->info();
}

void SensorHT::_setDefaultParam()
{
  paramHT dtParam;
  Serial.println("SensorHT::_setDefaultParam()");

  //parameter temperature
  dtParam.unit = "°C";
  dtParam.value = 35;
  dtParam.highRange = 50;
  dtParam.lowRange = -10;
  dtParam.highLimit = 40;
  dtParam.lowLimit = 10;
  dtParam.alfaEma = ALFA_EMA;
  dtParam.alarm = NO_ALARM;
  _tempParam->set(dtParam);

  //parameter humidity
  dtParam.unit = "%";
  dtParam.value = 70;
  dtParam.highRange = 100;
  dtParam.lowRange = 0;
  dtParam.highLimit = 90;
  dtParam.lowLimit = 40;
  dtParam.alfaEma = ALFA_EMA;
  dtParam.alarm = NO_ALARM;
  _humidParam->set(dtParam);
}

void SensorHT::_setFileParam(String fileName)
{

  /*
    {
    "Temperature": {
      "unit":"°C",
      "value":25,
      "highRange":50.0,
      "lowRange":0.0,
      "highLimit":40.0,
      "lowLimit":10.0,
      "alfaEma":80.0,
      "alarm":0
    },
    "Humidity": {
      "unit":"%",
      "value":80,
      "highRange":100.0,
      "lowRange":0.0,
      "highLimit":90.0,
      "lowLimit":40.0,
      "alfaEma":80.0,
      "alarm":0
    }
  }

  StaticJsonDocument<384> doc;

  JsonObject Temperature = doc.createNestedObject("Temperature");
  Temperature["unit"] = "°C";
  Temperature["value"] = 25;
  Temperature["highRange"] = 50;
  Temperature["lowRange"] = 0;
  Temperature["highLimit"] = 40;
  Temperature["lowLimit"] = 10;
  Temperature["alfaEma"] = 80;
  Temperature["alarm"] = 0;

  JsonObject Humidity = doc.createNestedObject("Humidity");
  Humidity["unit"] = "%";
  Humidity["value"] = 80;
  Humidity["highRange"] = 100;
  Humidity["lowRange"] = 0;
  Humidity["highLimit"] = 90;
  Humidity["lowLimit"] = 40;
  Humidity["alfaEma"] = 80;
  Humidity["alarm"] = 0;

  serializeJson(doc, output);

  ! Stream& input;

  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, input);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  JsonObject Temperature = doc["Temperature"];
  const char* Temperature_unit = Temperature["unit"]; // "°C"
  int Temperature_value = Temperature["value"]; // 25
  int Temperature_highRange = Temperature["highRange"]; // 50
  int Temperature_lowRange = Temperature["lowRange"]; // 0
  int Temperature_highLimit = Temperature["highLimit"]; // 40
  int Temperature_lowLimit = Temperature["lowLimit"]; // 10
  int Temperature_alfaEma = Temperature["alfaEma"]; // 80
  int Temperature_alarm = Temperature["alarm"]; // 0

  JsonObject Humidity = doc["Humidity"];
  const char* Humidity_unit = Humidity["unit"]; // "%"
  int Humidity_value = Humidity["value"]; // 80
  int Humidity_highRange = Humidity["highRange"]; // 100
  int Humidity_lowRange = Humidity["lowRange"]; // 0
  int Humidity_highLimit = Humidity["highLimit"]; // 90
  int Humidity_lowLimit = Humidity["lowLimit"]; // 40
  int Humidity_alfaEma = Humidity["alfaEma"]; // 80
  int Humidity_alarm = Humidity["alarm"]; // 0

  */
  paramHT dtParam;
  Serial.println("SensorHT::_setFileParam(String fileName)");

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

    StaticJsonDocument<512> doc;

    DeserializationError error = deserializeJson(doc, file);

    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    JsonObject Temperature = doc["Temperature"];
    //parameter temperature
    dtParam.unit = "°C";
    dtParam.value = Temperature["value"];
    dtParam.highRange = Temperature["highRange"];
    dtParam.lowRange = Temperature["lowRange"];
    dtParam.highLimit = Temperature["highLimit"];
    dtParam.lowLimit = Temperature["lowLimit"];
    dtParam.alfaEma = Temperature["alfaEma"];
    dtParam.alarm = Temperature["alarm"];
    _tempParam->set(dtParam);

    JsonObject Humidity = doc["Humidity"];
    //parameter humidity
    dtParam.unit = "%";
    dtParam.value = Humidity["value"];
    dtParam.highRange = Humidity["highRange"];
    dtParam.lowRange = Humidity["lowRange"];
    dtParam.highLimit = Humidity["highLimit"];
    dtParam.lowLimit = Humidity["lowLimit"];
    dtParam.alfaEma = Humidity["alfaEma"];
    dtParam.alarm = Humidity["alarm"];
    _humidParam->set(dtParam);

    file.close();
  }
}

String SensorHT::getValues()
{
  /*
    {
        "Temperature": {
        "value": 48.748010,
        "status": "AlarmH"
        },
        "Humidity": {
        "value": 70.86,
        "status": "Normal"
        }
    }
    */
  String output;
  StaticJsonDocument<128> doc;

  float t = _tempParam->get(PARAM_HT_VALUE);
  int tInt = t * 100;
  t = tInt / 100.0; //get 2 digits

  float h = _humidParam->get(PARAM_HT_VALUE);
  int hInt = h * 100;
  h = hInt / 100.0; //get 2 digits

  String statusT = "Normal";
  String statusH = "Normal";

  JsonObject Temperature = doc.createNestedObject("Temperature");
  Temperature["value"] = t;
  Temperature["alarm"] = _tempParam->get(PARAM_HT_ALARM);

  JsonObject Humidity = doc.createNestedObject("Humidity");
  Humidity["value"] = h;
  Humidity["alarm"] = _humidParam->get(PARAM_HT_ALARM);

  serializeJson(doc, output);

  return output;
}

logsheetData SensorHT::getValuesHT()
{
  logsheetData dataHT;
  dataHT.temperature = _tempParam->get(PARAM_HT_VALUE);
  dataHT.humidity = _humidParam->get(PARAM_HT_VALUE);

  return dataHT;
}

void SensorHT::_getSensorValue()
{
  float tRaw, hRaw; //raw data

  float alfaEmaT = _tempParam->get(PARAM_HT_ALFA_EMA);
  float alfaEmaH = _humidParam->get(PARAM_HT_ALFA_EMA);

  float humidityAlarmH = _humidParam->get(PARAM_HT_HIGH_LIMIT);
  float temperatureAlarmH = _tempParam->get(PARAM_HT_HIGH_LIMIT);

  float tempHighLimit = _tempParam->get(PARAM_HT_HIGH_LIMIT) * 10.0;
  float tempLowLimit = _tempParam->get(PARAM_HT_LOW_LIMIT) * 10.0;

  float humidityHighLimit = _humidParam->get(PARAM_HT_HIGH_LIMIT) * 10.0;
  float humidityLowLimit = _humidParam->get(PARAM_HT_LOW_LIMIT) * 10.0;

  // get raw values from sensor
  if (!SIMULATION)
  {
    tRaw = _dht->readTemperature(false);
    hRaw = _dht->readHumidity();
  }
  else
  {
    tRaw = random(tempLowLimit, tempHighLimit) / 10.0;
    hRaw = random(humidityLowLimit, humidityHighLimit) / 10.0;
  }

  //filtering raw value : AlfaEma filter
  float facCurrentT = alfaEmaT / 100.0;
  float facPrevT = (100.0 - alfaEmaT) / 100.0;
  float t = tRaw * facCurrentT + facPrevT * _prevT;

  float facCurrentH = alfaEmaH / 100.0;
  float facPrevH = (100.0 - alfaEmaH) / 100.0;
  float h = hRaw * facCurrentH + facPrevH * _prevH;

  if (_prevT != tRaw)
    _prevT = t;
  if (_prevH != hRaw)
    _prevH = h;

  //set value
  _tempParam->set(PARAM_HT_VALUE, t);
  _humidParam->set(PARAM_HT_VALUE, h);

  //set status
  if (t >= temperatureAlarmH)
  {
    if (_tempParam->get(PARAM_HT_ALARM) != HIGH_ALARM)
      _tempParam->set(PARAM_HT_ALARM, HIGH_ALARM);
  }
  else
    _tempParam->set(PARAM_HT_ALARM, NO_ALARM);

  if (h >= humidityAlarmH)
  {
    if (_humidParam->get(PARAM_HT_ALARM) != HIGH_ALARM)
      _humidParam->set(PARAM_HT_ALARM, HIGH_ALARM);
  }
  else
    _humidParam->set(PARAM_HT_ALARM, NO_ALARM);
}

void SensorHT::_displaySerialHT()
{
  float t = _tempParam->get(PARAM_HT_VALUE);
  float h = _humidParam->get(PARAM_HT_VALUE);

  displaySerial("t", t);
  displaySerial("h", h);
}