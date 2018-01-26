///////////////////////////////////////////////////////////////////
// Class managing the left collar gesture interpreted from the 
// capacitive sensing
//
// Based on ClickButton Class from raron
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once

#include "Arduino.h"
#include "Gesture.h"


class LeftCollar: public Gesture{
public:

    LeftCollar(LightManager* lightManager, int touchPin);
    
    void update(bool currState, int _touchPin);

    void updateGesture(bool currState);
    void printGesture();
    
    int clicks;                   // button click counts to return
    boolean depressed;            // the currently debounced button (press) state (presumably it is not sad :)
    long debounceTime;
    long multiclickTime;
    long longClickTime;
    
  private:
    uint8_t _pin;                 // Arduino pin connected to the button
    boolean _activeHigh;          // Type of button: Active-low = 0 or active-high = 1
    boolean _btnState;            // Current appearant button state
    boolean _lastState;           // previous button reading
    int _clickCount;              // Number of button clicks within multiclickTime milliseconds
    long _lastBounceTime;         // the last time the button input pin was toggled, due to noise or a press
    int function; 
};


LeftCollar::LeftCollar(LightManager* lightManager, int touchPin): Gesture(lightManager,touchPin)
{
  _pin           = 0;
  _activeHigh    = HIGH;           // Assume active-hight button
  _btnState      = !_activeHigh;  // initial button state in active-high logic
  _lastState     = _btnState;
  _clickCount    = 0;
  clicks         = 0;
  depressed      = false;
  _lastBounceTime= 0;
  debounceTime   = 20;            // Debounce timer in ms
  multiclickTime = 200;           // Time limit for multi clicks
  longClickTime  = 500;          // time until long clicks register
  function = 0;
  touchPin = 0;

  this->lightManager=lightManager;
}



 void LeftCollar::update(bool currState, int _touchPin)
 {   
 	  if(touchPin != _touchPin){
 	  	return;
 	  }

     updateGesture(currState);
     printGesture();
 }

 
 void LeftCollar::updateGesture(bool currState)
 {
       long now = (long)millis();      // get current time
      _btnState = currState;  // current appearant button state
      
      // Make the button logic active-high in code
      if (!_activeHigh) _btnState = !_btnState;
      
      // If the switch changed, due to noise or a button press, reset the debounce timer
      if (_btnState != _lastState) _lastBounceTime = now;
      
      
      // debounce the button (Check if a stable, changed state has occured)
      if (now - _lastBounceTime > debounceTime && _btnState != depressed)
      {
        depressed = _btnState;
        if (depressed) _clickCount++;
      }
      
      // If the button released state is stable, report nr of clicks and start new cycle
      if (!depressed && (now - _lastBounceTime) > multiclickTime)
      {
        // positive count for released buttons
        clicks = _clickCount;
        _clickCount = 0;
      }
      
      // Check for "long click"
      if (depressed && (now - _lastBounceTime > longClickTime))
      {
        // negative count for long clicks
        clicks = 0 - _clickCount;
        _clickCount = 0;
      }
      
      _lastState = _btnState;
 }


  void LeftCollar::printGesture()
 {
     // Save click codes in LEDfunction, as click codes are reset at next Update()
      if (clicks != 0) function = clicks;
    
      if(clicks == 1) Serial.println("LeftCollar::SINGLE click");
    
      if(function == 2) {
            Serial.println("LeftCollar::DOUBLE click");
            lightManager->toggleOnOff();
      }
    
      if(function == 3) Serial.println("TLeftCollar::RIPLE click");
    
      if(function == -1)
      {
          //lightManager->setFading(true);
          Serial.println("LeftCollar::SINGLE LONG click");
      }
    
      if(function == -2) Serial.println("LeftCollar::DOUBLE LONG click");
    
      if(function == -3) Serial.println("LeftCollar::TRIPLE LONG click");
    
      function = 0;

 }




