#include <Arduino.h>
#include "FastLED.h"
#include "FastLED_RGBW.h"
#include <RotaryEncoder.h>

#define NUM_LEDS 144
#define DATA_PIN D6 //D6?

#define PIN_ENCODER_A D7 //D7
#define PIN_ENCODER_B D1 //D1
#define COM_A    D2 //D2
#define COM_B    D4 //pin D4

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

const uint8_t brightness = 128;

int currPixel = 0; 
int width = 0;


RotaryEncoder encoder(PIN_ENCODER_A, PIN_ENCODER_B, RotaryEncoder::LatchMode::TWO03);
// This interrupt will do our encoder reading/checking!
void checkPosition() {
  encoder.tick(); // just call tick() to check the state.
}
int last_rotary = 0;



void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  FastLED.setBrightness(brightness);
  FastLED.show();
  //Serial.begin(9600);



  Serial.begin(115200);
  while (!Serial);
  Serial.println("ANO Rotary Encoder Demo");

  pinMode(COM_A, OUTPUT);
  digitalWrite(COM_A, LOW);
  pinMode(COM_B, OUTPUT);
  digitalWrite(COM_B, LOW);

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), checkPosition, CHANGE);









}

void loop(){


  //printValue(); 
  movePixel(); 


}

void fillWhite(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGBW(0, 0, 0, 10);
    FastLED.show();
    delay(10);
  }
}

void printValue(){

  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();
  

  if (curr_rotary != last_rotary) {
    Serial.print("Encoder value: ");
    Serial.print(curr_rotary);
    Serial.print(" direction: ");
    Serial.println((int)direction);
  }
  last_rotary = curr_rotary;


}

void movePixel(){


  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();
  

  if (curr_rotary != last_rotary) {
  
  Serial.print("Encoder value: ");
  Serial.print(curr_rotary);
  Serial.print(" direction: ");
  Serial.println((int)direction);
  
   FastLED.clear(); 
   leds[curr_rotary] = 0x228B22;
   FastLED.show();
   delay(10);
  }
  last_rotary = curr_rotary;


/*

  FastLED.clear();  // clear all pixel data
  FastLED.show();

  leds[mappedValue] = 0x228B22;
  FastLED.show();
  delay(10);

  currPixel = mappedValue; */

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


