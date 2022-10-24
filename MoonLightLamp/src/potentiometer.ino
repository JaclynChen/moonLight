#include <Arduino.h>
#include "FastLED.h"
#include "FastLED_RGBW.h"
#include "noise.h"

#define NUM_LEDS 144
#define DATA_PIN 4 //D4?

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

const uint8_t brightness = 128;

int currPixel = 0; 
int width = 0;

void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  FastLED.setBrightness(brightness);
  FastLED.show();
  Serial.begin(9600);
}

void loop(){
  //fillWhite();
  //movePixel();
  changeWidth();

}

void fillWhite(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGBW(0, 0, 0, 10);
    FastLED.show();
    delay(10);
  }
}

void movePixel(){
  int potentValue = analogRead(A0); //potentiometer A0 on esp
  int mappedValue = map(potentValue, 0, 1023, 0, 143);
=  FastLED.clear();  // clear all pixel data
  FastLED.show();

  leds[mappedValue] = 0x228B22;
  FastLED.show();
  delay(10);

  currPixel = mappedValue; 

}

//save potentiometer value
//save current mode

void changeWidth(){

  //int centerPixel = 22; 
  int increaseLimit = 144 - currPixel - width; 
  int potentValue = analogRead(A0); //potentiometer A0 on esp
  int mappedValue = map(potentValue, 0, 1023, 0, increaseLimit);

  if(mappedValue < width){
    for(int i = currPixel + width; i > currPixel; i--){
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(30);
    }
  }else{
    for(int i = currPixel; i < currPixel + mappedValue; i++){
      leds[i] = 0x228B22;
      FastLED.show();
      delay(30);
    }
  }

  //width = mappedValue; 

  // leds[0] = CRGB::Black; FastLED.show(); delay(30);

  //currPixel = currPixel + mappedValue; 
  FastLED.show();


}


