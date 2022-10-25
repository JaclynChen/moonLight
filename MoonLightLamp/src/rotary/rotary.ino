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
#define BUTTON_CENTER D5 

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

const uint8_t brightness = 128;

int prevButton = 0; 
int buttonMode = 0; 

int start = 0; 
int width = 5; 


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

   pinMode(BUTTON_CENTER, INPUT_PULLUP);


}

void loop(){

  int button = ! digitalRead(BUTTON_CENTER);

  if(button != prevButton){
    if(buttonMode == 2){
      buttonMode = 0; 
    }else{
      buttonMode++; 
    }
    prevButton = button;
    Serial.println(buttonMode);
    delay(200);
  }

  if(buttonMode == 0){
    FastLED.clear(); 
    FastLED.show(); 
    delay(10); 
  }else if(buttonMode == 1){
    fillWhite(); 
  }else if(buttonMode == 2){
    movePixel(); 
  }

 

}

void fillWhite(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGBW(0, 0, 0, 10);
  }
  FastLED.show();
    delay(10);
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

  int color = 0x228B22;

  FastLED.clear(); 
  for(int i = start; i < start + width; i++){
    leds[i] = color; 
  }  
  FastLED.show(); 
  delay(10); 

  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();

 
  if (curr_rotary != last_rotary) {

     int currStart = start; 
  
     if(curr_rotary <= 0){
    currStart = max(0, curr_rotary);

  }else{
    currStart = min(NUM_LEDS - width, curr_rotary);
  }


    FastLED.clear(); 
    for(int i = currStart; i < currStart + width; i++){
      leds[i] = color; 
    }

    start = currStart; 

    FastLED.show(); 
    delay(10);


   
   
   
   /*leds[curr_rotary % NUM_LEDS] = color; 
   FastLED.show();
   delay(10);*/
  }

  last_rotary = curr_rotary;
}



void changeWidth(){

  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();
  
  if(curr_rotary != last_rotary){


  }

}


