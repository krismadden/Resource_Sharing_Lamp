

/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
//#define IO_USERNAME    "anoukzib"
//#define IO_KEY         "f650630b996a467094b7cb6e10c195d3"
#define IO_USERNAME    "madden_kris"
#define IO_KEY         "eadf1329dc254a10ad333054881a2c2a"

/******************************* WIFI **************************************/

// the AdafruitIO_WiFi client will work with the following boards:
//   - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
//   - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
//   - Feather M0 WiFi -> https://www.adafruit.com/products/3010
//   - Feather WICED -> https://www.adafruit.com/products/3056

//#define WIFI_SSID       "ECALEVENT"
//#define WIFI_PASS       "frutiger"

//#define WIFI_SSID       "NETGEAR86"
//#define WIFI_PASS       "festivekayak5"

#define WIFI_SSID       "Kris' iPhone"
#define WIFI_PASS       "rivertubeblue"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);



