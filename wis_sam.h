#ifndef wis_sam_h
#define wis_sam_h

#define DHTPIN 2 // what pin we're connected to ????

#define DHTTYPE DHT11
#define OLED_RESET 0 // GPIO0

const boolean DEBUG = true;
const int DEFAULT_SAMPLING_TIME = 30000; //milli second
const int BLINK_NORMAL = 1000;           //milli second
const int BLINK_WARNING = 250;           //milli second
const int BLINK_ERROR = 125;             //milli second

// Replace with your network credentials
const char *SSID_AP = "wis_sam";
const char *PASSWORD_AP = "wis170466";
const char *SSID1 = "Sam-Laptop";
const char *PASSWORD1 = "sampon170466";
const char *SSID2 = "SamMobile";
const char *PASSWORD2 = "sampon170466";
const char *SSID3 = "SamFamily";
const char *PASSWORD3 = "muwa7x06";
const char *SSID4 = "Fariyo Ganteng";
const char *PASSWORD4 = "Faryganteng";

bool AP_MODE = false; // Do yo want the ESP as AP?

//NTP : Network Time Protokol
const boolean NTP_AVAILABLE = false;
const long timezone = 6; //for jakarta time
const byte daysavetime = 1;

#endif