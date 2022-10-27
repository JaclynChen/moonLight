#include <Arduino.h>
#include "FastLED.h"
#include "FastLED_RGBW.h"
#include <RotaryEncoder.h>

//pins correspond to NodeMCU 1.0 esp-12E 8266

//led strip
#define NUM_LEDS 144
#define DATA_PIN D6  //data pin for led strip


//pinout for rotary encoder: https://learn.adafruit.com/ano-rotary-encoder?view=all 
#define PIN_ENCODER_A D7  //D7
#define PIN_ENCODER_B D1  //D1
#define COM_A D2          //D2
#define COM_B D4          //D4
#define BUTTON_CENTER D5

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *)&leds[0];



//button settings for debouncing
int buttonMode = 0;
int lastButtonState = LOW;
int currentButtonState;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

//default lamp settings
const uint8_t brightness = 128;
int start = 0; //index of the left most led pixel
int width = 1; //number of pixels activated
int color = 0x228B22;


RotaryEncoder encoder(PIN_ENCODER_A, PIN_ENCODER_B, RotaryEncoder::LatchMode::TWO03);
// This interrupt will do our encoder reading/checking!
void checkPosition() {
  encoder.tick();  // just call tick() to check the state.
}
int last_rotary = 0;


void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  FastLED.setBrightness(brightness);
  FastLED.show();
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

void loop() {

  currentButtonState = digitalRead(BUTTON_CENTER);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    if (buttonMode == 3) {
      buttonMode = 0;
    } else {
      buttonMode++;
    }

    //print out button mode for debugging
    Serial.print("Button mode is ");
    Serial.println(buttonMode);
  }

  lastButtonState = currentButtonState;

  if (buttonMode == 0) {
    FastLED.clear();
    FastLED.show();
    delay(10);
  } else if (buttonMode == 1) {
    fillWhite();
  } else if (buttonMode == 2) {
    reset(0x228B22);
    movePixel();
  } else if (buttonMode == 3) {
    reset(0x9370DB);
    changeWidth();
  }
}

//print out rotary info for debugging
void printValue() {
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


//helper method used when there's a mode change
//currColor is the color used for the current mode
void reset(int currColor) {

  FastLED.clear();
  for (int i = start; i < start + width; i++) {
    leds[i] = currColor;
  }
  FastLED.show();
  delay(10);
}

//fills the entire led strip with white pixels
void fillWhite() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGBW(0, 0, 0, 10);
  }
  FastLED.show();
  delay(10);
}


//moves the entire width of activated pixels
void movePixel() {

  //get rotary info
  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();

  //if rotary info changes, adjust pixel location accordingly
  if (curr_rotary != last_rotary) {

    int currStart = start;

    if (curr_rotary <= 0) {
      currStart = max(0, curr_rotary);

    } else {
      currStart = min(NUM_LEDS - width, curr_rotary);
    }

    FastLED.clear();
    for (int i = currStart; i < currStart + width; i++) {
      leds[i] = color;
    }

    start = currStart;

    FastLED.show();
    delay(10);
  }

  last_rotary = curr_rotary;
}


//changes the width of the segment of activated pixels
void changeWidth() {
  int MediumPurple = 0x9370DB;

  //get rotary info
  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();

  if (curr_rotary != last_rotary) {

    int currWidth = width;
    int change = curr_rotary - last_rotary;

    if (change >= 0) {
      //increase width
      int changeLimit = NUM_LEDS - (start + width);
      change = min(change, changeLimit);
      currWidth = width + change;
    } else {
      //decrease width
      currWidth = width + change;
      currWidth = max(1, currWidth);
    }

    FastLED.clear();
    for (int i = start; i < min(NUM_LEDS, start + currWidth); i++) {
      leds[i] = MediumPurple;
    }

    width = currWidth;
    FastLED.show();
    delay(30);
  }
  last_rotary = curr_rotary;
}


//change the color of the led strip
//colors map from 0 - 256 and wrap around
void changeColor() {

  //get rotary info
  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();

  if (curr_rotary != last_rotary) {
  }
  last_rotary = curr_rotary;
}

void changeBrightness() {

  int curr_rotary = encoder.getPosition();
  RotaryEncoder::Direction direction = encoder.getDirection();

  if (curr_rotary != last_rotary) {
  }
  last_rotary = curr_rotary;
}