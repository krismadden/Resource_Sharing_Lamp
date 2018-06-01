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

// variables will change:
int brightness = 0;

int lastValue = 0;
int newValue = 10;

bool upButtonDown = false;
bool downButtonDown = false;

int feed2Data;
bool feed3Data = false;
bool feed4Data = true;
bool feed5Data = false;
int totalUnits;


/****** uncomment this is your are lausanne. comment this out if you are in paris.*******/
//AdafruitIO_Feed *adafruitFeed1 = io.feed("ecalLausanne"); //this is your lamp location ("parsonsParis" or "ecalLausanne")
//AdafruitIO_Feed *adafruitFeed2 = io.feed("parsonsParis"); // this is the OTHER lamp's location ("parsonsParis" or "ecalLausanne")
//AdafruitIO_Feed *adafruitFeed3 = io.feed("ecalSteal"); // this feed sends a true if you are stealing
//AdafruitIO_Feed *adafruitFeed5 = io.feed("parisSteal"); // this feed sends a true if the other location is stealing

/******uncomment this is your are paris. comment this out if you are in lausanne.*******/
AdafruitIO_Feed *adafruitFeed1 = io.feed("parsonsParis"); //this is your lamp location ("parsonsParis" or "ecalLausanne")
AdafruitIO_Feed *adafruitFeed2 = io.feed("ecalLausanne"); // this is the OTHER lamp's location ("parsonsParis" or "ecalLausanne")
AdafruitIO_Feed *adafruitFeed3 = io.feed("parisSteal"); // this feed sends a true if you are stealing
AdafruitIO_Feed *adafruitFeed5 = io.feed("ecalSteal"); // this feed sends a true if the other location is stealing


AdafruitIO_Feed *adafruitFeed4 = io.feed("startup"); // this feed sends the last resends the last data when one of the lamps is plugged in.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_STRIP_PIN, NEO_GRBW + NEO_KHZ800);


void setup() {

  //lights
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'



  
  Serial.begin(115200);

    // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());


  adafruitFeed2->onMessage(handleMessage_units);
  adafruitFeed3->onMessage(handleMessage_bing);
  adafruitFeed5->onMessage(handleMessage_bing5);
  adafruitFeed4->onMessage(handleMessage_startup);

  adafruitFeed4->save(feed4Data);
  // initialize the LED strip pin as an output: 
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(SOLENOID_PIN, OUTPUT);       
  // initialize the pushbutton pins as an input:
  pinMode(BRIGHTNESS_UP, INPUT_PULLUP);     
  pinMode(BRIGHTNESS_DOWN, INPUT_PULLUP);   
  

}

void loop(){
    io.run();
    
    turnOn(brightness);

    totalUnits = brightness + feed2Data;


    //they are stealing
    if(feed5Data){

      if(brightness > 0){
         brightness = brightness - interval;
         newValue = brightness;
      }
      if(brightness > 0 && totalUnits < maxBrightness{
        digitalWrite(SOLENOID_PIN, HIGH);    //Switch Solenoid ON
        delay(50);                      //Wait 1 Second
        digitalWrite(SOLENOID_PIN, LOW);     //Switch Solenoid OFF
        delay(50);                      //Wait 1 Second
        digitalWrite(SOLENOID_PIN, HIGH);    //Switch Solenoid ON
        delay(50);                      //Wait 1 Second
        digitalWrite(SOLENOID_PIN, LOW);     //Switch Solenoid OFF

        for(int i=0; i<maxBrightness; i++) {
          for(int x=0; x<6; x++){
            strip.setPixelColor(i*6 + x, strip.Color(255,0,0) );
          }
        }
        strip.show();
        delay(50);
        turnOn(brightness);
        delay(50);
        for(int i=0; i<maxBrightness; i++) {
          for(int x=0; x<6; x++){
            strip.setPixelColor(i*6 + x, strip.Color(255,0,0) );
          }
        }
        strip.show();
        delay(100);
        turnOn(brightness);

       feed5Data = false;
      }
    }
    

    //you are stealing
    if(feed3Data){

      if(brightness < maxBrightness){
         brightness = brightness + interval;
         newValue = brightness;
      }
      if(brightness > 0 && totalUnits < maxBrightness{
        digitalWrite(SOLENOID_PIN, HIGH);    //Switch Solenoid ON
        delay(50);                      //Wait 1 Second
        digitalWrite(SOLENOID_PIN, LOW);     //Switch Solenoid OFF
        delay(50);                      //Wait 1 Second
        digitalWrite(SOLENOID_PIN, HIGH);    //Switch Solenoid ON
        delay(50);                      //Wait 1 Second
        digitalWrite(SOLENOID_PIN, LOW);     //Switch Solenoid OFF

        for(int i=0; i<maxBrightness; i++) {
          for(int x=0; x<6; x++){
            strip.setPixelColor(i*6 + x, strip.Color(255,0,0) );
          }
        }
        strip.show();
        delay(50);
        turnOn(brightness);
        delay(50);
        for(int i=0; i<maxBrightness; i++) {
          for(int x=0; x<6; x++){
            strip.setPixelColor(i*6 + x, strip.Color(255,0,0) );
          }
        }
        strip.show();
        delay(100);
        turnOn(brightness);

       feed3Data = false;
      }
    }
    
    if(feed4Data){
      adafruitFeed1->save(brightness);
      feed4Data = false;
    }
   
    if (digitalRead(BRIGHTNESS_UP) == LOW && totalUnits < maxBrightness && upButtonDown == false){ 
      brightness = brightness + interval;
//      Serial.print("going up to -> ");
//      Serial.println(brightness); //for debugging purposes
      newValue = brightness;
      upButtonDown = true;
    }
    
    if (digitalRead(BRIGHTNESS_DOWN) == LOW && brightness > 0 && downButtonDown == false){
      brightness = brightness - interval;
//      Serial.print("going down to -> ");
//      Serial.println(brightness); //for debugging purposes
      newValue = brightness;
      downButtonDown = true;
    } 

     
    //  sends a true to adafruit when brightness is full
    int bUp = digitalRead(BRIGHTNESS_UP) == HIGH ? LOW : HIGH;
    if (bUp  == HIGH && totalUnits >= maxBrightness && upButtonDown == false){

       adafruitFeed3->save(1);
       upButtonDown = true;

    }
    delay(80);
    
    // only push to adafruit when the value changes
    if(lastValue != newValue){
        lastValue = newValue;
        adafruitFeed1->save(brightness);
//        Serial.print("sending -> ");
//        Serial.println(brightness); //for debugging purposes
    }


    //make sure the button is released before it can be pressed again
    if (digitalRead(BRIGHTNESS_DOWN) == HIGH){
      downButtonDown = false;
    }
    if (digitalRead(BRIGHTNESS_UP) == HIGH){
      upButtonDown = false;
    }

}


void fullOff() {
    //turns all lights off before turning them on
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0) );
    }
      strip.show();
}


void turnOn(int lights){
  //turns all lights off before turning them on
  //use this instead of fullOff() because it keeps the lights from being updated twice (and flickering)
  for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0) );
    }
    //turing on lights to white
  for(int i=0; i<lights; i++) {
        for(int x=0; x<6; x++){
          strip.setPixelColor(i*6 + x, strip.Color(255,255,255) );
        }
    }
      strip.show();
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

void handleMessage_bing5(AdafruitIO_Data *data) {

  // print RGB values and hex value
  Serial.println("Received : ");
  Serial.println(data->value());
  String feed5Data_char = data->value();
  feed5Data = feed5Data_char.toInt();

}

void handleMessage_startup(AdafruitIO_Data *data) {

  // print RGB values and hex value
  Serial.println("Received startup: ");
  Serial.println(data->value());
  String feed4Data_char = data->value();
  feed4Data = feed4Data_char.toInt();

}
