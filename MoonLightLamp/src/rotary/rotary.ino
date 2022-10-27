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


int buttonMode = 0; 
int lastButtonState = LOW; 
int currentButtonState;
long lastDebounceTime = 0; // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


int start = 0; 
int width = 5; 

int color = 0x228B22;


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



  currentButtonState = digitalRead(BUTTON_CENTER);
  
  if(lastButtonState == HIGH && currentButtonState == LOW){
    if(buttonMode == 3){
      buttonMode = 0; 
    }else{
      buttonMode++; 
    }
  Serial.print("Button mode is ");
  Serial.println(buttonMode);
  
  }

  lastButtonState = currentButtonState; 

  if(buttonMode == 0){
    FastLED.clear(); 
    FastLED.show(); 
    delay(10); 
  }else if(buttonMode == 1){
    fillWhite(); 
  }else if(buttonMode == 2){
    movePixel(); 
  }else if(buttonMode == 3){
    reset(0x9370DB); 
    changeWidth(); 
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
  }

  last_rotary = curr_rotary;
}


void reset(int currColor){

  FastLED.clear(); 
  for(int i = start; i < start + width; i++){
    leds[i] = currColor; 
  }  
  FastLED.show(); 
  delay(10); 


}



void changeWidth(){

  int MediumPurple = 0x9370DB; 

  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();

  if(curr_rotary != last_rotary){

    int currWidth = width; 
    int change = curr_rotary - last_rotary; 

    if(change >= 0){
      //increase width
      int changeLimit = NUM_LEDS - (start + width);
      change = min(change, changeLimit);
      currWidth = width + change; 
    }else{
      //decrease width
      currWidth = width + change; 
      currWidth = max(1, currWidth);
    }

    FastLED.clear(); 

    for(int i = start; i < min(NUM_LEDS, start + currWidth); i++){
      leds[i] = MediumPurple; 
    }

    width = currWidth; 
    FastLED.show(); 
    delay(30);

  
  }
  last_rotary = curr_rotary;

}


