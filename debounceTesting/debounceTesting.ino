#include <Arduino.h>
#include <RotaryEncoder.h>


#define PIN_ENCODER_A D7 //D7
#define PIN_ENCODER_B D1 //D1
#define COM_A    D2 //D2
#define COM_B    D4 //pin D4
#define BUTTON_CENTER D5 




int buttonMode = 0; 
int lastButtonState = LOW; 
int currentButtonState;
long lastDebounceTime = 0; // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


RotaryEncoder encoder(PIN_ENCODER_A, PIN_ENCODER_B, RotaryEncoder::LatchMode::TWO03);
// This interrupt will do our encoder reading/checking!
void checkPosition() {
  encoder.tick(); // just call tick() to check the state.
}
int last_rotary = 0;



void setup() { 

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
    if(buttonMode == 2){
      buttonMode = 0; 
    }else{
      buttonMode++; 
    }
  Serial.print("Button mode is ");
  Serial.println(buttonMode);
  
  }

  lastButtonState = currentButtonState; 
  
  delay(200);
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



