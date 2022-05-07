#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 7
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_RGBW + NEO_KHZ800);

//define functions
void onePixel(uint8_t wait);

void setup() {
  strip.begin();
  pinMode(PIN,OUTPUT);
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  //strip.setPixelColor(44,strip.Color(0, 255, 0, 255));
  digitalWrite(PIN, HIGH);
}

void loop() {
  //colorSet(strip.Color(255, 255, 255, 255), 100);  //Green
  //onePixel(10); 

  onePixel(10);
  digitalWrite(PIN, HIGH);
}   



//uint8_t
void onePixel(uint8_t wait) {  
// From NeoPixel Library

  uint16_t r = 72; //72
  uint16_t l = 72; //72
  for(uint16_t i = 72; i<strip.numPixels(); i++) {
      strip.setPixelColor(r, 0, 255, 255, 255);
      strip.setPixelColor(l, 0, 255, 255, 255);
      r++; 
      l--; 
      strip.show(); 
      delay(wait); 
      //strip.setPixelColor(i, 0, 0, 0, 0); 
      //strip.show(); 
      //delay(wait); 
    }
     
}