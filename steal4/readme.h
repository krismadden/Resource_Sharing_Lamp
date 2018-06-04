Install Feather Board Manager
> Arduino > Preferences > Additional Board Managers URLs > "http://arduino.esp8266.com/stable/package_esp8266com_index.json"

> Tools > Boards Manager > "esp8266" > Install

Install AdafruitIO_WiFi Library
> Sketch > Include Libraries > Manage Libraries > "Adafruit IO Arduino" > Install

Install Adafruit_MQTT Library
> Sketch > Include Libraries > Manage Libraries > "Adafruit MQTT Library" > Install

Install ArduinoHttpClient Library
> Sketch > Include Libraries > Manage Libraries > "ArduinoHttpClient" > Install

Install Adafruit_NeoPixel Library
> Sketch > Include Libraries > Manage Libraries > "Adafruit NeoPixel" > Install

Connect to Wifi
> config.h > #define WIFI_SSID "[Your Wifi Network Name in Quotes]"
> config.h > #define WIFI_PASS "[Your Wifi Network's Password in Quotes]"

Connect to the Adafruit.io feed
> config.h > #define IO_USERNAME "[Your adafruit.io username in quotes]"
> config.h > #define IO_KEY "[Your adafruit.io key in quotes]"

Plug buttons and LEDs in
> twoButtons_09 > #define LED_STRIP_PIN [The pin number of where your white wire of the neo pixel is plugged to the Feather] 
> twoButtons_09 > #define BRIGHTNESS_UP [The pin number of where your button to turn the light up is plugged to the Feather]
> twoButtons_09 > #define BRIGHTNESS_DOWN [The pin number of where your button to turn the light down is plugged to the Feather]

Connect to Adafruit Feeds
> twoButtons_09 > AdafruitIO_Feed *adafruitFeed1 = io.feed(["ecalLausanne"]);
> twoButtons_09 > AdafruitIO_Feed *adafruitFeed2 = io.feed(["parsonsParis"]);

