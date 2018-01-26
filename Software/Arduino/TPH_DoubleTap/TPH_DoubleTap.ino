/*
  TPH Double Tap
   
 Description:
 * Software to turn on/off a light with double tap gesture

 Software:
 * Adafruit_MPR121 Library 
 * FastLed
 
 Hardware:
* MPR121 12-channel Capacitive touch sensor
* Adafruit Feather M0 Basic Proto
* Sewable NeoPixel
   
 created 23 January 2018
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gomez

 
 */
 


#include "CapSensorManager.h"
#include "GestureManager.h"
#include "LightManager.h"


LightManager lightManager;
CapSensorManager capSensorManager(&lightManager);
GestureManager gestureManager(&lightManager);


void setup() {
  
    Serial.begin(115200);
    delay(300);
    
    capSensorManager.setup();
    gestureManager.setup();
    lightManager.setup();
}

void loop() {

    capSensorManager.update();
    gestureManager.update(capSensorManager.getCurrentState());
    lightManager.update();
    
  // put a delay so it isn't overwhelming
  //delay(10);
}
