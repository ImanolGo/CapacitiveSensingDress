///////////////////////////////////////////////////////////////////
// A Capacitive Sensor interface for reading from a MPR121 module
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include <Wire.h>
#include "Adafruit_MPR121.h"

#include "LightManager.h"

class CapSensorManager{
public:

  CapSensorManager(LightManager* lightManager);
  
  /// set all information necessary to use the module and initialize it
  void setup();

  void initializeMPR121();
  
  void update();       ///< checks the current state

  uint8_t getCurrentPad() const {return currentPad;}  
  bool getCurrentState() const {return currState;}  
  
private:

    Adafruit_MPR121*  cap;  //Class controlling the MPR121 module
    uint16_t lasttouched = 0;
    uint16_t currtouched = 0;
    uint8_t currentPad = 0;
    bool currState = LOW;

    LightManager* lightManager;
    
};



CapSensorManager::CapSensorManager(LightManager* lightManager)
{
     this->lightManager=lightManager;
}

void CapSensorManager::setup()
{
    initializeMPR121();
}

void CapSensorManager::initializeMPR121()
{
    cap = new Adafruit_MPR121();
    Serial.println("Starting Adafruit MPR121 Capacitive Touch"); 
    
    // Default address is 0x5A, if tied to 3.3V its 0x5B
    // If tied to SDA its 0x5C and if SCL then 0x5D
    if (!cap->begin(0x5A)) {
      Serial.println("MPR121 not found, check wiring?");
      while (1);
    }
    Serial.println("MPR121 found!");
}


 void CapSensorManager::update()
 {
      
      // Get the currently touched pads
      currtouched = cap->touched();
      
      for (uint8_t i=0; i<12; i++) {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched &  (1<<i)) && !(lasttouched &  (1<<i)) ) {
          //Serial.print(i); Serial.println(" touched");
          currentPad = i;
          currState = true;

         // float hue = map(i, 0, 11, 0, 255);
          //lightManager->setColorHue(hue);

          //Serial.print("CapSensorManager::hue-> "); Serial.println(hue);
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched & (1<<i)) && (lasttouched & (1<<i)) ) {
          // Serial.print(i); Serial.println(" released");
           currentPad = i;
           currState = false;
        }
      }
    
        //Serial.print(latestButton); Serial.print(", "); Serial.println(buttonState);
      // reset our state
      lasttouched = currtouched;
 }















