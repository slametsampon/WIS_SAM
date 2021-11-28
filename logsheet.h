/*
  logsheet.h - Library for DHT wrapper
  By : Sam Sept 04, 2021

  functions : see following function in detail....
*/

#ifndef logsheet_h
#define logsheet_h

#include <Arduino.h>
#include <ArduinoJson.h>

#include "model.h"
#include "utility.h"
#include "sensorHT.h"

const String HEADER = "TIME;TEMPERATURE;HUMIDITY\n";

const int DAY_366 = 366;
const int HOUR_24 = 24;
const int MINUTE_60 = 60;
//const int SECOND_6 = 6;     //average at last one
const int SECOND_60 = 60;   //average at last one
const int TRENDING_24 = 24; //for trending data

const int DAILY_SIZE = 425; //26 + (16*24) = 410

enum DayOfWeek
{
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};
class Logsheet
{
public:
    Logsheet(String);
    void attachFileSystem(FileSystem *);
    void attachSensor(SensorHT *);
    String getHourlyAvg(int);
    String getTrendingData();
    void setTime(struct tm);
    void execute(unsigned long); //sampling periode ms
    void info();

private:
    String _getTimeStr(int);
    String _getDayOfWeek(int);
    void _recordEvent();
    void _recordLogsheet();
    void _minuteLogsheet();
    void _hourlyLogsheet();
    void _dailyLogsheet();
    String _getCsv(logsheetData data);
    void _shiftArray(int size, logsheetData last);
    logsheetData _calculateAverage(logsheetData data[], int size);
    String _readFileJson(int);
    String _initRandomJson();
    void _print(logsheetData data);

    String _id;
    float _prevT, _prevH; //temperature and humidity
    unsigned long _prevMilli, _samplingTime;
    int _samplingSec, _nbrSamplingSec, _samplingTrend;
    boolean _minuteEvent, _hourEvent, _dayEvent;
    boolean _saveHourlyEvent, _saveDailyEvent;
    boolean _synchronized = false;
    struct tm _tm;

    FileSystem *_localStorage;
    SensorHT *_sensorHT;
    logsheetData _logsheetSecond[SECOND_60];
    logsheetData _logsheetMinute[MINUTE_60];
    logsheetData _logsheetTrending[TRENDING_24];
};

#endif
