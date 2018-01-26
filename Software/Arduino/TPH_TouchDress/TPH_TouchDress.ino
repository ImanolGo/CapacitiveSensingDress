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
#include "Gesture.h"
#include "LeftCollar.h"
#include "RightCollar.h"
#include "LightManager.h"



#define NUM_GESTURES 2
#define LEFT_COLLAR_PIN 1
#define RIGHT_COLLAR_PIN 5



LightManager lightManager;
CapSensorManager capSensorManager(&lightManager);
LeftCollar leftCollar(&lightManager, LEFT_COLLAR_PIN);
RightCollar rightCollar(&lightManager, RIGHT_COLLAR_PIN);

// Gesture** gestures = new Gesture*[NUM_GESTURES];
// gestures[0] = new LeftCollar(&lightManager, LEFT_COLLAR_PIN);

  Gesture* gestures[NUM_GESTURES] = {&leftCollar, &rightCollar};
  
//Gesture gestures[] = {leftCollar};


void setup() {
  
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting Software!!!!");
    
    capSensorManager.setup();
    lightManager.setup();
}

void loop() 
{
    capSensorManager.update(gestures,NUM_GESTURES);
    lightManager.update();
    
  // put a delay so it isn't overwhelming
  //delay(10);
}
