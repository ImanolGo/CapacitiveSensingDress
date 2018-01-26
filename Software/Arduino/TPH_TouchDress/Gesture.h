///////////////////////////////////////////////////////////////////
// Base class for all Gestures
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once

#include "Arduino.h"
#include "LightManager.h"


class Gesture{
public:

    //! Constructor
    Gesture(LightManager* _lightManager, int _touchPin): lightManager(_lightManager), touchPin(_touchPin){}

    //! Destructor
    virtual ~Gesture(){
      lightManager = NULL;
    }
    
    //! Draws the BasicVisual
    virtual void setup(){}

    virtual void update(bool currState, int _touchPin) = 0;

    virtual int getTouchPin() { return touchPin; }

    
  protected:
    
    int touchPin;
    LightManager* lightManager;   
};


