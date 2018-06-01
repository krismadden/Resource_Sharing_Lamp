#include "config.h"
#include <Adafruit_NeoPixel.h>


#define NUM_LEDS 60
#define BRIGHTNESS 255

//assign pins

#define LED_STRIP_PIN 2
#define BRIGHTNESS_UP 14
#define BRIGHTNESS_DOWN 12
#define SOLENOID_PIN 4

const int maxBrightness = 10; //this can be any number - it's the number of steps between dimmest and brightest.
const int interval = 1;
const boolean paris = true;

// variables will change:
int brightness = 0;

int lastValue = 0;
int newValue = 10;

bool upButtonDown = false;
bool downButtonDown = false;

int feed1Data;
int feed2Data;
int otherLight;
bool feed3Data = false;
bool feed4Data = true;
int totalUnits;

/****** uncomment this is your are lausanne. comment this out if you are in paris.*******/
//AdafruitIO_Feed *adafruitFeed1 = io.feed("ecalLausanne"); //this is your lamp location ("parsonsParis" or "ecalLausanne")
//AdafruitIO_Feed *adafruitFeed2 = io.feed("parsonsParis"); // this is the OTHER lamp's location ("parsonsParis" or "ecalLausanne")

/******uncomment this is your are paris. comment this out if you are in lausanne.*******/
AdafruitIO_Feed *adafruitFeed1 = io.feed("parsonsParis"); //this is your lamp location ("parsonsParis" or "ecalLausanne")
AdafruitIO_Feed *adafruitFeed2 = io.feed("ecalLausanne"); // this is the OTHER lamp's location ("parsonsParis" or "ecalLausanne")


AdafruitIO_Feed *adafruitFeed3 = io.feed("bing"); // this feed sends the bing to both locations
AdafruitIO_Feed *adafruitFeed4 = io.feed("startup"); // this feed sends the last resends the last data when one of the lamps is plugged in.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_STRIP_PIN, NEO_GRBW + NEO_KHZ800);


void setup() {

  //lights
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'




  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();


  adafruitFeed1->onMessage(handleMessage_MYunits);
  adafruitFeed2->onMessage(handleMessage_units);
  adafruitFeed3->onMessage(handleMessage_bing);
  adafruitFeed4->onMessage(handleMessage_startup);
  
//commented out setup
//  adafruitFeed4->save(feed4Data);
  adafruitFeed1->save(0);
  // initialize the LED strip pin as an output:
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(SOLENOID_PIN, OUTPUT);
  // initialize the pushbutton pins as an input:
  pinMode(BRIGHTNESS_UP, INPUT_PULLUP);
  pinMode(BRIGHTNESS_DOWN, INPUT_PULLUP);


}

void loop() {
  io.run();
  
  brightness = feed1Data;
  turnOn(feed1Data);
  

  totalUnits = feed1Data + feed2Data;

//if this feed recieved a 1 from adafruit it will flicker the lights and trigger the solenoid
  if (feed3Data) {
    if (brightness > 0) {
      digitalWrite(SOLENOID_PIN, HIGH);    //Switch Solenoid ON
      delay(50);                      //Wait 1 Second
      digitalWrite(SOLENOID_PIN, LOW);     //Switch Solenoid OFF
      delay(50);                      //Wait 1 Second
      digitalWrite(SOLENOID_PIN, HIGH);    //Switch Solenoid ON
      delay(50);                      //Wait 1 Second
      digitalWrite(SOLENOID_PIN, LOW);     //Switch Solenoid OFF

      for (int i = 0; i < maxBrightness; i++) {
        for (int x = 0; x < 6; x++) {
          strip.setPixelColor(i * 6 + x, strip.Color(255, 0, 0) );
        }
      }
      strip.show();
      delay(50);
      turnOn(brightness);
      delay(50);
      for (int i = 0; i < maxBrightness; i++) {
        for (int x = 0; x < 6; x++) {
          strip.setPixelColor(i * 6 + x, strip.Color(255, 0, 0) );
        }
      }
      strip.show();
      delay(100);
      turnOn(brightness);

      feed3Data = false;
    }

  }

//this sends the brightness of this lamp to adafruit when the other lamp is turned on.
//commented out setup
//  if (feed4Data) {
//    adafruitFeed1->save(brightness);
//    feed4Data = false;
//  }


//when the + button is pressed and it was previouslly released
  if (digitalRead(BRIGHTNESS_UP) == LOW && upButtonDown == false) {
    
    //when the total number of units in use is 10 and this lamp's brightness is less than 10
    if(totalUnits >= maxBrightness && brightness < 10){
      //take brightness from the other lamp
      otherLight = feed2Data;
      otherLight = otherLight - interval;
      delay(80);
      adafruitFeed2->save(otherLight);
      
      //turn this lamps' brightness up
      brightness = brightness + interval;
      delay(80);
      adafruitFeed1->save(brightness);
    }
    else if(brightness < 10){
      brightness = brightness + interval;
      delay(80);
      adafruitFeed1->save(brightness);
    }
    //this waits for the button to be released before the button press can be registered again
    upButtonDown = true;
  }


  if (digitalRead(BRIGHTNESS_DOWN) == LOW && brightness > 0 && downButtonDown == false) {
    brightness = brightness - interval;
    delay(80);
    adafruitFeed1->save(brightness);
    downButtonDown = true;
  }


  //  sends a true to adafruit when brightness is full to trigger the solenoid and the flash of the lights
  if (digitalRead(BRIGHTNESS_UP) == LOW && totalUnits >= maxBrightness && upButtonDown == false) {

    //set lights
    adafruitFeed3->save(1);

    upButtonDown = true;

  }


  //make sure the button is released before it can be pressed again
  if (digitalRead(BRIGHTNESS_DOWN) == HIGH) {
    downButtonDown = false;
  }
  if (digitalRead(BRIGHTNESS_UP) == HIGH) {
    upButtonDown = false;
  }

}


void fullOff() {
  //turns all lights off before turning them on
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0) );
  }
  strip.show();
}


void turnOn(int lights) {
  //turns all lights off before turning them on
  //use this instead of fullOff() because it keeps the lights from being updated twice (and flickering)
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0) );
  }
  //turing on lights to white
  for (int i = 0; i < lights; i++) {
    for (int x = 0; x < 6; x++) {
      strip.setPixelColor(i * 6 + x, strip.Color(255, 255, 255) );
    }
  }
  strip.show();
}

void handleMessage_MYunits(AdafruitIO_Data *data) {

  // print RGB values and hex value
  String feed1Data_char = data->value();
  feed1Data = feed1Data_char.toInt();

}

void handleMessage_units(AdafruitIO_Data *data) {

  // print RGB values and hex value
  String feed2Data_char = data->value();
  feed2Data = feed2Data_char.toInt();

}

void handleMessage_bing(AdafruitIO_Data *data) {

  // print RGB values and hex value
  Serial.println("Received : ");
  Serial.println(data->value());
  String feed3Data_char = data->value();
  feed3Data = feed3Data_char.toInt();

}

void handleMessage_startup(AdafruitIO_Data *data) {

  // print RGB values and hex value
  Serial.println("Received startup: ");
  Serial.println(data->value());
  String feed4Data_char = data->value();
  feed4Data = feed4Data_char.toInt();

}
