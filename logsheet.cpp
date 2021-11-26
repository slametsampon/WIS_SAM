/*
by : Sam 04/09/2021

*/

#include <CSV_Parser.h>
#include "logsheet.h"

Logsheet::Logsheet(String id) : _id(id)
{
    _prevMilli = millis();
    _samplingTrend = 0;
}

void Logsheet::attachFileSystem(FileSystem *lc)
{
    _localStorage = lc;
}

void Logsheet::info()
{
    Serial.println("Logsheet::info()");
}

String Logsheet::getHourlyAvg(int dWeek)
{
    return (this->_readFileJson(dWeek));
}

String Logsheet::getTrendingData()
{
    /*
    {
    "Time": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23],
    "Temperature": [20.00, 20.01, 20.02, 20.03, 20.04, 20.05, 20.06, 20.07, 20.08, 20.09,
                 20.10, 20.11, 20.12, 20.13, 20.14, 20.15, 20.16, 20.17, 20.18, 20.19, 20.20, 20.21, 20.22, 20.23],
    "Humidity": [70.00, 70.01, 70.02, 70.03, 70.04, 70.05, 70.06, 70.07, 70.08, 70.09,
                 70.10, 70.11, 70.12, 70.13, 70.14, 70.15, 70.16, 70.17, 70.18, 70.19, 70.20, 70.21, 70.22, 70.23]
    }

    DynamicJsonDocument doc(1536);    
  */

    //Json assembling
    String output;
    DynamicJsonDocument doc(1536);
    float h, t;
    doc.clear();
    JsonArray time = doc.createNestedArray("time");
    JsonArray temperature = doc.createNestedArray("temperature");
    JsonArray humidity = doc.createNestedArray("humidity");

    for (int row = 0; row < TRENDING_24; row++)
    {

        String Ti = String(_logsheetTrending[row].time);
        String T = String(_logsheetTrending[row].temperature);
        String H = String(_logsheetTrending[row].humidity);

        time.add(Ti.toInt());
        temperature.add(T.toFloat());
        humidity.add(H.toFloat());
    }

    serializeJson(doc, output);
    return output;
}

String Logsheet::_initRandomJson()
{
    String output;
    DynamicJsonDocument doc(1536);
    float h, t;
    doc.clear();
    JsonArray time = doc.createNestedArray("time");
    JsonArray temperature = doc.createNestedArray("temperature");
    JsonArray humidity = doc.createNestedArray("humidity");

    float tempHighLimit = 45;
    float tempLowLimit = 0;

    float humidityHighLimit = 80;
    float humidityLowLimit = 40;

    for (int i = 0; i < 24; i++)
    {
        t = random(tempLowLimit, tempHighLimit) / 10.0;
        h = random(humidityLowLimit, humidityHighLimit) / 10.0;

        time.add(i);
        temperature.add(t);
        humidity.add(h);
    }

    serializeJson(doc, output);
    return output;
}

void Logsheet::execute(unsigned long samplingTime)
{

    //set samplingTime
    _samplingTime = samplingTime;

    //calculate sampling per minute
    _nbrSamplingSec = 60000 / _samplingTime;
    if (_nbrSamplingSec < 1)
        _nbrSamplingSec = 1;
    else if (_nbrSamplingSec > SECOND_60)
        _nbrSamplingSec = SECOND_60;

    unsigned long currMilli = millis();

    if ((currMilli - _prevMilli) >= samplingTime)
    {
        _prevMilli = currMilli;

        //record event
        this->_recordEvent();

        //record logsheet
        this->_recordLogsheet();
    }
}

void Logsheet::_recordEvent()
{

    //handle trending sample
    if (_samplingTrend >= (TRENDING_24 - 1))
    {
        _samplingTrend = 0;
    }
    else
        _samplingTrend++;

    //handle sampling second
    if (_samplingSec >= (_nbrSamplingSec - 1))
    {
        _samplingSec = 0;
        _minuteEvent = true;
    }
    else
        _samplingSec++;
    /*
    //handle sampling minute
    if (_tm.tm_min == 0)
    {
        _hourEvent = true;
        Serial.print("_hourEvent : ");
        Serial.println(_hourEvent);
    }
    else
    {
        _hourEvent = false;
        _saveHourlyEvent = false;
    }

    //handle sampling hour
    if (_tm.tm_hour == 0)
        _dayEvent = true;
    else
    {
        _dayEvent = false;
        _saveDailyEvent = false;
    }
    */
}

void Logsheet::_recordLogsheet()
{
}

void Logsheet::_minuteLogsheet()
{
    _minuteEvent = false;
    Serial.print("Logsheet::_recordLogsheet() => _samplingMinute : ");
    //Serial.println(_tm.tm_min);

    //calculate average minute
    logsheetData avgMinute = _calculateAverage(_logsheetSecond, _nbrSamplingSec);

    //shift record minute & put average to bottom
    _shiftArray(MINUTE_60, avgMinute);
}

void Logsheet::_hourlyLogsheet()
{
}

void Logsheet::_dailyLogsheet()
{
}

String Logsheet::_getCsv(logsheetData data)
{

    String csv = data.time + ";";

    csv += String(data.temperature);
    csv += ";";

    csv += String(data.humidity);
    csv += "\n";

    return csv;
}

void Logsheet::_shiftArray(int size, logsheetData last)
{

    switch (size)
    {
    case SECOND_6:
        for (int i = 0; i < (size - 1); i++)
        {
            _logsheetSecond[i].temperature = _logsheetSecond[i + 1].temperature;
            _logsheetSecond[i].humidity = _logsheetSecond[i + 1].humidity;
            _logsheetSecond[i].time = _logsheetSecond[i + 1].time;

            //print to serial
            //_print(_logsheetSecond[i]);
        }
        _logsheetSecond[size - 1].temperature = last.temperature;
        _logsheetSecond[size - 1].humidity = last.humidity;
        _logsheetSecond[size - 1].time = last.time;

        //print to serial
        //_print(_logsheetSecond[size - 1]);
        break;

    case TRENDING_24:
        for (int i = 0; i < (size - 1); i++)
        {
            _logsheetTrending[i].temperature = _logsheetTrending[i + 1].temperature;
            _logsheetTrending[i].humidity = _logsheetTrending[i + 1].humidity;
            _logsheetTrending[i].time = _logsheetTrending[i + 1].time;

            //print to serial
            //_print(_logsheetTrending[i]);
        }
        _logsheetTrending[size - 1].temperature = last.temperature;
        _logsheetTrending[size - 1].humidity = last.humidity;
        _logsheetTrending[size - 1].time = last.time;

        //print to serial
        //_print(_logsheetTrending[size - 1]);
        break;

    case MINUTE_60:
        for (int i = 0; i < (size - 1); i++)
        {
            _logsheetMinute[i].temperature = _logsheetMinute[i + 1].temperature;
            _logsheetMinute[i].humidity = _logsheetMinute[i + 1].humidity;
            _logsheetMinute[i].time = _logsheetMinute[i + 1].time;

            //print to serial
            //_print(_logsheetMinute[i]);
        }
        _logsheetMinute[size - 1].temperature = last.temperature;
        _logsheetMinute[size - 1].humidity = last.humidity;
        _logsheetMinute[size - 1].time = last.time;

        //print to serial
        //_print(_logsheetMinute[size - 1]);
        break;
    default:
        break;
    }
}

logsheetData Logsheet::_calculateAverage(logsheetData data[], int size)
{
    float totalT, totalH;
    logsheetData avg;
    int nbrData = 0;

    return avg;
}

void Logsheet::_print(logsheetData data)
{
    String str = data.time;

    str = String(str + "=> Temperature : ");
    str = String(str + data.temperature);

    str = String(str + ", Humidity : ");
    str = String(str + data.humidity);

    Serial.println(str);
}

String Logsheet::_getDayOfWeek(int dayNumber)
{
    String dayOfWeek;
    switch (dayNumber)
    {
    case Monday:
        dayOfWeek = "Monday";
        break;

    case Tuesday:
        dayOfWeek = "Tuesday";
        break;

    case Wednesday:
        dayOfWeek = "Wednesday";
        break;

    case Thursday:
        dayOfWeek = "Thursday";
        break;

    case Friday:
        dayOfWeek = "Friday";
        break;

    case Saturday:
        dayOfWeek = "Saturday";
        break;

    case Sunday:
        dayOfWeek = "Sunday";
        break;

    default:
        break;
    }

    return dayOfWeek;
}
