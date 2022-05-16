#include <Arduino.h>
#include "FastLED.h"
#include "FastLED_RGBW.h"
#include "noise.h"

#define NUM_LEDS 144
#define DATA_PIN 7

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

const uint8_t brightness = 128;

void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  FastLED.setBrightness(brightness);
  FastLED.show();
  Serial.begin(9600);
}
static int hue = 0;
int noise[NUM_LEDS];
int noise2[NUM_LEDS];

void loop(){
  //fillWhite();
  testfunction1();
}

// messing around with noise function (doesn't really worK)
void testfunction3() {
  for (int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CHSV(noise2[i], 255, noise[i]);  
  }  
}

// should add guassian noise to the hue of the leds
void fillnoise() {
  int scale = 300;
  int speed = 200;
  int z = 2;
  int ioffset = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    ioffset = scale * i;
    noise[i] = inoise8(i * scale);
    noise2[i] = inoise8(i * scale);
    z+= speed;
  }
}

// this one doesn't really work either
void testfunction2() {
  int noise[NUM_LEDS];
  int scale = 300;
  int speed = 20;
  int z = 2;
  int ioffset = 0;

  for (int i = 0; i < NUM_LEDS; i++) {
    ioffset = scale * i;
    noise[i] = inoise8(3 + ioffset, 4, z);
    z+= speed;
  }

  for (int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CHSV(noise[i], 255, noise[i]);  
  }

  FastLED.show();
  hue+=1;
}

// this one is really fricken cool
void testfunction1() {
  int noise[NUM_LEDS];
  int scale = 300;
  int speed = 20;
  int z = 2;
  for (int i = 0; i < NUM_LEDS; i++) {
    noise[i] = inoise8(3, 4, z);
    z+= speed;
  }

  for (int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CHSV(noise[i] + hue, 255, noise[i] + hue);  
  }

  FastLED.show();
  hue+=1;
}

void colorFill(CRGB c){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = c;
    FastLED.show();
    delay(10);
  }
}

void fillWhite(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGBW(0, 0, 0, 10);
    FastLED.show();
    delay(10);
  }
}

void rainbow(){
  static uint8_t hue;

  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV((i * 256 / NUM_LEDS) + hue, 255, 255);
  }
  FastLED.show();
  hue++;
}

void rainbowLoop(){
  long millisIn = millis();
  long loopTime = 5000; // 5 seconds

  while(millis() < millisIn + loopTime){
    rainbow();
    delay(5);
  }
}
