#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>

#define PIN 7
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_RGBW + NEO_KHZ800);

ezButton limitSwitch(2);

boolean lightState = false; 

//define functions
void onePixel(uint8_t wait);
void fillWhite(); 
void allOff(); 


void setup() {
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
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


  limitSwitch.loop(); // MUST call the loop() function first
 if(limitSwitch.isPressed()){
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
    lightState = !lightState; 
    if(lightState){
      onePixel(10);
    }
    else{
      allOff();
    }
    
 }

 
  if(limitSwitch.isReleased()){
    Serial.println("The limit switch: TOUCHED -> UNTOUCHED");

  }
  /*
  int state = limitSwitch.getState();
  if(state == HIGH)
	  Serial.println("The limit switch: UNTOUCHED");
  else
	  Serial.println("The limit switch: TOUCHED");*/


  //onePixel(10);
  //fillWhite();
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
void fillWhite(){
   for(int i = 0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, 255, 255, 255);
   }
    strip.show();
  
}

void allOff(){
   
    strip.clear();
    strip.show();
  
}


